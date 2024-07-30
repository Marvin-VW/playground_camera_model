import numpy as np

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
        self.render_faces = [0] * 12

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
        
        self.triangles = [
            # Top face
            (self.Cube_cubeP4, self.Cube_cubeP5, self.Cube_cubeP6),
            (self.Cube_cubeP4, self.Cube_cubeP6, self.Cube_cubeP7),
            # Bottom face
            (self.Cube_cubeP0, self.Cube_cubeP1, self.Cube_cubeP2),
            (self.Cube_cubeP0, self.Cube_cubeP2, self.Cube_cubeP3),
            # Left face
            (self.Cube_cubeP0, self.Cube_cubeP3, self.Cube_cubeP7),
            (self.Cube_cubeP0, self.Cube_cubeP7, self.Cube_cubeP4),
            # Right face
            (self.Cube_cubeP1, self.Cube_cubeP5, self.Cube_cubeP6),
            (self.Cube_cubeP1, self.Cube_cubeP6, self.Cube_cubeP2),
            # Front face
            (self.Cube_cubeP0, self.Cube_cubeP4, self.Cube_cubeP5),
            (self.Cube_cubeP0, self.Cube_cubeP5, self.Cube_cubeP1),
            # Back face
            (self.Cube_cubeP3, self.Cube_cubeP2, self.Cube_cubeP6),
            (self.Cube_cubeP3, self.Cube_cubeP6, self.Cube_cubeP7),
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

        for i, triangle in enumerate(self.triangles):
            self.triangles[i] = tuple(translation_matrix @ vertex for vertex in triangle)

    def set_face_points(self, *face_points):
        self.face_points = list(face_points)

    def get_face_points(self):
        return self.face_points

