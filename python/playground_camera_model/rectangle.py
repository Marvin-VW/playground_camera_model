import numpy as np

class Rectangle:
    
    @staticmethod  
    def create_point(x: float, y: float, z: float) -> np.array:
        return np.array([
            [x],
            [y],
            [z],
            [1]
        ])

    def __init__(self, Width, Height, Depth, pos_x, pos_y, pos_z):

        self.generate_vertices(Width, Height, Depth)
        self.set_position(pos_x, pos_y, pos_z)
        self.render_faces : list = [0,0,0,0,0,0] #top, bottom, front, back, left, right

    def generate_vertices(self, Width, Height, Depth):
        #Creates vertices of the rectangle
        # Vertices are defined relative to the center of the rectangle
        self.Rec_rectangleP0 = self.create_point(-Width, Height, -Depth)
        self.Rec_rectangleP1 = self.create_point(-Width, -Height, -Depth)
        self.Rec_rectangleP2 = self.create_point(Width, -Height, -Depth)
        self.Rec_rectangleP3 = self.create_point(Width, Height, -Depth)
        self.Rec_rectangleP4 = self.create_point(-Width, Height, Depth)
        self.Rec_rectangleP5 = self.create_point(-Width, -Height, Depth)
        self.Rec_rectangleP6 = self.create_point(Width, -Height, Depth)
        self.Rec_rectangleP7 = self.create_point(Width, Height, Depth)

        self.rectangle_points = [
            self.Rec_rectangleP0, self.Rec_rectangleP1,
            self.Rec_rectangleP2, self.Rec_rectangleP3,
            self.Rec_rectangleP4, self.Rec_rectangleP5,
            self.Rec_rectangleP6, self.Rec_rectangleP7
        ]

    def set_position(self, pos_x, pos_y, pos_z):
        #Translate the rectangle to a new position
        translation_matrix = np.array([
            [1, 0, 0, pos_x],
            [0, 1, 0, pos_y],
            [0, 0, 1, pos_z],
            [0, 0, 0, 1]
        ])

        for pos, point in enumerate(self.rectangle_points):
            translated_vec = translation_matrix @ point
            self.rectangle_points[pos] = translated_vec

    def rectangle_drawer(self, C_T_V, V_T_Cube):
        rectangle_points_transform = []

        for point in self.rectangle_points:
            rectangleP = C_T_V @ V_T_Cube @ point
            rectangle_points_transform.append(rectangleP)

        return rectangle_points_transform
    
    #faces to render
    def set_render_faces(self, top, bottom, front, back, left, right):
        self.render_faces = [top, bottom, front, back, left, right]

    #center points of faces
    def set_face_points(self, Cen_rectangleP0, Cen_rectangleP1, Cen_rectangleP2, Cen_rectangleP3, Cen_rectangleP4, Cen_rectangleP5, Cen_rectangleP6, Cen_rectangleP7):
        self.face_points = [Cen_rectangleP0, Cen_rectangleP1, Cen_rectangleP2, Cen_rectangleP3, Cen_rectangleP4, Cen_rectangleP5, Cen_rectangleP6, Cen_rectangleP7]

    def get_face_points(self):
        return self.face_points
    
    def get_points(self):
        return self.rectangle_points
    
    def get_render_faces(self):
        return self.render_faces
