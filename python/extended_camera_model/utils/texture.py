import numpy as np
import cv2 as cv

class Texture:
    def project_vertex(vertex, view_matrix, projection_matrix):
        vertex_homogeneous = np.append(vertex[:3, 0], 1)
        transformed_vertex = np.dot(view_matrix, vertex_homogeneous)
        projected_vertex = np.dot(projection_matrix, transformed_vertex)
        if projected_vertex[3] != 0:
            projected_vertex /= projected_vertex[3]
        screen_x = int((projected_vertex[0] * 0.5 + 0.5) * 1280)
        screen_y = int((projected_vertex[1] * -0.5 + 0.5) * 720)
        return np.array([screen_x, screen_y])

    def draw_textured_triangle(img, pts, texture, uv_pts):
        h, w, _ = texture.shape
        src_pts = np.float32([[uv[0] * w, uv[1] * h] for uv in uv_pts])
        dst_pts = np.float32(pts)
        M = cv.getAffineTransform(src_pts[:3], dst_pts[:3])
        warped_texture = cv.warpAffine(texture, M, (img.shape[1], img.shape[0]), None, flags=cv.INTER_LINEAR, borderMode=cv.BORDER_REFLECT_101)
        mask = np.zeros(img.shape, dtype=np.uint8)
        cv.fillConvexPoly(mask, np.int32([dst_pts]), (255, 255, 255))
        img[mask == 255] = warped_texture[mask == 255]

    def render_textured_cube(cube, texture, view_matrix, projection_matrix):
        img = np.zeros((720, 1280, 3), dtype=np.uint8)
        img[:] = (255, 255, 255)
        for i, tri in enumerate(cube.triangles):
            pts = [Texture.project_vertex(vertex, view_matrix, projection_matrix) for vertex in tri]
            uv_pts = cube.uv_coords[i]
            Texture.draw_textured_triangle(img, pts, texture, uv_pts)
        return img
