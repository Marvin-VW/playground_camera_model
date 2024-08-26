# Copyright (C) 2024 twyleg, Marvin-VW
import numpy as np
import cv2 as cv
from typing import List, Tuple

class CameraModel:
    def __init__(self, sensor_width: float, sensor_height: float, focal_length: float, resolution_x: int, resolution_y: int, u0: int, v0: int):
        self.sensor_width: float = sensor_width
        self.sensor_height: float = sensor_height
        self.focal_length: float = focal_length
        self.resolution_x: int = resolution_x
        self.resolution_y: int = resolution_y
        self.u0: int = u0
        self.v0: int = v0

        self.camera_image = np.zeros((resolution_y, resolution_x, 3), dtype=np.uint8)
        self.reset_camera_image()

        # Create camera transformation matrix I_T_C
        rho_width = sensor_width / resolution_x
        rho_height = sensor_height / resolution_y

        matrix_k = np.array([
            [1/rho_width,   0,              u0],
            [0,             1/rho_height,   v0],
            [0,             0,               1],
        ])

        matrix_c = np.array([
            [focal_length,  0,              0,  0],
            [0,             focal_length,   0,  0],
            [0,             0,              1,  0],
        ])

        self.I_T_C = np.matmul(matrix_k, matrix_c)

    
    @staticmethod
    def transform_normals_to_world_space(normals, V_T_Cube):
        normals_in_world_space = V_T_Cube[:3, :3] @ normals
        return normals_in_world_space
    

    @staticmethod
    def world_transform(triangle, V_T_Cube):
        transformed_triangles = []

        for point in triangle:
                transformed_triangle = V_T_Cube @ point
                transformed_triangles.append(transformed_triangle)

        return transformed_triangles
    
    @staticmethod
    def camera_transform(object, C_T_V):
        transformed_triangles = []

        for point in object:
            transformed_triangle = tuple(C_T_V @ point)
            transformed_triangles.append(transformed_triangle)

        return transformed_triangles
    
    
    def draw_all_cube_points(self, triangles: List) -> None:

        for triangle in triangles:
            for point in triangle.camera_points:
                self.draw_camera_image_point(point)


    def draw_camera_image_point(self, C_point: np.array) -> None:
        I_point = np.matmul(self.I_T_C, C_point)
        u = int(I_point[0] / I_point[2])
        v = int(I_point[1] / I_point[2])
        cv.circle(self.camera_image, (u, v), 5, (255, 0, 0), 2)
    
    def draw_all_cube_lines(self, triangles : List) -> None:

        for triangle in triangles:
            for i in range(3):
                C_point0 = triangle.camera_points[i]
                C_point1 = triangle.camera_points[(i + 1) % 3]
                self.draw_camera_image_line(C_point0, C_point1)


    def draw_camera_image_line(self, C_point0: np.array, C_point1: np.array) -> None:
        I_point0 = np.matmul(self.I_T_C, C_point0)
        I_point1 = np.matmul(self.I_T_C, C_point1)

        u0 = int(I_point0[0] / I_point0[2])
        v0 = int(I_point0[1] / I_point0[2])

        u1 = int(I_point1[0] / I_point1[2])
        v1 = int(I_point1[1] / I_point1[2])

        cv.line(self.camera_image, (u0, v0), (u1, v1), (0, 0, 0), 1)

    def draw_camera_image_arrow(self, C_point0: np.array, C_point1: np.array) -> None:
        try:
            I_point0 = np.matmul(self.I_T_C, C_point0)
            I_point1 = np.matmul(self.I_T_C, C_point1)

            u0 = int(I_point0[0] / I_point0[2])
            v0 = int(I_point0[1] / I_point0[2])

            u1 = int(I_point1[0] / I_point1[2])
            v1 = int(I_point1[1] / I_point1[2])

            cv.arrowedLine(self.camera_image, (u0, v0), (u1, v1), (0, 255, 0), 2)
        except:
            raise ValueError(f"Could draw normal {C_point0}, {C_point1}")


    def fill_cube_faces(self, triangles) -> None:
        for triangle in triangles:
            I_points = []

            for C_point in triangle.camera_points:
                I_point = np.matmul(self.I_T_C, C_point)
                
                u = int(I_point[0] / I_point[2])
                v = int(I_point[1] / I_point[2])

                I_points.append((u, v))
            
            Poly_Points = np.array(I_points, np.int32)
            cv.fillPoly(self.camera_image, [Poly_Points], triangle.color)

    def draw_poly(self, points):

        I_points = []

        for point in points:

            I_point = np.matmul(self.I_T_C, point)
                
            u = int(I_point[0] / I_point[2])
            v = int(I_point[1] / I_point[2])

            I_points.append((u, v))
            
        Poly_Points = np.array(I_points, np.int32)
        hull = cv.convexHull(Poly_Points)
        cv.fillPoly(self.camera_image, [hull], (50,50,50))


    def reset_camera_image(self) -> None:
        self.camera_image.fill(255)


    @staticmethod
    def get_camera_vectors(V_T_C: np.array) -> np.array:
        rotation_matrix = V_T_C[:3, :3]
        forward_vector = -rotation_matrix[:, 2]
        camera_position = V_T_C[:3, 3]
        final_vector_x = forward_vector[0] + camera_position[0]
        final_vector_y = forward_vector[1] + camera_position[1]
        final_vector_z = forward_vector[2] + camera_position[2]
        return (final_vector_x, final_vector_y, final_vector_z)