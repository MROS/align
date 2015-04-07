#include "util.h"

using namespace cv;
int sum(Vec3b s) {
    return ((int)s.val[0] + (int)s.val[1] + (int)s.val[2]);
}

double black_rate(cv::Mat &image) {
    double count = 0;
    for (int row = 0; row < image.rows; row++) {
        for (int col = 0; col < image.cols; col++) {
            if (image.at<Vec3b>(row, col)[0]) {
                count++;
            }
        }
    }
    return (count / (image.cols * image.rows));
}
