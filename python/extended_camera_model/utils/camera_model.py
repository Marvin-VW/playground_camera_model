import numpy as np
import cv2 as cv
from typing import List, Tuple

from utils.cube import Cube


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

    
    def camera_transform(self, object, C_T_V, V_T_Cube) -> List:
        transformed_triangles = []

        if isinstance(object, Cube):
            for triangle_tuple in object.triangles:
                transformed_triangle = tuple(C_T_V @ V_T_Cube @ vertex for vertex in triangle_tuple)
                transformed_triangles.append(transformed_triangle)
        else:
            for triangle_tuple in object:
                transformed_triangle = tuple(C_T_V @ V_T_Cube @ vertex for vertex in triangle_tuple)
                transformed_triangles.append(transformed_triangle)

        return transformed_triangles

    def draw_camera_image_point(self, C_point: np.array) -> None:
        I_point = np.matmul(self.I_T_C, C_point)
        u = int(I_point[0] / I_point[2])
        v = int(I_point[1] / I_point[2])
        cv.circle(self.camera_image, (u, v), 5, (255, 0, 0), 2)

    def draw_all_cube_points(self, cube_points) -> None:

        for tuple in cube_points:
            for point in tuple:
                self.draw_camera_image_point(point)


    def draw_camera_image_line(self, C_point0: np.array, C_point1: np.array) -> None:
        I_point0 = np.matmul(self.I_T_C, C_point0)
        I_point1 = np.matmul(self.I_T_C, C_point1)

        u0 = int(I_point0[0] / I_point0[2])
        v0 = int(I_point0[1] / I_point0[2])

        u1 = int(I_point1[0] / I_point1[2])
        v1 = int(I_point1[1] / I_point1[2])

        cv.line(self.camera_image, (u0, v0), (u1, v1), (0, 0, 0), 1)

    def draw_camera_image_arrow(self, C_point0: np.array, C_point1: np.array) -> None:
        I_point0 = np.matmul(self.I_T_C, C_point0)
        I_point1 = np.matmul(self.I_T_C, C_point1)

        u0 = int(I_point0[0] / I_point0[2])
        v0 = int(I_point0[1] / I_point0[2])

        u1 = int(I_point1[0] / I_point1[2])
        v1 = int(I_point1[1] / I_point1[2])

        cv.arrowedLine(self.camera_image, (u0, v0), (u1, v1), (0, 255, 0), 2)

    def draw_cube_lines(self, triangles) -> None:

        for triangle in triangles:
            for i in range(3):
                C_point0 = triangle[i]
                C_point1 = triangle[(i + 1) % 3]
                self.draw_camera_image_line(C_point0, C_point1)


    def fill_cube_faces(self, triangles, cube) -> None:
        for pos, triangle in enumerate(triangles):
            I_points = []

            for C_point in triangle:
                I_point = np.matmul(self.I_T_C, C_point)
                
                u = int(I_point[0] / I_point[2])
                v = int(I_point[1] / I_point[2])

                I_points.append((u, v))
            
            Poly_Points = np.array(I_points, np.int32)
            color = cube.get_color_face(pos)
            cv.fillPoly(self.camera_image, [Poly_Points], color)

    def reset_camera_image(self) -> None:
        self.camera_image.fill(255)

    