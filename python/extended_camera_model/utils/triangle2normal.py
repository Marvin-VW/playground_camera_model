import numpy as np

class CalculateNormal:

    @staticmethod    
    def vector(point1, point2):
        return point2 - point1

    @staticmethod
    def normal(triangle, scale = 0.5):
        """
        Calculate the normal vector of the triangle using the cross product.
        Returns:
            normal_start (np.ndarray): The starting point of the normal vector.
            normal_end (np.ndarray): The ending point of the normal vector.
        """
                
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
        
        normal_start = centroid
        normal_end = centroid + scaled_normal

        # reshape to project struct.
        normal_start = np.vstack([normal_start.reshape(-1, 1), [[1]]])
        normal_end = np.vstack([normal_end.reshape(-1, 1), [[1]]])
        
        return normal_start, normal_end