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
            norm = 0.5
        
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

        return scaled_normal, normal_start, normal_end, centroid
    
    @staticmethod
    def get_shadow(triangles, light_vec):

        shadow_points = []
        plane_normal = np.array([0, 0, 1])

        for triangle in triangles:
            for point in triangle.world_points:
                shadow_points.append(CalculateNormal.find_intersection(plane_normal, point[:3].flatten(), light_vec))

        unique_array = list(map(np.array, set(tuple(arr) for arr in shadow_points)))
        shadow_points = []

        for point in unique_array:
            shadow_points.append(np.vstack([point.reshape(-1, 1), [[1]]]))

        return shadow_points

    @staticmethod
    def find_intersection(plane_normal, line_point, line_dir, plane_d=2):

        a, b, c = plane_normal
        x0, y0, z0 = line_point
        vx, vy, vz = line_dir
        
        denominator = a * vx + b * vy + c * vz
        
        if denominator == 0:
            return None
        
        t = -(a * x0 + b * y0 + c * z0 + plane_d) / denominator
        
        intersection_point = np.array([x0 + t * vx, y0 + t * vy, z0 + t * vz])
        
        return intersection_point