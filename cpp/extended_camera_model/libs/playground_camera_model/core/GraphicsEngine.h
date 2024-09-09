// Copyright (C) 2024 Marvin-VW
#pragma once

#include <cstdint>
#include "Shape.h" 

class Shape;
class CameraModel;
class HomogenousTransformationMatrix;
class FpsCounter;

class GraphicsEngine
{
public:
    GraphicsEngine();

	bool release();
    CameraModel* create_matrices();
    HomogenousTransformationMatrix* init_matrices();
    FpsCounter* update_fps();

    Shape* createCube();
    CameraModel* createCamera(  double sensorWidth,
                                                double sensorHeight,
                                                double focalLength,
                                                uint32_t resolutionX,
                                                uint32_t resolutionY,
                                                uint32_t u0,
                                                uint32_t v0);

    void renderFrame();
    bool isRunning();
    void shutdown();

    ~GraphicsEngine();

private:
    int32_t cameraSystemTranslationX;
    int32_t cameraSystemTranslationY;
    int32_t cameraSystemTranslationZ;
    int32_t cameraSystemRotationRoll;
    int32_t cameraSystemRotationPitch;
    int32_t cameraSystemRotationYaw;

    int32_t cubeSystemTranslationX;
    int32_t cubeSystemTranslationY;
    int32_t cubeSystemTranslationZ;
    int32_t cubeSystemRotationRoll;
    int32_t cubeSystemRotationPitch;
    int32_t cubeSystemRotationYaw;
    int32_t cubeSystemScale;

private:
    friend class Shape;
    friend class HomogenousTransformationMatrix;
    friend class FpsCounter;
private:
    std::vector<triangle> mesh;
private:
    CameraModel* cm;
    HomogenousTransformationMatrix* ht;
private:
    FpsCounter* fc;
};