// Copyright (C) 2024 Marvin-VW
#include "ClippingSpace.h"
#include "Shape.h"
#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

#define DEG_TO_RAD(x) ((x) * (M_PI / 180.0))

ClippingSpace::ClippingSpace() 
{
    double fov = DEG_TO_RAD(64.0 / 1.77);
    double aspect_ratio = 1.77;
    double near = 1.0;
    double far = 100.0;
    create_perspective_projection_matrix(fov, aspect_ratio, near, far);

    border = 0.9f;
}

cv::Mat ClippingSpace::create_perspective_projection_matrix(double fov, double aspect_ratio, double near, double far) {
    double f = 1.0 / std::tan(fov / 2.0);
    double nf = 1.0 / (near - far);

    double temp_matrix[4*4] = {
        f / aspect_ratio,   0,              0,                         0,
        0,                  f,              0,                         0,
        0,                  0,              (far + near) * nf,          (2 * far * near) * nf,
        0,                  0,              -1,                        0
    };

    cv::Mat matrix(4, 4, CV_64F, temp_matrix);
    this->projection_matrix = matrix.clone();

    return matrix;
}

std::vector<triangle> ClippingSpace::cubeInSpace(const std::vector<triangle>* cube_points)
{
    std::vector<triangle> full_triangle_list;
    for (const auto& tri : *cube_points)
    {

        std::vector<cv::Mat> inside_points, outside_points;
        for (const auto& point : tri.camera_points)
        {

            cv::Mat clip_space_point = projection_matrix * point;
            cv::Mat ndc_point = clip_space_point / clip_space_point.at<double>(3);

            // Check if point is inside the clipping space
            if (ndc_point.at<double>(0) >= -border && ndc_point.at<double>(0) <= border &&
                ndc_point.at<double>(1) >= -border && ndc_point.at<double>(1) <= border && 
                ndc_point.at<double>(2) >= 0.1f && ndc_point.at<double>(2) <= 100.0f)
            
            {
                inside_points.push_back(ndc_point);
            }
            else
            {
                outside_points.push_back(ndc_point);
            }

        }

        if (inside_points.size() == 3)
        {
            full_triangle_list.push_back(tri);
        }
        else if (inside_points.size() == 1)
        {


            cv::Mat new_point1, new_point2;
            new_point1 = find_intersection_with_plane(inside_points[0], outside_points[0]);
            new_point2 = find_intersection_with_plane(inside_points[0], outside_points[1]);

            std::vector<cv::Mat> full_point_list = {
                inside_points[0],
                appendHomogeneousCoordinate(new_point1),
                appendHomogeneousCoordinate(new_point2)
            };


            for (auto& point : full_point_list)
            {
                point = transferBackCameraSpace(point);
            }

            triangle t = tri;
            for (int i = 0; i < 3; i++)
            {
                t.point[i] = full_point_list[i];
            }

            full_triangle_list.push_back(t);
        }
        else if (inside_points.size() == 2)
        {

            cv::Mat new_point1, new_point2;
            new_point1 = find_intersection_with_plane(inside_points[0], outside_points[0]);
            new_point2 = find_intersection_with_plane(inside_points[1], outside_points[0]);

            triangle t1 = tri;
            triangle t2 = tri;
            t1.point[0] = appendHomogeneousCoordinate(new_point1);
            t1.point[1] = appendHomogeneousCoordinate(new_point2);
            t1.point[2] = inside_points[0];
            for (int i = 0; i < 3; i++)
            {
                t1.point[i] = transferBackCameraSpace(t1.point[i]);
            }

            full_triangle_list.push_back(t1);
            t2.point[0] = inside_points[0];
            t2.point[1] = appendHomogeneousCoordinate(new_point2);
            t2.point[2] = inside_points[1];
            for (int i = 0; i < 3; i++)
            {
                t2.point[i] = transferBackCameraSpace(t2.point[i]);
            }
            full_triangle_list.push_back(t2);
        }
    }
    return full_triangle_list;
}

