import numpy as np
import cv2 as cv
from math import cos, sin, pi
import time

from utils.camera_model import CameraModel
from utils.matrix_functions import Matrix_Functions
from utils.window import Window
from utils.cube import Cube
from utils.structure import Structure_Generator
from utils.color import Color
from utils.render_faces import RenderFaces
from utils.clipping_space import Clipping_Space
from utils.fps_counter import FpsCounter
from utils.vectors import CalculateNormal

class Engine:

    def __init__(self):

        self.camera_model = CameraModel(0.00452, 0.00254, 0.004, 1280, 720, 1280/2, 720/2)
        self.window = Window()
        self.clipping_space = Clipping_Space()
        
        self.start_time = time.time()
        self.frame_count = 0

        self.V_T_C = None
        self.C_T_V = None
        self.V_T_Cube = None

        self.fps_counter = FpsCounter(60)

        self.show_normals = False


    def fps_setter(self):
        fps = self.fps_counter.get_fps_filtered()
        cv.putText(self.camera_model.camera_image, f"FPS: {fps:.0f}", (10, 30), cv.FONT_HERSHEY_PLAIN, 1.2, (0, 255, 0), 1)

    def main(self):

        self.W_T_V = Matrix_Functions.create_homogeneous_transformation_matrix(0, 0, 0, 0, 0, 0, 0)
        self.V_T_C = Matrix_Functions.create_homogeneous_transformation_matrix(0, 0, 0, 0, 0, 0, 0)
        self.C_T_V = np.linalg.inv(self.V_T_C)
        self.V_T_Cube = Matrix_Functions.create_homogeneous_transformation_matrix(2, 0, 1, 0, 0, 0, 0)

        self.render_list = []

        #self.render_list.extend(Structure_Generator.ground(width=5, height=1, depth=5, size=1, start_x=0, start_y=0, start_z=0))
        cub1 = Cube(size=1, pos_x=0, pos_y=0, pos_z=0)
        self.render_list.append(cub1)

        for cube in self.render_list:
            cube.set_dafault_color(Color.ROYAL_BLUE)
            #top, bottom, left, right, front, back
            #cube.set_color_faces(Color.ROYAL_BLUE, Color.ROYAL_BLUE, Color.LIGHT_BLUE, Color.LIGHT_BLUE, Color.ORANGE_RED, Color.ORANGE_RED)

        while True:

            self.fps_counter.update()

            self.V_T_C, self.C_T_V, self.V_T_Cube = Matrix_Functions.homogeneous_transformation(self.window)
            camera_vector = CalculateNormal.get_camera_vector(self.window)


            self.camera_model.reset_camera_image()
            face_points_front = []

            for cube in self.render_list: 
                
                #fit cube points to view
                transformed_triangles = self.camera_model.camera_transform(cube, self.C_T_V, self.V_T_Cube)
                face_points_front.append(RenderFaces.generate_center_points(cube, transformed_triangles)) 

            sorted_cube_list = RenderFaces.set_render_order(self.render_list, face_points_front)
            for object in sorted_cube_list:

                transformed_triangles = self.camera_model.camera_transform(object, self.C_T_V, self.V_T_Cube)
                ndc_points = self.clipping_space.cube_in_space(transformed_triangles)

                #z coords of normals
                dot_product_result = []

                for triangle in ndc_points:

                    scaled_normal, normal_start, normal_end = CalculateNormal.normal(triangle)
                    dot_product_result.append(np.dot(scaled_normal, camera_vector))
                    if self.show_normals:
                        self.camera_model.draw_camera_image_arrow(normal_start, normal_end)

                final_render_list = []

                for pos, res in enumerate(dot_product_result):
                    if res < 0:
                        final_render_list.append(ndc_points[pos])

                self.camera_model.draw_all_cube_points(final_render_list)
                self.camera_model.draw_cube_lines(final_render_list)
                #self.camera_model.fill_cube_faces(ndc_points, object)

            self.fps_setter()
            self.window.window_show(self.camera_model)

engine = Engine()
engine.main()