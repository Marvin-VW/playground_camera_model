// Copyright (C) 2024 Marvin-VW
#include "Engine.h"
#include <cmath>
#include <iostream>

#define DEG_TO_RAD(x) ((x) * (M_PI / 180.0))

float Engine::is_triangle_facing_camera(triangle& tri, cv::Vec3f cam) {

    float dot_product =    tri.normal.at<double>(0) * (tri.centroid.at<double>(0) - cam[0]) +
                           tri.normal.at<double>(1) * (tri.centroid.at<double>(1) - cam[1]) +
                           tri.normal.at<double>(2) * (tri.centroid.at<double>(2) - cam[2]);

    return dot_product;
};


Engine::Engine()
{
    // Initialize the graphics renderer
    RenderSystem* renderer = new RenderSystem();

    //instances of shape, camera and matrix
    Shape* shape = renderer->createCube();
    CameraModel* camera = renderer->createCamera(0.00452, 0.00254, 0.004, 1280, 720, 1280 / 2, 720 / 2);
    HomogenousTransformationMatrix* matrix = renderer->init_matrices();
    ClippingSpace* clipping = renderer->init_clipping();
    Vectors* vec = renderer->init_vector();
    Color* color = renderer->init_color();

    //generate cube mesh
    std::vector<triangle> mesh = shape->generate_mesh();

}


void Engine::setPosition(float x, float y, float z)
{
    shape->set_position(x,y,z,&mesh);
}

//void setScale(float scaleX, float scaleY, float scaleZ);

//void setRotation(float rotX, float rotY, float rotZ);


void Engine::run(int key)
{

    cv::Vec3f camera_vector_world = camera->getCameraVector(camera->V_T_C);

    camera->resetCameraImage();

    renderer->create_matrices();

    std::vector<triangle> visiable_mesh;

    for (auto& tri : mesh) {

        camera->world_transform(&camera->V_T_Cube, &tri);
        camera->camera_transform(&camera->C_T_V, &tri);

        std::tuple<cv::Mat, cv::Mat> result = vec->normal(tri, 0.5f);

        cv::Mat normal_start = std::get<0>(result);
        cv::Mat normal_end = std::get<1>(result);
        cv::Mat normal_start_camera = camera->C_T_V * normal_start;
        cv::Mat normal_end_camera = camera->C_T_V * normal_end;

        //backface culling
        if (is_triangle_facing_camera(tri, camera_vector_world) < 0.0f) {


            if (renderer->window.cubeSystemNormals == 1)
                camera->drawCameraImageArrow(normal_start_camera, normal_end_camera);

            cv::Vec3f light_direction(1.0f, -0.5f, -0.8f);
            cv::Scalar base_color(255, 248, 240);

            tri.ilm = color->intensity(light_direction, tri.normal);
            tri.color = color->adjust_bgr_intensity(base_color, tri.ilm);

            visiable_mesh.push_back(tri);

        }
    }

    //clipping
    std::vector<triangle> clipped_mesh;
    clipped_mesh = clipping->cubeInSpace(&visiable_mesh);

    if (renderer->window.cubeSystemPoints == 1)
    {
        camera->drawAllPoints(&clipped_mesh);
    }

    if (renderer->window.cubeSystemFaces == 1)
    {
            camera->fillCubeFaces(&clipped_mesh);
    }
    camera->drawAllLines(&clipped_mesh);

    renderer->update_fps();
    renderer->renderFrame();

    renderer->update_movement(key);

}

Engine::~Engine()
{
    delete renderer;
    delete shape;
    delete camera;
    delete matrix;
    delete clipping;
    delete vec;
    delete color;
    renderer->shutdown();

}
