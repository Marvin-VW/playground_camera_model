# Copyright (C) 2024 Marvin-VW
import numpy as np

class Triangle4D:
    def __init__(self, p1, p2, p3):
        self.points = [p1, p2, p3]
        self.world_points = None
        self.camera_points = None
        self.normal = None
        self.ilm = None
        self.color = None
        self.centroids = None

class Cube:
    
    @staticmethod  
    def create_point(x: float, y: float, z: float) -> np.array:
        return np.array([
            [x],
            [y],
            [z],
            [1]
        ])

    def __init__(self, size, pos_x, pos_y, pos_z):
        self.generate_vertices(size)
        self.set_position(pos_x, pos_y, pos_z)

    def generate_vertices(self, size):
        self.Cube_cubeP0 = self.create_point(-size, size, -size)
        self.Cube_cubeP1 = self.create_point(-size, -size, -size)
        self.Cube_cubeP2 = self.create_point(size, -size, -size)
        self.Cube_cubeP3 = self.create_point(size, size, -size)
        self.Cube_cubeP4 = self.create_point(-size, size, size)
        self.Cube_cubeP5 = self.create_point(-size, -size, size)
        self.Cube_cubeP6 = self.create_point(size, -size, size)
        self.Cube_cubeP7 = self.create_point(size, size, size)

        self.cube_points = [
            self.Cube_cubeP0, self.Cube_cubeP1,
            self.Cube_cubeP2, self.Cube_cubeP3,
            self.Cube_cubeP4, self.Cube_cubeP5,
            self.Cube_cubeP6, self.Cube_cubeP7
        ]

        triangle_top_1 = Triangle4D(self.Cube_cubeP4, self.Cube_cubeP5, self.Cube_cubeP6)
        triangle_top_2 = Triangle4D(self.Cube_cubeP4, self.Cube_cubeP6, self.Cube_cubeP7)

        triangle_bottom_1 = Triangle4D(self.Cube_cubeP1, self.Cube_cubeP0, self.Cube_cubeP2)
        triangle_bottom_2 = Triangle4D(self.Cube_cubeP2, self.Cube_cubeP0, self.Cube_cubeP3)

        triangle_left_1 = Triangle4D(self.Cube_cubeP3, self.Cube_cubeP0, self.Cube_cubeP7)
        triangle_left_2 = Triangle4D(self.Cube_cubeP7, self.Cube_cubeP0, self.Cube_cubeP4)

        triangle_right_1 = Triangle4D(self.Cube_cubeP5, self.Cube_cubeP1, self.Cube_cubeP6)
        triangle_right_2 = Triangle4D(self.Cube_cubeP6, self.Cube_cubeP1, self.Cube_cubeP2)

        triangle_front_1 = Triangle4D(self.Cube_cubeP4, self.Cube_cubeP0, self.Cube_cubeP5)
        triangle_front_2 = Triangle4D(self.Cube_cubeP5, self.Cube_cubeP0, self.Cube_cubeP1)

        triangle_back_1 = Triangle4D(self.Cube_cubeP2, self.Cube_cubeP3, self.Cube_cubeP6)
        triangle_back_2 = Triangle4D(self.Cube_cubeP6, self.Cube_cubeP3, self.Cube_cubeP7)

        self.mesh = [
            triangle_top_1, triangle_top_2,
            triangle_bottom_1, triangle_bottom_2,
            triangle_left_1, triangle_left_2,
            triangle_right_1, triangle_right_2,
            triangle_front_1, triangle_front_2,
            triangle_back_1, triangle_back_2
        ]

    def set_position(self, pos_x, pos_y, pos_z):
        translation_matrix = np.array([
            [1, 0, 0, pos_x],
            [0, 1, 0, pos_y],
            [0, 0, 1, pos_z],
            [0, 0, 0, 1]
        ])

        for pos, point in enumerate(self.cube_points):
            translated_vec = translation_matrix @ point
            self.cube_points[pos] = translated_vec

        for triangle in self.mesh:
            triangle.points = [translation_matrix @ vertex for vertex in triangle.points]




