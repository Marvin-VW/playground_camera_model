from utils.shape import Cube, Triangle4D
import numpy as np

class Structure_Generator:

    @staticmethod
    def ground(width, height, depth, size=1, start_x=0, start_y=0, start_z=0):
        """
        Generate a ground structure of cubes with the specified width, height, and depth.
        """
        cubes = []

        for z in range(height):
            for row in range(width):
                for col in range(depth):
                    pos_x = start_x + col * size*2
                    pos_y = start_y + row * size*2
                    pos_z = start_z + z * size*2
                    cub = Cube(size, pos_x, pos_y, pos_z)
                    cubes.append(cub)

        return cubes
    
    @staticmethod  
    def create_point(x: float, y: float, z: float) -> np.array:
        return np.array([
            [x],
            [y],
            [z],
            [1]
        ])

    @staticmethod
    def load_from_obj(filename):
        try:
            with open(filename, 'r') as f:
                verts = []
                tris = []

                for line in f:
                    if line.startswith('v '):
                        parts = line.strip().split()
                        v = Structure_Generator.create_point(float(parts[1]), float(parts[2]), float(parts[3]))
                        verts.append(v)

                    if line.startswith('f '):
                        parts = line.strip().split()
                        f = [int(parts[1]), int(parts[2]), int(parts[3])]
                        tris.append(Triangle4D(verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1]))

                return tris
            
        except Exception as e:
            print(f"Failed to load {e}")
