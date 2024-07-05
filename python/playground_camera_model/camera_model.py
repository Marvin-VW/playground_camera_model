import numpy as np
import cv2 as cv
from typing import List


class CameraModel:
    def __init__(self, sensor_width: float, sensor_height: float, focal_length: float, resolution_x: int, resolution_y: int, u0: int, v0: int):
        self.sensor_width: float = sensor_width
        self.sensor_height: float = sensor_height
        self.focal_length: float = focal_length
        self.resolution_x: int = resolution_x
        self.resolution_y: int = resolution_y
        self.u0: int = u0
        self.v0: int = v0

        self.fov_x_deg, self.fov_y_deg = self.calculate_fov(sensor_width, sensor_height, focal_length)

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

    def calculate_fov(self, sensor_width, sensor_height, focal_length):
        # Calculate horizontal FOV (in radians)
        fov_x = 2 * np.arctan(sensor_width / (2 * focal_length))
        # Calculate vertical FOV (in radians)
        fov_y = 2 * np.arctan(sensor_height / (2 * focal_length))
        # Convert FOVs from radians to degrees
        fov_x_deg = np.rad2deg(fov_x)
        fov_y_deg = np.rad2deg(fov_y)
        return fov_x_deg, fov_y_deg


    def draw_camera_image_point(self, C_point: np.array) -> None:
        I_point = np.matmul(self.I_T_C, C_point)
        u = int(I_point[0] / I_point[2])
        v = int(I_point[1] / I_point[2])
        cv.circle(self.camera_image, (u, v), 5, (255, 0, 0), 2)

    def draw_all_cube_points(self, cube_points) -> None:
        for point in cube_points:
            self.draw_camera_image_point(point)


    def draw_camera_image_line(self, C_point0: np.array, C_point1: np.array) -> None:
        I_point0 = np.matmul(self.I_T_C, C_point0)
        I_point1 = np.matmul(self.I_T_C, C_point1)

        u0 = int(I_point0[0] / I_point0[2])
        v0 = int(I_point0[1] / I_point0[2])

        u1 = int(I_point1[0] / I_point1[2])
        v1 = int(I_point1[1] / I_point1[2])

        cv.line(self.camera_image, (u0, v0), (u1, v1), (0, 0, 0), 1)

    def draw_cube_lines(self, cube_points) -> None:
        edge_pairs = [
            (0, 1), (1, 2), (2, 3), (3, 0),  # Bottom face
            (4, 5), (5, 6), (6, 7), (7, 4),  # Top face
            (0, 4), (1, 5), (2, 6), (3, 7)   # Vertical edges
        ]

        for start, end in edge_pairs:
            C_point0 = cube_points[start]
            C_point1 = cube_points[end]
            self.draw_camera_image_line(C_point0, C_point1)


    def fill_cube_faces(self, cube_points: List[np.array], color) -> None:
        # Define the vertex indices for each face of the cube
        face_vertex_indices = [
            [0, 1, 2, 3],  # Bottom face
            [4, 5, 6, 7],  # Top face
            [0, 1, 5, 4],  # Front face
            [2, 3, 7, 6],  # Back face
            [0, 3, 7, 4],  # Left face
            [1, 2, 6, 5]   # Right face
        ]

        for face_indices in face_vertex_indices:
            # Extract the 3D points for the current face
            face_points = [cube_points[i] for i in face_indices]

            I_points = []

            for C_point in face_points:
                I_point = np.matmul(self.I_T_C, C_point)
                
                u0 = int(I_point[0] / I_point[2])
                v0 = int(I_point[1] / I_point[2])

                I_points.append((u0, v0))
            
            Poly_Points = np.array(I_points)

            cv.fillPoly(self.camera_image, [Poly_Points], color)

    def reset_camera_image(self) -> None:
        self.camera_image.fill(255)
