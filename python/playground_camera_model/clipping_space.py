import numpy as np

class Clipping_Space:
    def __init__(self) -> None:
        
        # Create a projection matrix
        fov = np.deg2rad(58)  # Field of view in radians
        aspect_ratio = 1.77  # Screen width / height
        near = 1.0
        far = 100.0
        self.projection_matrix = self.create_perspective_projection_matrix(fov, aspect_ratio, near, far)

    def create_perspective_projection_matrix(self, fov, aspect_ratio, near, far):
        
        f = 1.0 / np.tan(fov / 2)
        nf = 1 / (near - far)
        
        return np.array([
            [f / aspect_ratio, 0, 0, 0],
            [0, f, 0, 0],
            [0, 0, (far + near) * nf, (2 * far * near) * nf],
            [0, 0, -1, 0]
        ])

    def cube_in_space(self, cube_points: list):
        points = []
        for point in cube_points:
            clip_space_point = np.matmul(self.projection_matrix, point)
            # Perform perspective division
            ndc_point = clip_space_point / clip_space_point[3]
            points.append(ndc_point)
            print(ndc_point)
        return points