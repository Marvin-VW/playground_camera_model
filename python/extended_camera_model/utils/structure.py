from utils.shape import Triangle4D
import numpy as np

class OBJ_Importer:

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
                        v = OBJ_Importer.create_point(float(parts[1]), float(parts[2]), float(parts[3]))
                        verts.append(v)

                    if line.startswith('f '):
                        parts = line.strip().split()
                        f = [int(parts[1]), int(parts[2]), int(parts[3])]
                        tris.append(Triangle4D(verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1]))

                return tris
            
        except Exception as e:
            print(f"Failed to load {e}")
