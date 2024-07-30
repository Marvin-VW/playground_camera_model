import numpy as np

class RenderFaces:

    @staticmethod
    def generate_center_points(cube, transformed_triangles) -> int:

        face_points_center = []

        for i in range (0, len(transformed_triangles), 2):

            #face first point of first triangle
            P0 = transformed_triangles[i][1]
            #second point of second triangle
            P1 = transformed_triangles[i+1][2]

            cen_point = np.add(P0, P1)/2

            face_points_center.append(cen_point)

        cube.set_face_points(face_points_center)
        return face_points_center[4][2] #equals front face


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