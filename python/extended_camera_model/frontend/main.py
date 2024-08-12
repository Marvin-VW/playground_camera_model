import sys
import numpy as np
import cv2 as cv
from math import cos, sin, pi
import time
import os
from PySide6.QtCore import QUrl, QObject, Signal, Slot, QTimer
from PySide6.QtGui import QGuiApplication, QImage, QPixmap
from PySide6.QtQml import QQmlApplicationEngine

from playground_camera_model.camera_model import CameraModel
from playground_camera_model.matrix_functions import Matrix_Functions
from playground_camera_model.window import Window
from playground_camera_model.cube import Cube
from playground_camera_model.structure import Structure_Generator
from playground_camera_model.color import Color
from playground_camera_model.polygons import Polygon
from playground_camera_model.pyramide import Pyramide
from playground_camera_model.cone import Cone


class Engine(QObject):

    # Signal to send the QImage to QML
    imageChanged = Signal(QImage)

    def __init__(self):
        super().__init__()

        self.camera_model = CameraModel(0.00452, 0.00254, 0.004, 1280, 720, 1280/2, 720/2)
        self.window = Window()
    
        self.start_time = time.time()
        self.frame_count = 0

        self.V_T_C = None
        self.C_T_V = None
        self.V_T_Cube = None
        self.V_T_Polygon = None
        self.V_T_Pyramide = None
        self.V_T_Cone = None

        self.cube_list = []
        self.polygon_list = []
        self.pyramide_list = []
        self.cone_list = []

        self.timer = QTimer()
        self.timer.timeout.connect(self.main)
        self.timer.start(1000 // 30)  # Run at 30 FPS

    def setImage(self, array):
        # Convert the NumPy array to QImage
        height, width, channel = array.shape
        bytes_per_line = channel * width
        q_img = QImage(array.data, width, height, bytes_per_line, QImage.Format_RGB888)
        self.imageChanged.emit(q_img)

    @staticmethod  
    def create_point(x: float, y: float, z: float) -> np.array:
        return np.array([
            [x],
            [y],
            [z],
            [1]
        ])

    def fps_setter(self):
        self.frame_count += 1
        elapsed_time = time.time() - self.start_time
        fps = self.frame_count / elapsed_time
        cv.putText(self.camera_model.camera_image, f"FPS: {fps:.0f}", (10, 30), cv.FONT_HERSHEY_PLAIN, 1.2, (0, 255, 0), 1)

    def main(self):
        self.W_T_V = Matrix_Functions.create_homogeneous_transformation_matrix(0, 0, 0, 0, 0, 0, 0)
        self.V_T_C = Matrix_Functions.create_homogeneous_transformation_matrix(0, 0, 0, 0, 0, 0, 0)
        self.C_T_V = np.linalg.inv(self.V_T_C)
        self.V_T_Cube = Matrix_Functions.create_homogeneous_transformation_matrix(2, 0, 1, 0, 0, 0, 0)
        self.V_T_Polygon = Matrix_Functions.create_homogeneous_transformation_matrix(2, 0, 1, 0, 0, 0, 0)
        self.V_T_Pyramide = Matrix_Functions.create_homogeneous_transformation_matrix(2, 0, 1, 0, 0, 0, 0)
        self.V_T_Cone = Matrix_Functions.create_homogeneous_transformation_matrix(2, 0, 1, 0, 0, 0, 0)

        self.cube_list = []
        self.polygon_list = []
        self.pyramide_list = []
        self.cone_list = []

        self.cube_list.extend(Structure_Generator.ground(width=10, height=5, depth=1, size=1, start_x=5, start_y=0, start_z=0))
        cub1 = Cube(size=1, pos_x=0, pos_y=9, pos_z=0)
        self.cube_list.append(cub1)
        pol1 = Polygon(size=1, pos_x=0, pos_y=6, pos_z=0)
        self.polygon_list.append(pol1)
        con1 = Cone(radius=1,height=2,pos_x=0,pos_y=3,pos_z=-1)
        self.cone_list.append(con1)
        pyr1 = Pyramide(size=1, pos_x=0, pos_y=12, pos_z=0)
        self.pyramide_list.append(pyr1)
        self.polygon_list.extend(Structure_Generator.ramp_from_polygons(size=1, start_x= 0, start_y=0, start_z=0))

        self.V_T_C, self.C_T_V, self.V_T_Cube, self.V_T_Polygon, self.V_T_Pyramide, self.V_T_Cone = Matrix_Functions.homogeneous_transformation(self.window)
        self.camera_model.reset_camera_image()

        for cube in self.cube_list:
            cube_points = cube.cube_drawer(self.C_T_V, self.V_T_Cube)
            self.camera_model.draw_all_cube_points(cube_points)
            self.camera_model.fill_cube_faces(cube_points, Color.GRAY)
            self.camera_model.draw_cube_lines(cube_points)

        for polygon in self.polygon_list:
            polygon_points = polygon.polygon_drawer(self.C_T_V, self.V_T_Polygon)
            self.camera_model.draw_all_polygon_points(polygon_points)
            self.camera_model.fill_polygon_faces(polygon_points, Color.BEIGE)
            self.camera_model.draw_polygon_lines(polygon_points)

        for pyramide in self.pyramide_list:
            pyramide_points = pyramide.pyramide_drawer(self.C_T_V, self.V_T_Pyramide)
            self.camera_model.draw_all_pyramide_points(pyramide_points)
            self.camera_model.fill_pyramide_faces(pyramide_points, Color.BEIGE)
            self.camera_model.draw_pyramide_lines(pyramide_points)

        for cone in self.cone_list:
            cone_points = cone.cone_drawer(self.C_T_V, self.V_T_Cone)
            self.camera_model.draw_all_cone_points(cone_points)
            self.camera_model.fill_cone_faces(cone_points, Color.BEIGE)
            self.camera_model.draw_cone_lines(cone_points)

        # Convert the OpenCV image to a format suitable for QImage
        image = cv.cvtColor(self.camera_model.camera_image, cv.COLOR_BGR2RGB)
        
        # Set the image using the new setImage method
        self.setImage(image)


if __name__ == '__main__':
    app = QGuiApplication(sys.argv)
    engine = QQmlApplicationEngine()

    engine_instance = Engine()
    engine.rootContext().setContextProperty("engineInstance", engine_instance)

    qml_file_path = os.path.join(os.path.dirname(__file__), "frontend", "main.qml")
    engine.load(QUrl.fromLocalFile(qml_file_path))

    if not engine.rootObjects():
        sys.exit(-1)

    sys.exit(app.exec())
