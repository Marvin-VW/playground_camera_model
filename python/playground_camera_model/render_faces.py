import numpy as np

class RenderFaces:

    @staticmethod
    def generate_center_points(cube, cube_points) -> None:
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
        face_points_front = []

        for pos, face_indices in enumerate(face_vertex_indices):
            face_points = [cube_points[i] for i in face_indices]

            # Calculate midpoints and center point using NumPy operations
            mid_P0_P2 = np.add(face_points[0], face_points[2])/2
            mid_P1_P3 = np.add(face_points[1], face_points[3])/2
            cen_point = np.add(mid_P0_P2, mid_P1_P3)/2

            if pos == 2: #equals front face
                face_points_front.append(cen_point[2])

            face_points_center.append(cen_point)

        cube.set_face_points(face_points_center)
        return face_points_front


    @staticmethod
    def set_render_order(cube_list, face_points_front):
        # Combine cube_list and face_points_front into tuples
        cubes_with_front_points = list(zip(cube_list, face_points_front))
        
        # Sort based on face_points_front in descending order
        sorted_cubes = sorted(cubes_with_front_points, key=lambda x: x[1], reverse=True)
        
        # Extract sorted cube_list from sorted_cubes
        sorted_cube_list = [cube for cube, _ in sorted_cubes]
        
        # Return the sorted cube_list
        return sorted_cube_list