# Copyright (C) 2024 twyleg, Marvin-VW
import numpy as np
from math import cos, sin, pi


class Matrix_Functions:

    @staticmethod
    def DEG_TO_RAD(deg: float) -> float:
        return deg*(pi/180.0)
    

    def create_homogeneous_transformation_matrix(translation_x: float, translation_y: float, translation_z: float,
                                                rotation_roll: float, rotation_pitch: float, rotation_yaw: float, scale: int) -> np.array:

        rotation_matrix_roll = np.array([
            [1,	                    0,					0,					0],
            [0,	                    cos(rotation_roll),	-sin(rotation_roll),0],
            [0,	                    sin(rotation_roll),	cos(rotation_roll),	0],
            [0,	                    0,					0,					1]
        ])

        rotation_matrix_pitch = np.array([
            [cos(rotation_pitch),	0,	                sin(rotation_pitch),0],
            [0,					    1,	                0,					0],
            [-sin(rotation_pitch),  0,	                cos(rotation_pitch),0],
            [0,					    0,	                0,					1]
        ])

        rotation_matrix_yaw = np.array([
            [cos(rotation_yaw),	    -sin(rotation_yaw),	0,	                0],
            [sin(rotation_yaw),     cos(rotation_yaw),	0,	                0],
            [0,					    0,					1,	                0],
            [0,					    0,					0,	                1]
        ])

        translation_matrix = np.array([
            [1,	0,	0,  translation_x],
            [0,	1,	0,	translation_y],
            [0,	0,	1,	translation_z],
            [0,	0,	0,	1]
        ])

        if scale == 0:
            scale = 1

        scale_matrix = np.array([
            [scale,	0,	    0,    0],
            [0,	    scale,	0,  	0],
            [0,	    0,	    scale,	0],
            [0,	    0,	    0,	    1]
        ])

        transformation_matrix = np.matmul(translation_matrix,
                                      np.matmul(scale_matrix,
                                                np.matmul(rotation_matrix_yaw,
                                                          np.matmul(rotation_matrix_pitch,
                                                                    rotation_matrix_roll))))
        return transformation_matrix
    
    @classmethod
    def homogeneous_transformation(cls, window):
        V_T_C = cls.create_homogeneous_transformation_matrix(
            (window.get_camera_system_translation_x() - 10000) / 1000.0,
            (window.get_camera_system_translation_y() - 10000) / 1000.0,
            (window.get_camera_system_translation_z() - 10000) / 1000.0,
            cls.DEG_TO_RAD(window.get_camera_system_rotation_roll() / 10.0),
            cls.DEG_TO_RAD(window.get_camera_system_rotation_pitch() / 10.0),
            cls.DEG_TO_RAD(window.get_camera_system_rotation_yaw() / 10.0),
            1
        )


        C_T_V = np.linalg.inv(V_T_C)

        V_T_Cube = cls.create_homogeneous_transformation_matrix(
            (window.get_cube_system_translation_x() - 10000) / 1000.0,
            (window.get_cube_system_translation_y() - 10000) / 1000.0,
            (window.get_cube_system_translation_z() - 10000) / 1000.0,
            cls.DEG_TO_RAD(window.get_cube_system_rotation_roll() / 10.0),
            cls.DEG_TO_RAD(window.get_cube_system_rotation_pitch() / 10.0),
            cls.DEG_TO_RAD(window.get_cube_system_rotation_yaw() / 10.0),
            window.get_cube_system_scale()
        )

        return V_T_C, C_T_V, V_T_Cube