// Copyright (C) 2024 twyleg
#pragma once

#include <cstdint>

#include <opencv2/opencv.hpp>

namespace playground_camera_model {

class CameraModel {

private:

	double sensorWidth;
	double sensorHeight;
	double focalLength;
	uint32_t resolutionX;
	uint32_t resolutionY;
	uint32_t u0;
	uint32_t v0;

	cv::Mat cameraImage;

public:

	cv::Mat I_T_C;


	CameraModel(double sensorWidth,
				double sensorHeight,
				double focalLength,
				uint32_t resolutionX,
				uint32_t resolutionY,
				uint32_t u0,
				uint32_t v0);

	void drawCameraImagePoint(const cv::Mat& point);
	void drawCameraImageLine(const cv::Mat& point0, const cv::Mat& point1);

	void resetCameraImage();
	cv::Mat& getCameraImage();

};

}
