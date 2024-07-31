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


        while True:


            self.window.handle_movement()

            self.fps_counter.update()

            self.V_T_C, self.C_T_V, self.V_T_Cube = Matrix_Functions.homogeneous_transformation(self.window)

            self.camera_model.reset_camera_image()

            for object in self.render_list:
                transformed_triangles = self.camera_model.camera_transform(object, self.C_T_V, self.V_T_Cube)

                ndc_points = self.clipping_space.cube_in_space(transformed_triangles)

                self.camera_model.draw_all_cube_points(ndc_points)
                self.camera_model.draw_cube_lines(ndc_points)
                self.camera_model.fill_cube_faces(ndc_points, Color.ROYAL_BLUE)

            self.fps_setter()
            self.window.window_show(self.camera_model)



engine = Engine()
engine.main()