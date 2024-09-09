#include <opencv2/opencv.hpp>
#include <vector>
#include "Shape.h"


class ClippingSpace
{
public:
    ClippingSpace();

    std::vector<triangle> cubeInSpace(const std::vector<triangle>& cube_points);

private:
    cv::Mat projection_matrix;
    float border;

    cv::Mat create_perspective_projection_matrix(double fov, double aspect_ratio, double near, double far);
    cv::Mat find_intersection_with_plane(const cv::Mat& point1, const cv::Mat point2);
    cv::Mat transferBackCameraSpace(const cv::Mat& point);
    cv::Mat intersection_with_plane_x(const cv::Mat& A, const cv::Mat& B, float x);
    cv::Mat intersection_with_plane_y(const cv::Mat& A, const cv::Mat& B, float y);
    cv::Mat appendHomogeneousCoordinate(const cv::Mat& point);
};
