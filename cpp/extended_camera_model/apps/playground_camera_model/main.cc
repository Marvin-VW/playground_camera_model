// Copyright (C) 2024 twyleg, Daniel-VW
#include <unistd.h>
#include <iostream>

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#include <playground_camera_model/core/camera_model.h>
#include <playground_camera_model/core/homogeneous_transformation_matrix.h>
#include <playground_camera_model/core/window.h>
#include <playground_camera_model/core/fps_counter.h>

#define DEG_TO_RAD(x) (x*(M_PI/180.0))


namespace HTM = playground_camera_model::homogeneous_transformation_matrix;

int main(int argc, char** argv){

	FpsCounter fps_counter(5);

	int32_t cameraSystemTranslationX = 10000;
	int32_t cameraSystemTranslationY = 10000;
	int32_t cameraSystemTranslationZ = 11000;

	int32_t cameraSystemRotationRoll = 2500;
	int32_t cameraSystemRotationPitch = 0;
	int32_t cameraSystemRotationYaw = 2700;

	int32_t cubeSystemTranslationX = 14000;
	int32_t cubeSystemTranslationY = 10000;
	int32_t cubeSystemTranslationZ = 11000;

	int32_t cubeSystemRotationRoll =  0;
	int32_t cubeSystemRotationPitch = 0;
	int32_t cubeSystemRotationYaw =   0;

	int32_t cubeSystemScale = 1;
	// Init camera model
	playground_camera_model::CameraModel cameraModel(0.00452, 0.00254, 0.004, 1280, 720, 1280/2, 720/2);

	// Init homogeneous transformation matrices
	cv::Mat W_T_V 		= HTM::createHomogeneousTransformationMatrix(0,0,0, 0,0,0,1);
	cv::Mat V_T_C 		= HTM::createHomogeneousTransformationMatrix(0,0,0, 0,0,0,1);
	cv::Mat C_T_V 		= V_T_C.inv();
	cv::Mat V_T_Cube 	= HTM::createHomogeneousTransformationMatrix(2,0,1, 0,0,0,1);


	// Init points of cube
	cv::Mat Cube_cubeP0 = HTM::createPoint(-1, 1,-1);
	cv::Mat Cube_cubeP1 = HTM::createPoint(-1,-1,-1);
	cv::Mat Cube_cubeP2 = HTM::createPoint( 1,-1,-1);
	cv::Mat Cube_cubeP3 = HTM::createPoint( 1, 1,-1);

	cv::Mat Cube_cubeP4 = HTM::createPoint(-1, 1,1);
	cv::Mat Cube_cubeP5 = HTM::createPoint(-1,-1,1);
	cv::Mat Cube_cubeP6 = HTM::createPoint( 1,-1,1);
	cv::Mat Cube_cubeP7 = HTM::createPoint( 1, 1,1);

	cv::Mat V_cubeP0(4, 1, CV_64F);
	cv::Mat V_cubeP1(4, 1, CV_64F);
	cv::Mat V_cubeP2(4, 1, CV_64F);
	cv::Mat V_cubeP3(4, 1, CV_64F);

	cv::Mat V_cubeP4(4, 1, CV_64F);
	cv::Mat V_cubeP5(4, 1, CV_64F);
	cv::Mat V_cubeP6(4, 1, CV_64F);
	cv::Mat V_cubeP7(4, 1, CV_64F);

	cv::Mat C_cubeP0(4, 1, CV_64F);
	cv::Mat C_cubeP1(4, 1, CV_64F);
	cv::Mat C_cubeP2(4, 1, CV_64F);
	cv::Mat C_cubeP3(4, 1, CV_64F);

	cv::Mat C_cubeP4(4, 1, CV_64F);
	cv::Mat C_cubeP5(4, 1, CV_64F);
	cv::Mat C_cubeP6(4, 1, CV_64F);
	cv::Mat C_cubeP7(4, 1, CV_64F);

	// Create gui
	Window::createCameraSettingsWindow(&cameraSystemTranslationX, &cameraSystemTranslationY, &cameraSystemTranslationZ,
                                          &cameraSystemRotationRoll, &cameraSystemRotationPitch, &cameraSystemRotationYaw);
                                          
    Window::createCubeSettingsWindow(&cubeSystemTranslationX, &cubeSystemTranslationY, &cubeSystemTranslationZ,
                                        &cubeSystemRotationRoll, &cubeSystemRotationPitch, &cubeSystemRotationYaw, &cubeSystemScale);


	while(true){

		// Update homogeneous transformation matrices
		V_T_C = HTM::createHomogeneousTransformationMatrix(
				(cameraSystemTranslationX-10000)/1000.0,
				(cameraSystemTranslationY-10000)/1000.0,
				(cameraSystemTranslationZ-10000)/1000.0,
				DEG_TO_RAD(cameraSystemRotationRoll/10.0),
				DEG_TO_RAD(cameraSystemRotationPitch/10.0),
				DEG_TO_RAD(cameraSystemRotationYaw/10.0),
				cubeSystemScale);

		C_T_V = V_T_C.inv();

		V_T_Cube = HTM::createHomogeneousTransformationMatrix(
				(cubeSystemTranslationX-10000)/1000.0,
				(cubeSystemTranslationY-10000)/1000.0,
				(cubeSystemTranslationZ-10000)/1000.0,
				DEG_TO_RAD(cubeSystemRotationRoll/10.0),
				DEG_TO_RAD(cubeSystemRotationPitch/10.0),
				DEG_TO_RAD(cubeSystemRotationYaw/10.0),
				cubeSystemScale);

		// Transform and draw cube points and lines on image
		C_cubeP0 = C_T_V * V_T_Cube * Cube_cubeP0;
		C_cubeP1 = C_T_V * V_T_Cube * Cube_cubeP1;
		C_cubeP2 = C_T_V * V_T_Cube * Cube_cubeP2;
		C_cubeP3 = C_T_V * V_T_Cube * Cube_cubeP3;

		C_cubeP4 = C_T_V * V_T_Cube * Cube_cubeP4;
		C_cubeP5 = C_T_V * V_T_Cube * Cube_cubeP5;
		C_cubeP6 = C_T_V * V_T_Cube * Cube_cubeP6;
		C_cubeP7 = C_T_V * V_T_Cube * Cube_cubeP7;

		cameraModel.resetCameraImage();

		// Points
		cameraModel.drawCameraImagePoint(C_cubeP0);
		cameraModel.drawCameraImagePoint(C_cubeP1);
		cameraModel.drawCameraImagePoint(C_cubeP2);
		cameraModel.drawCameraImagePoint(C_cubeP3);

		cameraModel.drawCameraImagePoint(C_cubeP4);
		cameraModel.drawCameraImagePoint(C_cubeP5);
		cameraModel.drawCameraImagePoint(C_cubeP6);
		cameraModel.drawCameraImagePoint(C_cubeP7);

		// Lines
		cameraModel.drawCameraImageLine(C_cubeP0, C_cubeP1);
		cameraModel.drawCameraImageLine(C_cubeP1, C_cubeP2);
		cameraModel.drawCameraImageLine(C_cubeP2, C_cubeP3);
		cameraModel.drawCameraImageLine(C_cubeP3, C_cubeP0);

		cameraModel.drawCameraImageLine(C_cubeP4, C_cubeP5);
		cameraModel.drawCameraImageLine(C_cubeP5, C_cubeP6);
		cameraModel.drawCameraImageLine(C_cubeP6, C_cubeP7);
		cameraModel.drawCameraImageLine(C_cubeP7, C_cubeP4);

		cameraModel.drawCameraImageLine(C_cubeP0, C_cubeP4);
		cameraModel.drawCameraImageLine(C_cubeP1, C_cubeP5);
		cameraModel.drawCameraImageLine(C_cubeP2, C_cubeP6);
		cameraModel.drawCameraImageLine(C_cubeP3, C_cubeP7);

        cv::Mat image = cameraModel.getCameraImage();
        fps_counter.update();
        std::string fps_text = "FPS: " + std::to_string(static_cast<int>(fps_counter.get_fps_filtered()));
        cv::putText(image, fps_text, cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 2);

        cv::imshow("image window", image);

        // Wait for a short while
        cv::waitKey(10);
	}


}
