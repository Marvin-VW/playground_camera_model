// Copyright (C) 2024 Marvin-VW
#ifndef ENGINE_H
#define ENGINE_H


#include "RenderSystem.h"
#include "Shape.h"
#include "CameraModel.h"
#include "HomogenousTransformationMatrix.h"
#include "ClippingSpace.h"
#include "Vectors.h"
#include "Color.h"
#include "Window.h"
#include <opencv2/core.hpp>
#include <vector>

class Engine {
public:
    Engine();
    ~Engine();

    void setPosition(float x, float y, float z);
    //void setScale(float scaleX, float scaleY, float scaleZ);
    //void setRotation(float rotX, float rotY, float rotZ);

    void run(int key);

private:
    RenderSystem* renderer;
    Shape* shape;
    CameraModel* camera;
    HomogenousTransformationMatrix* matrix;
    ClippingSpace* clipping;
    Vectors* vec;
    Color* color;

    std::vector<triangle> mesh;
    std::vector<triangle> visible_mesh;
    std::vector<triangle> clipped_mesh;

    float is_triangle_facing_camera(triangle& tri, cv::Vec3f cam);
};


#endif