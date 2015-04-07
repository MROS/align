#ifndef HDRCPP_ALIGNMENT_H
#define HDRCPP_ALIGNMENT_H
#include <opencv2/opencv.hpp>

cv::Mat black_white(const cv::Mat &image);
cv::Mat exclusion_bitmap(const cv::Mat &image);
void align_all(std::vector<cv::Mat> image_list);

#endif //HDRCPP_ALIGNMENT_H
