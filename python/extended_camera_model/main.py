# Copyright (C) 2024 twyleg, Marvin-VW
import numpy as np
import cv2 as cv

from utils.camera_model import CameraModel
from utils.matrix_functions import Matrix_Functions
from utils.window import Window
from utils.shape import Cube
from utils.structure import Structure_Generator
from utils.color import Color
from utils.clipping_space import Clipping_Space
from utils.fps_counter import FpsCounter
from utils.vectors import CalculateNormal

class Engine:

    def __init__(self):

        self.camera_model = CameraModel(0.00452, 0.00254, 0.004, 1280, 720, 1280/2, 720/2)
        self.window = Window()
        self.clipping_space = Clipping_Space()
        self.fps_counter = FpsCounter(60)
        self.V_T_C = Matrix_Functions.create_homogeneous_transformation_matrix(0, 0, 0, 0, 0, 0, 0)
        self.C_T_V = np.linalg.inv(self.V_T_C)
        self.V_T_Cube = Matrix_Functions.create_homogeneous_transformation_matrix(2, 0, 1, 0, 0, 0, 0)
        self.render_list = []

    def fps_setter(self):
        fps = self.fps_counter.get_fps_filtered()
        cv.putText(self.camera_model.camera_image, f"FPS: {fps:.0f}", (10, 30), cv.FONT_HERSHEY_PLAIN, 1.2, (0, 255, 0), 1)

    def is_triangle_facing_camera(self, normal, tri, cam):
        dot_product = ( normal[0] * (tri[0] - cam[0]) +
                        normal[1] * (tri[1] - cam[1]) +
                        normal[2] * (tri[2] - cam[2])   )
        return dot_product < 0.0

    def main(self):

        # file_path = r"utils\resources\VideoShip.obj"
        # struc = Structure_Generator.load_from_obj(file_path)
        # self.render_list.extend(struc)

        #self.render_list.extend(Structure_Generator.ground(width=5, height=1, depth=5, size=1, start_x=0, start_y=0, start_z=0))
        cube = Cube(size=1, pos_x=0, pos_y=0, pos_z=0)
        self.render_list.extend(cube.mesh)

        while True:

            self.window.handle_movement()
            self.fps_counter.update()
            self.camera_model.reset_camera_image()

            self.V_T_C, self.C_T_V, self.V_T_Cube = Matrix_Functions.homogeneous_transformation(self.window)
            camera_vector_world = CalculateNormal.get_camera_vector(self.window)

            visiable_triangles = []

            for triangle in self.render_list:

                triangle.world_points = self.camera_model.world_transform(triangle.points, self.V_T_Cube)
                triangle.normal, normal_start, normal_end = CalculateNormal.normal(triangle.world_points)
                transformed_normals = self.camera_model.camera_transform([normal_start, normal_end], self.C_T_V)

                if self.window.show_normals:
                    self.camera_model.draw_camera_image_arrow(transformed_normals[0], transformed_normals[1])

                if self.is_triangle_facing_camera(triangle.normal, triangle.world_points[0].flatten()[:3], camera_vector_world):

                    light_direction = camera_vector_world
                    triangle.ilm = Color.intensity(light_direction, triangle.normal)
                    triangle.color = Color.adjust_bgr_intensity(Color.ALICE_BLUE, triangle.ilm)

                    triangle.camera_points = self.camera_model.world_transform(triangle.world_points, self.C_T_V)
                    visiable_triangles.append(triangle)

            #sort triangles by z
            sorted_list = sorted(visiable_triangles, key=lambda triangle: triangle.camera_points[0][2], reverse=True)

            clipped_triangles = []
            clipped_triangles.extend(self.clipping_space.cube_in_space(sorted_list))

            self.camera_model.draw_all_cube_lines(clipped_triangles)
            if self.window.show_points:
                self.camera_model.draw_all_cube_points(clipped_triangles)
            if self.window.show_planes:
                self.camera_model.fill_cube_faces(clipped_triangles)

            self.fps_setter()
            self.window.window_show(self.camera_model)

engine = Engine()
engine.main()