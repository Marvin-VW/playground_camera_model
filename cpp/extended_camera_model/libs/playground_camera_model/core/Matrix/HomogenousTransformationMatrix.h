// Copyright (C) 2024 twyleg
#pragma once

#include <opencv2/opencv.hpp>

class HomogenousTransformationMatrix {

public:
	cv::Mat createHomogeneousTransformationMatrix(
			double translationX,
			double translationY,
			double translationZ,
			double rotationRoll,
			double rotationPitch,
			double rotationYaw,
			double scale);

};