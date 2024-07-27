import numpy as np

class RenderFaces:

    @staticmethod
    def generate_center_points(cube_points) -> None:
        # Define the vertex indices for each face of the cube
        face_vertex_indices = [
            [0, 1, 2, 3],  # Top face
            [4, 5, 6, 7],  # Bottom face
            [0, 1, 5, 4],  # Front face
            [2, 3, 7, 6],  # Back face
            [0, 3, 7, 4],  # Left face
            [1, 2, 6, 5]   # Right face
        ]

        face_points_center = []

        for face_indices in face_vertex_indices:
            face_points = [cube_points[i] for i in face_indices]

            # Calculate midpoints and center point using NumPy operations
            mid_P0_P2 = np.add(face_points[0], face_points[2])/2
            mid_P1_P3 = np.add(face_points[1], face_points[3])/2
            cen_point = np.add(mid_P0_P2, mid_P1_P3)/2

            face_points_center.append(cen_point)


        return face_points_center