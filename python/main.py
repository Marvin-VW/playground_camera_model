import numpy as np
import cv2 as cv
from math import cos, sin, pi
import time

from playground_camera_model.camera_model import CameraModel
from playground_camera_model.matrix_functions import Matrix_Functions
from playground_camera_model.window import Window
from playground_camera_model.cube import Cube
from playground_camera_model.structure import Structure_Generator
from playground_camera_model.color import Color
from playground_camera_model.render_faces import RenderFaces
from playground_camera_model.clipping_space import Clipping_Space


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

        self.cube_list = []

    def fps_setter(self):
        #Calculate FPS
        self.frame_count += 1
        elapsed_time = time.time() - self.start_time
        fps = self.frame_count / elapsed_time
        cv.putText(self.camera_model.camera_image, f"FPS: {fps:.0f}", (10, 30), cv.FONT_HERSHEY_PLAIN, 1.2, (0, 255, 0), 1)

    def main(self):

        self.cube_list = []

        #self.cube_list.extend(Structure_Generator.ground(width=10, height=1, depth=10, size=1, start_x=0, start_y=0, start_z=0))
        cub1 = Cube(size=1, pos_x=0, pos_y=0, pos_z=0)
        self.cube_list.append(cub1)

        while True:

            self.V_T_C, self.C_T_V, self.V_T_Cube = Matrix_Functions.homogeneous_transformation(self.window)
            self.camera_model.reset_camera_image()

            for cube in self.cube_list:

                cube_points = cube.cube_drawer(self.C_T_V, self.V_T_Cube)

                clippend_points = self.clipping_space.cube_in_space(cube_points)

                #RenderFaces.generate_center_points(cube, cube_points)
                
                #self.camera_model.draw_camera_image_point(clippend_points)
                self.camera_model.draw_all_cube_points(clippend_points)
                self.camera_model.draw_cube_lines(clippend_points)
                #self.camera_model.fill_cube_faces(cube_points, Color.BURLYWOOD)

            #self.fps_setter()
            self.window.window_show(self.camera_model)


engine = Engine()
engine.main()