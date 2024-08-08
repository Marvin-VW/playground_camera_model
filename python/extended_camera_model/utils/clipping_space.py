# Copyright (C) 2024 Marvin-VW
import numpy as np
import copy

class Clipping_Space:
    def __init__(self) -> None:

        fov = np.deg2rad(64/1.77)
        aspect_ratio = 1.77
        near = 1
        far = 100.0
        self.projection_matrix = self.create_perspective_projection_matrix(fov, aspect_ratio, near, far)
        self.border = 1

    def create_perspective_projection_matrix(self, fov, aspect_ratio, near, far):
        
        f = 1.0 / np.tan(fov / 2)
        nf = 1 / (near - far)
        
        return np.array([
            [f / aspect_ratio, 0, 0, 0],
            [0, f, 0, 0],
            [0, 0, (far + near) * nf, (2 * far * near) * nf],
            [0, 0, -1, 0]
        ])

    def cube_in_space(self, cube_points: list):
        full_triangle_list = []

        planes = [
            ("left", [-self.border, 0, 0]),
            ("right", [self.border, 0, 0]),
            ("bottom", [0, -self.border, 0]),
            ("top", [0, self.border, 0])
        ]

        for triangle in cube_points:
            triangles_to_clip = [triangle]

            for plane_name, plane_position in planes:
                new_triangles = []

                for triangle_to_clip in triangles_to_clip:
                    inside_points = []
                    outside_points = []

                    for point in triangle_to_clip.camera_points:
                        clip_space_point = np.matmul(self.projection_matrix, point)
                        ndc_point = clip_space_point / clip_space_point[3]

                        if self.is_inside_plane(ndc_point, plane_name):
                            inside_points.append(ndc_point)
                        else:
                            outside_points.append(ndc_point)

                    if len(inside_points) == 3:
                        new_triangles.append(triangle_to_clip)

                    elif len(inside_points) == 0:
                        continue

                    elif len(inside_points) == 1:
                        new_point1 = self.find_intersection_with_plane(inside_points[0], outside_points[0], plane_name)
                        new_point2 = self.find_intersection_with_plane(inside_points[0], outside_points[1], plane_name)

                        if new_point1 is not None and new_point2 is not None:
                            new_triangle_points = [
                                inside_points[0],
                                np.vstack(new_point1.reshape(-1, 1)),
                                np.vstack(new_point2.reshape(-1, 1))
                            ]

                            for pos, point in enumerate(new_triangle_points):
                                new_triangle_points[pos] = self.transfer_back_camera_space(point)

                            new_triangle = copy.deepcopy(triangle_to_clip)
                            new_triangle.camera_points = new_triangle_points
                            new_triangles.append(new_triangle)

                    elif len(inside_points) == 2:
                        new_point1 = self.find_intersection_with_plane(inside_points[0], outside_points[0], plane_name)
                        new_point2 = self.find_intersection_with_plane(inside_points[1], outside_points[0], plane_name)

                        if new_point1 is not None:
                            new_triangle_points1 = [
                                np.vstack(new_point1.reshape(-1, 1)),
                                np.vstack(new_point2.reshape(-1, 1)) if new_point2 is not None else inside_points[1],
                                inside_points[0]
                            ]

                            for pos, point in enumerate(new_triangle_points1):
                                new_triangle_points1[pos] = self.transfer_back_camera_space(point)

                            triangle_new1 = copy.deepcopy(triangle_to_clip)
                            triangle_new1.camera_points = new_triangle_points1
                            new_triangles.append(triangle_new1)

                        if new_point2 is not None:
                            new_triangle_points2 = [
                                inside_points[0],
                                np.vstack(new_point2.reshape(-1, 1)),
                                inside_points[1]
                            ]

                            for pos, point in enumerate(new_triangle_points2):
                                new_triangle_points2[pos] = self.transfer_back_camera_space(point)

                            triangle_new2 = copy.deepcopy(triangle_to_clip)
                            triangle_new2.camera_points = new_triangle_points2
                            new_triangles.append(triangle_new2)

                triangles_to_clip = new_triangles

            full_triangle_list.extend(triangles_to_clip)

        return full_triangle_list


    @staticmethod
    def is_inside_plane(point, plane_name):
        if plane_name == "left":
            return point[0] >= -1
        elif plane_name == "right":
            return point[0] <= 1
        elif plane_name == "bottom":
            return point[1] >= -1
        elif plane_name == "top":
            return point[1] <= 1
        return False
    
    @staticmethod
    def intersection_with_plane_x(A, B, x):
        if A[0] == B[0]:
            return None
        t = (x - A[0]) / (B[0] - A[0])
        if 0 <= t <= 1:
            intersection = A + t * (B - A)
            return intersection
        return None

    @staticmethod
    def intersection_with_plane_y(A, B, y):
        if A[1] == B[1]:
            return None
        t = (y - A[1]) / (B[1] - A[1])
        if 0 <= t <= 1:
            intersection = A + t * (B - A)
            return intersection
        return None

    def find_intersection_with_plane(self, point1, point2, plane_name):
        if plane_name == "left":
            return self.intersection_with_plane_x(point1, point2, -self.border)
        elif plane_name == "right":
            return self.intersection_with_plane_x(point1, point2, self.border)
        elif plane_name == "bottom":
            return self.intersection_with_plane_y(point1, point2, -self.border)
        elif plane_name == "top":
            return self.intersection_with_plane_y(point1, point2, self.border)
        return None


    def transfer_back_camera_space(self, point):
        converted_point = np.matmul(np.linalg.inv(self.projection_matrix), point)
        converted_point /= converted_point[3]
        return converted_point
