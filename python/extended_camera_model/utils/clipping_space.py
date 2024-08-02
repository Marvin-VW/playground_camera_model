# Copyright (C) 2024 Marvin-VW
import numpy as np

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

        #for each triangle:
        for tuple in cube_points:

            full_point_list = []
            inside_point = []
            outside_point = []

            for point in tuple:
                clip_space_point = np.matmul(self.projection_matrix, point)
                ndc_point = clip_space_point / clip_space_point[3]

                #check which points are in space
                if -self.border <= ndc_point[0] <= self.border and -self.border <= ndc_point[1] <= self.border and 1 <= ndc_point[2] <= 100:
                    inside_point.append(ndc_point)
                else:
                    outside_point.append(ndc_point)


            #all points inside -> return points
            if len(inside_point) == 3:
                for point in inside_point:
                    ndc_point = np.matmul(np.linalg.inv(self.projection_matrix), point)
                    full_point_list.append(ndc_point)

                full_triangle_list.append(full_point_list)

            #no points inside -> return none
            elif len(inside_point) == 0:
                continue

            #one point inside -> two new points
            elif len(inside_point) == 1:
                _, new_point1 = self.find_intersection_with_plane(inside_point[0], outside_point[0])
                _, new_point2 = self.find_intersection_with_plane(inside_point[0], outside_point[1])

                full_point_list.append(np.vstack([new_point1.reshape(-1, 1), [[1]]]))
                full_point_list.append(inside_point[0])
                full_point_list.append(np.vstack([new_point2.reshape(-1, 1), [[1]]]))


                for pos, point in enumerate(full_point_list):
                    full_point_list[pos] = np.matmul(np.linalg.inv(self.projection_matrix), point)

                print("two new points")
                print(full_point_list)
        
                full_triangle_list.append(full_point_list)

            #two points inside -> two new triangles
            elif len(inside_point) == 2:
                _, new_point1 = self.find_intersection_with_plane(inside_point[0], outside_point[0])
                _, new_point2 = self.find_intersection_with_plane(inside_point[1], outside_point[0])

                #first triangle
                full_point_list.append(np.vstack([new_point1.reshape(-1, 1), [[1]]]))
                full_point_list.append(inside_point[0])
                full_point_list.append(np.vstack([new_point2.reshape(-1, 1), [[1]]]))
                
                for pos, point in enumerate(full_point_list):
                    full_point_list[pos] = np.matmul(np.linalg.inv(self.projection_matrix), point)

                print("two new triangles (1)")
                print(full_point_list)

                full_triangle_list.append(full_point_list)
                full_point_list = []
        
                #second triangle
                full_point_list.append(np.vstack([new_point2.reshape(-1, 1), [[1]]]))
                full_point_list.append(inside_point[0])
                full_point_list.append(inside_point[1])
                
                for pos, point in enumerate(full_point_list):
                    full_point_list[pos] = np.matmul(np.linalg.inv(self.projection_matrix), point)

                print("two new triangles (2)")
                print(full_point_list)

                full_triangle_list.append(full_point_list)

        return full_triangle_list
    

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

    def find_intersection_with_plane(self, point1, point2):

        planes_x = [-self.border, self.border]
        planes_y = [-self.border, self.border]

        A = point1.flatten()
        A = A[:3]
        B = point2.flatten()
        B = B[:3]

        #get intersections with borders
        intersections = {
            "left": self.intersection_with_plane_x(A, B, planes_x[0]),
            "right": self.intersection_with_plane_x(A, B, planes_x[1]),
            "bottom": self.intersection_with_plane_y(A, B, planes_y[0]),
            "top": self.intersection_with_plane_y(A, B, planes_y[1])
        }

        #delete points with "None"
        valid_intersections = {}
        for plane, point in intersections.items():
            if point is not None:
                valid_intersections[plane] = point

        if not valid_intersections:
            return None, None

        #get closest border
        closest_intersection = min(valid_intersections, key=lambda k: np.linalg.norm(valid_intersections[k] - A))

        return closest_intersection, valid_intersections[closest_intersection]