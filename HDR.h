#ifndef HDRCPP_HDR_H
#define HDRCPP_HDR_H
#include <opencv2/opencv.hpp>
#include <vector>

cv::Mat HDR(std::vector<cv::Mat> image_list);

#endif //HDRCPP_HDR_H
