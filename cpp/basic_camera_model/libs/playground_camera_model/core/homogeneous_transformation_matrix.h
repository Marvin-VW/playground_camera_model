// Copyright (C) 2024 twyleg
#pragma once

#include <opencv2/opencv.hpp>

namespace playground_camera_model::homogeneous_transformation_matrix {

cv::Mat createHomogeneousTransformationMatrix(
		double translationX,
		double translationY,
		double translationZ,
		double rotationRoll,
		double rotationPitch,
		double rotationYaw);


cv::Mat createPoint(double x, double y, double z);


}
