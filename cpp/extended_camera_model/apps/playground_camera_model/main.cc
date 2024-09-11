// Copyright (C) 2024 Marvin-VW
#include "GraphicsEngine.h"
#include "Shape.h"
#include "CameraModel.h"
#include "HomogenousTransformationMatrix.h"
#include "ClippingSpace.h"
#include "Vectors.h"
#include "Color.h"
#include <cmath>

#include <iostream>

#define DEG_TO_RAD(x) ((x) * (M_PI / 180.0))


static float is_triangle_facing_camera(triangle& tri, cv::Vec3f cam) {

    float dot_product =    tri.normal.at<double>(0) * (tri.centroid.at<double>(0) - cam[0]) +
                           tri.normal.at<double>(1) * (tri.centroid.at<double>(1) - cam[1]) +
                           tri.normal.at<double>(2) * (tri.centroid.at<double>(2) - cam[2]);

    return dot_product;
};

int main(int argc, char** argv) {
    // Initialize the graphics engine
    GraphicsEngine* engine = new GraphicsEngine();

    //instances of shape, camera and matrix
    Shape* shape = engine->createCube();
    CameraModel* camera = engine->createCamera(0.00452, 0.00254, 0.004, 1280, 720, 1280 / 2, 720 / 2);
    HomogenousTransformationMatrix* matrix = engine->init_matrices();
    ClippingSpace* clipping = engine->init_clipping();
    Vectors* vec = engine->init_vector();
    Color* color = engine->init_color();

    //generate cube mesh
    std::vector<triangle> mesh = shape->generate_mesh();


    // Start rendering loop
    while (true) {

        cv::Vec3f camera_vector_world = camera->getCameraVector(camera->V_T_C);

        camera->resetCameraImage();

        engine->create_matrices();

        std::vector<triangle> visiable_mesh;

        for (auto& tri : mesh) {

            camera->world_transform(&camera->V_T_Cube, &tri);

            camera->camera_transform(&camera->C_T_V, &tri);

            std::tuple<cv::Mat, cv::Mat> result = vec->normal(tri, 0.5f);

            // Unpack the tuple
            cv::Mat normal_start = std::get<0>(result);
            cv::Mat normal_end = std::get<1>(result);

            //normal to camera
            cv::Mat normal_start_camera = camera->C_T_V * normal_start;
            cv::Mat normal_end_camera = camera->C_T_V * normal_end;

            if (engine->cubeSystemNormals == 1)
                camera->drawCameraImageArrow(normal_start_camera, normal_end_camera);

            if (is_triangle_facing_camera(tri, camera_vector_world) < 0.0f) {

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

        if (engine->cubeSystemPoints == 1)
        {
            camera->drawAllPoints(&clipped_mesh);
        }

        if (engine->cubeSystemFaces == 1)
        {
                camera->fillCubeFaces(&clipped_mesh);
        }

        camera->drawAllLines(&clipped_mesh);

        engine->update_fps();
        engine->renderFrame();

        int key = cv::waitKey(10) & 0xFF;
        engine->update_movement(key);

        if (key == 27) {
            break;
        }
        
    }

    // Clean up and shut down
    engine->shutdown();

    return 0;
}
