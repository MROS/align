#include <iostream>
#include <opencv2/opencv.hpp>
#include "matrix.h"
using namespace cv;
using namespace std;

Mat andop(const Mat &img1, const Mat &img2) {
    Mat res = img1.clone(); // TODO: copy ??
    for (int row = 0; row < img1.rows; row++) {
        for (int col = 0; col < img1.cols; col++) {
            if (img1.at<Vec3b>(row, col)[0] && img2.at<Vec3b>(row, col)[0]) {
                res.at<Vec3b>(row, col)[0] = 255;
                res.at<Vec3b>(row, col)[1] = 255;
                res.at<Vec3b>(row, col)[2] = 255;
            }
            else {
                res.at<Vec3b>(row, col)[0] = 0;
                res.at<Vec3b>(row, col)[1] = 0;
                res.at<Vec3b>(row, col)[2] = 0;
            }
        }
    }
    return res;
}

Mat eqop(const Mat &img1, const Mat &img2) {
    Mat res = img1.clone(); // TODO: copy ??
    for (int row = 0; row < img1.rows; row++) {
        for (int col = 0; col < img1.cols; col++) {
            if (img1.at<Vec3b>(row, col)[0] == img2.at<Vec3b>(row, col)[0]) {
                res.at<Vec3b>(row, col)[0] = 255;
                res.at<Vec3b>(row, col)[1] = 255;
                res.at<Vec3b>(row, col)[2] = 255;
            }
            else {
                res.at<Vec3b>(row, col)[0] = 0;
                res.at<Vec3b>(row, col)[1] = 0;
                res.at<Vec3b>(row, col)[2] = 0;
            }
        }
    }
    return res;
}