class Rectangle:
    
    @staticmethod  
    def create_point(x: float, y: float, z: float) -> np.array:
        return np.array([
            [x],
            [y],
            [z],
            [1]
        ])

    def __init__(self, size_x, size_y, size_z, pos_x, pos_y, pos_z):
        self.generate_vertices(size_x, size_y, size_z)
        self.set_position(pos_x, pos_y, pos_z)

    def generate_vertices(self, size_x, size_y, size_z):
        self.Cube_cubeP0 = self.create_point(-size_x, size_y, -size_z)
        self.Cube_cubeP1 = self.create_point(-size_x, -size_y, -size_z)
        self.Cube_cubeP2 = self.create_point(size_x, -size_y, -size_z)
        self.Cube_cubeP3 = self.create_point(size_x, size_y, -size_z)
        self.Cube_cubeP4 = self.create_point(-size_x, size_y, size_z)
        self.Cube_cubeP5 = self.create_point(-size_x, -size_y, size_z)
        self.Cube_cubeP6 = self.create_point(size_x, -size_y, size_z)
        self.Cube_cubeP7 = self.create_point(size_x, size_y, size_z)

        self.cube_points = [
            self.Cube_cubeP0, self.Cube_cubeP1,
            self.Cube_cubeP2, self.Cube_cubeP3,
            self.Cube_cubeP4, self.Cube_cubeP5,
            self.Cube_cubeP6, self.Cube_cubeP7
        ]

        triangle_top_1 = Triangle4D(self.Cube_cubeP4, self.Cube_cubeP5, self.Cube_cubeP6)
        triangle_top_2 = Triangle4D(self.Cube_cubeP4, self.Cube_cubeP6, self.Cube_cubeP7)

        triangle_bottom_1 = Triangle4D(self.Cube_cubeP1, self.Cube_cubeP0, self.Cube_cubeP2)
        triangle_bottom_2 = Triangle4D(self.Cube_cubeP2, self.Cube_cubeP0, self.Cube_cubeP3)

        triangle_left_1 = Triangle4D(self.Cube_cubeP3, self.Cube_cubeP0, self.Cube_cubeP7)
        triangle_left_2 = Triangle4D(self.Cube_cubeP7, self.Cube_cubeP0, self.Cube_cubeP4)

        triangle_right_1 = Triangle4D(self.Cube_cubeP5, self.Cube_cubeP1, self.Cube_cubeP6)
        triangle_right_2 = Triangle4D(self.Cube_cubeP6, self.Cube_cubeP1, self.Cube_cubeP2)

        triangle_front_1 = Triangle4D(self.Cube_cubeP4, self.Cube_cubeP0, self.Cube_cubeP5)
        triangle_front_2 = Triangle4D(self.Cube_cubeP5, self.Cube_cubeP0, self.Cube_cubeP1)

        triangle_back_1 = Triangle4D(self.Cube_cubeP2, self.Cube_cubeP3, self.Cube_cubeP6)
        triangle_back_2 = Triangle4D(self.Cube_cubeP6, self.Cube_cubeP3, self.Cube_cubeP7)

        self.mesh = [
            triangle_top_1, triangle_top_2,
            triangle_bottom_1, triangle_bottom_2,
            triangle_left_1, triangle_left_2,
            triangle_right_1, triangle_right_2,
            triangle_front_1, triangle_front_2,
            triangle_back_1, triangle_back_2
        ]

    def set_position(self, pos_x, pos_y, pos_z):
        translation_matrix = np.array([
            [1, 0, 0, pos_x],
            [0, 1, 0, pos_y],
            [0, 0, 1, pos_z],
            [0, 0, 0, 1]
        ])

        for pos, point in enumerate(self.cube_points):
            translated_vec = translation_matrix @ point
            self.cube_points[pos] = translated_vec

        for triangle in self.mesh:
            triangle.points = [translation_matrix @ vertex for vertex in triangle.points]