// Copyright (C) 2024 Marvin-VW
#include "GraphicsEngine.h"
#include "Shape.h"
#include "iostream"
#include "HomogenousTransformationMatrix.h"
#include "Window.h"
#include "CameraModel.h"
#include "FPSCounter.h"
#include "ClippingSpace.h"
#include "Color.h"
#include "Vectors.h"
#include <exception>

#define DEG_TO_RAD(x) ((x) * (M_PI / 180.0))

GraphicsEngine::GraphicsEngine() {

        // Initialize transformation parameters
    cameraSystemTranslationX = 17223;
    cameraSystemTranslationY = 1445;
    cameraSystemTranslationZ = 16900;

    cameraSystemRotationRoll = 2463;
    cameraSystemRotationPitch = 0;
    cameraSystemRotationYaw = 210;

    cubeSystemTranslationX = 12263;
    cubeSystemTranslationY = 14403;
    cubeSystemTranslationZ = 10000;

    cubeSystemRotationRoll = 0;
    cubeSystemRotationPitch = 0;
    cubeSystemRotationYaw = 0;

    cubeSystemScale = 0;

    // Create GUI
    Window::createCameraSettingsWindow(&cameraSystemTranslationX, &cameraSystemTranslationY, &cameraSystemTranslationZ,
                                       &cameraSystemRotationRoll, &cameraSystemRotationPitch, &cameraSystemRotationYaw);
    Window::createCubeSettingsWindow(&cubeSystemTranslationX, &cubeSystemTranslationY, &cubeSystemTranslationZ,
                                     &cubeSystemRotationRoll, &cubeSystemRotationPitch, &cubeSystemRotationYaw, &cubeSystemScale);

                                
    fc = new FpsCounter(60);

}

HomogenousTransformationMatrix* GraphicsEngine::init_matrices()
{
    ht = new HomogenousTransformationMatrix();

    cm->V_T_C = ht->createHomogeneousTransformationMatrix(0, 0, 0, 0, 0, 0, 0);
    cm->C_T_V = cm->V_T_C.inv();
    cm->V_T_Cube = ht->createHomogeneousTransformationMatrix(2, 0, 1, 0, 0, 0, 0);

    return ht;
}

Color *GraphicsEngine::init_color()
{
    c = new Color();
    return c;
}

ClippingSpace *GraphicsEngine::init_clipping()
{
    cs = new ClippingSpace();
    return cs;
}

Vectors* GraphicsEngine::init_vector()
{
    v = new Vectors();
    return v;
}

FpsCounter* GraphicsEngine::update_fps()
{
    fc->update();
    std::string fps_text = "FPS: " + std::to_string(static_cast<int>(fc->get_fps_filtered()));
    cv::putText(cm->getCameraImage(), fps_text, cv::Point(10, 30), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 2);

    return fc;
}


CameraModel* GraphicsEngine::create_matrices()
{

    // Create camera to world matrix
    cm->V_T_C = ht->createHomogeneousTransformationMatrix(
        (cameraSystemTranslationX - 10000) / 1000.0,
        (cameraSystemTranslationY - 10000) / 1000.0,
        (cameraSystemTranslationZ - 10000) / 1000.0,
        DEG_TO_RAD(cameraSystemRotationRoll / 10.0),
        DEG_TO_RAD(cameraSystemRotationPitch / 10.0),
        DEG_TO_RAD(cameraSystemRotationYaw / 10.0),
        1.0f);

    // Compute inverse (world to camera matrix)
    cm->C_T_V = cm->V_T_C.inv();

    // Create cube to world matrix
    cm->V_T_Cube = ht->createHomogeneousTransformationMatrix(
        (cubeSystemTranslationX - 10000) / 1000.0,
        (cubeSystemTranslationY - 10000) / 1000.0,
        (cubeSystemTranslationZ - 10000) / 1000.0,
        DEG_TO_RAD(cubeSystemRotationRoll / 10.0),
        DEG_TO_RAD(cubeSystemRotationPitch / 10.0),
        DEG_TO_RAD(cubeSystemRotationYaw / 10.0),
        cubeSystemScale);

    return cm;

}


Shape* GraphicsEngine::createCube() {

    Shape* sp = new Shape();

    mesh = sp->generate_mesh();

    return sp;

}

CameraModel* GraphicsEngine::createCamera(  double sensorWidth,
                                            double sensorHeight,
                                            double focalLength,
                                            uint32_t resolutionX,
                                            uint32_t resolutionY,
                                            uint32_t u0,
                                            uint32_t v0) 
{

    cm = new CameraModel(sensorWidth, sensorHeight, focalLength, resolutionX, resolutionY, u0, v0);

    return cm;

}

void GraphicsEngine::renderFrame() {

        cv::imshow("image window", cm->getCameraImage());

}

GraphicsEngine::~GraphicsEngine() {

}

void GraphicsEngine::shutdown() {
    std::cout << "Shutting down Graphics Engine" << std::endl;
    delete ht;
    delete cm;
    delete fc;
}
