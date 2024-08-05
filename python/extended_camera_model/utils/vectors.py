# Copyright (C) 2024 Marvin-VW
import numpy as np
import math
from math import pi

class CalculateNormal:

    @staticmethod    
    def vector(point1, point2):
        return point2 - point1
    
    @staticmethod
    def DEG_TO_RAD(deg: float) -> float:
        return deg*(pi/180.0)
    

    @staticmethod
    def normal(triangle, scale = 0.5):
                
        p1 = triangle[0].flatten()
        p1 = p1[:3]

        p2 = triangle[1].flatten()
        p2 = p2[:3]

        p3 = triangle[2].flatten()
        p3 = p3[:3]

        # vectors
        vec1 = CalculateNormal.vector(p1, p2)
        vec2 = CalculateNormal.vector(p1, p3)
        
        # normal vector
        normal_vector = np.cross(vec1, vec2)
        
        # normalize to unit length
        norm = np.linalg.norm(normal_vector)
        if norm == 0:
            raise ValueError("The points do not form a valid triangle.")
        
        normalized_normal = normal_vector / norm
        
        # mid of triangle
        centroid = (p1 + p2 + p3) / 3

        # scale vector
        scaled_normal = normalized_normal * scale

        #z,x,y
        scaled_normal = (scaled_normal[0], scaled_normal[1], scaled_normal[2])

        normal_start = centroid
        normal_end = centroid + scaled_normal

        # reshape to project struct.
        normal_start = np.vstack([normal_start.reshape(-1, 1), [[1]]])
        normal_end = np.vstack([normal_end.reshape(-1, 1), [[1]]])

        return scaled_normal, normal_start, normal_end
    

    @staticmethod
    def get_camera_vector(window):
        x = (window.get_camera_system_translation_x() - 10000) / 1000.0
        y = (window.get_camera_system_translation_z() - 10000) / 1000.0
        z = (window.get_camera_system_translation_y() - 10000) / 1000.0

        pitch_rad = CalculateNormal.DEG_TO_RAD(window.get_camera_system_rotation_pitch() / 10.0)
        yaw_rad = CalculateNormal.DEG_TO_RAD(window.get_camera_system_rotation_yaw() / 10.0)

        vx = math.cos(yaw_rad) * math.cos(pitch_rad)
        vy = math.sin(yaw_rad) * math.cos(pitch_rad)
        vz = math.sin(pitch_rad)
        
        final_vector_x = vx
        final_vector_y = vy + 1
        final_vector_z = vz

        final_vector = (final_vector_x, final_vector_y, final_vector_z)

        return final_vector