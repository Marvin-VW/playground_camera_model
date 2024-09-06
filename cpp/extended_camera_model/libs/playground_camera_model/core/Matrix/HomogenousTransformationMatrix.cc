// Copyright (C) 2024 twyleg
#include "HomogenousTransformationMatrix.h"

cv::Mat HomogenousTransformationMatrix::createHomogeneousTransformationMatrix(
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