cv::Mat createPerspectiveProjectionMatrix(double fov, double aspect_ratio, double near, double far)
    {
        double f = 1.0 / std::tan(fov / 2.0);
        double nf = 1.0 / (near - far);

        cv::Mat proj = (cv::Mat_<double>(4, 4) << 
            f / aspect_ratio, 0, 0, 0,
            0, f, 0, 0,
            0, 0, (far + near) * nf, (2 * far * near) * nf,
            0, 0, -1, 0);

        return proj;
    }

cv::Mat ClippingSpace::appendHomogeneousCoordinate(const cv::Mat& point)
{
    cv::Mat homogenous_point(4, 1, CV_64F);

    homogenous_point.at<double>(0) = point.at<double>(0);
    homogenous_point.at<double>(1) = point.at<double>(1);
    homogenous_point.at<double>(2) = point.at<double>(2);
    
    homogenous_point.at<double>(3) = 1.0;

    return homogenous_point;
}

cv::Mat ClippingSpace::intersection_with_plane_x(const cv::Mat& A, const cv::Mat& B, float x) {
    // Extract x coordinates of A and B
    double x0 = A.at<double>(0);
    double x1 = B.at<double>(0);

    double t = (x - x0) / (x1 - x0);

    if (t >= 0 && t <= 1) {

        // Direction vector from A to B
        cv::Mat direction = B - A;

        // intersection point
        cv::Mat intersection = A + t * direction;

        return intersection;
    }
    return cv::Mat();
}

cv::Mat ClippingSpace::intersection_with_plane_y(const cv::Mat& A, const cv::Mat& B, float y) {
    // Extract y coordinates of A and B
    double y0 = A.at<double>(1);
    double y1 = B.at<double>(1);

    double t = (y - y0) / (y1 - y0);

    if (t >= 0 && t <= 1) {

        // Direction vector from A to B
        cv::Mat direction = B - A;

        // intersection point
        cv::Mat intersection = A + t * direction;

        return intersection;
    }
    return cv::Mat();
}

cv::Mat ClippingSpace::find_intersection_with_plane(const cv::Mat& point1, const cv::Mat point2) {

    float planes_x[] = {-border, border};
    float planes_y[] = {-border, border};
    // Flatten and reduce to 3D points
    cv::Mat A = point1.rowRange(0, 3);
    cv::Mat B = point2.rowRange(0, 3);
    // Get intersections with borders
    std::map<std::string, cv::Mat> intersections = {
        {"left", intersection_with_plane_x(A, B, planes_x[0])},
        {"right", intersection_with_plane_x(A, B, planes_x[1])},
        {"bottom", intersection_with_plane_y(A, B, planes_y[0])},
        {"top", intersection_with_plane_y(A, B, planes_y[1])}
    };

    // Filter out empty intersections (no intersection)
    std::map<std::string, cv::Mat> valid_intersections;
    for (const auto& entry : intersections) {
        if (!entry.second.empty()) {
            valid_intersections[entry.first] = entry.second;
        }
    }

    if (valid_intersections.empty()) {
        return cv::Mat();
    }
    std::string closest_intersection;
    float min_distance = std::numeric_limits<float>::max();
    cv::Mat closest_point;
    for (const auto& entry : valid_intersections) {
        float distance = cv::norm(entry.second - A);
        if (distance < min_distance) {
            min_distance = distance;
            closest_intersection = entry.first;
            closest_point = entry.second;
        }
    }
    return closest_point;
}

cv::Mat ClippingSpace::transferBackCameraSpace(const cv::Mat& point) {
    // Inverse of the projection matrix
    cv::Mat invProjectionMatrix;
    invProjectionMatrix = projection_matrix.inv();

    cv::Mat convertedPoint = invProjectionMatrix * point;
    convertedPoint /= convertedPoint.at<double>(3);

    return convertedPoint;
}
