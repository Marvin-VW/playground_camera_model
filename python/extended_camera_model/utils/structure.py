from utils.cube import Cube

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