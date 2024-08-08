// Copyright (C) 2024 twyleg
#include "homogeneous_transformation_matrix.h"


namespace playground_camera_model::homogeneous_transformation_matrix {


cv::Mat createHomogeneousTransformationMatrix(
		double translationX,
		double translationY,
		double translationZ,
		double rotationRoll,
		double rotationPitch,
		double rotationYaw){


	double rotationMatrixRollData[4*4] =  {	1,	0,					0,					0,
											0,	cos(rotationRoll),	-sin(rotationRoll),	0,
											0,	sin(rotationRoll),	cos(rotationRoll),	0,
											0,	0,					0,					1};

	double rotationMatrixPitchData[4*4] =  {cos(rotationPitch),	0,	sin(rotationPitch),	0,
											0,					1,	0,					0,
											-sin(rotationPitch),0,	cos(rotationPitch),	0,
											0,					0,	0,					1};

	double rotationMatrixYawData[4*4] =  {	cos(rotationYaw),	-sin(rotationYaw),	0,	0,
											sin(rotationYaw),	cos(rotationYaw),	0,	0,
											0,					0,					1,	0,
											0,					0,					0,	1};

	double translationMatrixData[4*4] =  {	1,	0,	0,	translationX,
											0,	1,	0,	translationY,
											0,	0,	1,	translationZ,
											0,	0,	0,	1};

	cv::Mat rotationMatrixRoll(4,4,CV_64F,rotationMatrixRollData);
	cv::Mat rotationMatrixPitch(4,4,CV_64F,rotationMatrixPitchData);
	cv::Mat rotationMatrixYaw(4,4,CV_64F,rotationMatrixYawData);
	cv::Mat translationMatrix(4,4,CV_64F,translationMatrixData);

	return (translationMatrix * ((rotationMatrixYaw*rotationMatrixPitch)*rotationMatrixRoll));


}

cv::Mat createPoint(double x, double y, double z){

	cv::Mat point(4,1,CV_64F);

	point.at<double>(0) = x;
	point.at<double>(1) = y;
	point.at<double>(2) = z;
	point.at<double>(3) = 1;

	return point;
}

}
