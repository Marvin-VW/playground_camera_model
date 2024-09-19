#include "CubeTracker.h"

CubeTracker::CubeTracker(int frame_width, int frame_height)
    : frame_width(frame_width), frame_height(frame_height),
      lower_pink(cv::Scalar(130, 130, 130)), upper_pink(cv::Scalar(255, 255, 255)) {
}

std::vector<cv::Point2d> CubeTracker::trackCube(const cv::Mat& frame) {
    std::vector<cv::Point2d> corner_points;

    cv::Mat hls_frame;
    cv::Mat mask;

    cv::cvtColor(frame, hls_frame, cv::COLOR_RGB2HLS);

    cv::inRange(hls_frame, lower_pink, upper_pink, mask);

    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(mask, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    if (!contours.empty()) {
        auto largest_contour = *std::max_element(contours.begin(), contours.end(),
                                                 [](const std::vector<cv::Point>& a, const std::vector<cv::Point>& b) {
                                                     return cv::contourArea(a) < cv::contourArea(b);
                                                 });

        cv::Rect bounding_rect = cv::boundingRect(largest_contour);

        corner_points.push_back(cv::Point2d(bounding_rect.x, bounding_rect.y)); // top left
        corner_points.push_back(cv::Point2d(bounding_rect.x + bounding_rect.width, bounding_rect.y)); // top right
        corner_points.push_back(cv::Point2d(bounding_rect.x, bounding_rect.y + bounding_rect.height)); // bottom left
        corner_points.push_back(cv::Point2d(bounding_rect.x + bounding_rect.width, bounding_rect.y + bounding_rect.height)); // bottom right

        cv::rectangle(frame, bounding_rect, cv::Scalar(0, 255, 0), 2);
        for (const auto& point : corner_points) {
            //cv::circle(frame, point, 5, cv::Scalar(255, 0, 0), -1);
        }
    }

    cv::imshow("Mask", mask);
    return corner_points;
}
