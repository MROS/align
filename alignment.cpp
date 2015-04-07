#include <iostream>
#include <opencv2/opencv.hpp>
#include "alignment.h"
#include "util.h"
#include "matrix.h"
using namespace cv;
using namespace std;

int mid_intensity(const Mat &image) {
    int intensity[256 * 3] = {};
    int size = (image.rows * image.cols);
    for (int row = 0; row < image.rows; row++) {
        for (int col = 0; col < image.cols; col++) {
            Vec3b s = image.at<Vec3b>(row, col);
            intensity[sum(s)] ++;
        }
    }
    int count = 0;
    int ans = 0;
    for (int i = 0; i < 256 * 3; i++) {
        count += intensity[i];
        if (count >= size / 2) {
            ans = i;
            break;
        }
    }
    return ans;
}

Mat black_white(const Mat &image) {
    int threshold = mid_intensity(image);
    Mat res = image.clone();  // need to be optimized
    for (int row = 0; row < image.rows; row++) {
        for (int col = 0; col < image.cols; col++) {
            if (sum(image.at<Vec3b>(row, col)) < threshold) {
                res.at<Vec3b>(row, col)[0] = 0;
                res.at<Vec3b>(row, col)[1] = 0;
                res.at<Vec3b>(row, col)[2] = 0;
            }
            else {
                res.at<Vec3b>(row, col)[0] = 255;
                res.at<Vec3b>(row, col)[1] = 255;
                res.at<Vec3b>(row, col)[2] = 255;
            }
        }
    }
    return res;
}

const int TOLERENT = 1;
Mat exclusion_bitmap(const Mat &image){
    int threshold = mid_intensity(image);
    Mat res = image.clone();  // need to be optimized
    for (int row = 0; row < image.rows; row++) {
        for (int col = 0; col < image.cols; col++) {
            if (sum(image.at<Vec3b>(row, col)) <= threshold + TOLERENT &&
                    sum(image.at<Vec3b>(row, col)) >= threshold - TOLERENT) {
                res.at<Vec3b>(row, col)[0] = 0;
                res.at<Vec3b>(row, col)[1] = 0;
                res.at<Vec3b>(row, col)[2] = 0;
            }
            else {
                res.at<Vec3b>(row, col)[0] = 255;
                res.at<Vec3b>(row, col)[1] = 255;
                res.at<Vec3b>(row, col)[2] = 255;
            }
        }
    }
    return res;
}

int to_pos(int x) {
    if (x <= 0)
        return 0;
    else
        return x;
}

// 回傳 xor 且 exclusion bitmap 之後與整體的比值
double diff_grade(Mat &img1, Mat &img2) {
    Mat eximg1 = exclusion_bitmap(img1);
    Mat eximg2 = exclusion_bitmap(img2);
    Mat eq12 = eqop(img1, img2);
    cout << "equal rate = " << black_rate(eq12) << endl;
    Mat table = andop(eq12, andop(eximg1, eximg2));
    int count = 0;
    for (int row = 0; row < img1.rows; row++) {
        for (int col = 0; col < img1.cols; col++) {
            if (table.at<Vec3b>(row, col)[0]) {
                count++;
            }
        }
    }
    cout << "count = " << count << endl;
    double total = img1.cols * img1.rows;
    return (count / total);
}

// 以 img1 左上角為原點, 回傳 img2 左上角的應該在的位置
const int LEVEL = 7;
Point offset(const Mat &img1, const Mat &img2, int level) {
    int rows = img1.rows, cols = img1.cols;
    Point next_level_offset(0, 0);
    if (level < LEVEL) {
        Mat nimg1, nimg2;
        resize(img1, nimg1, Size(), 0.5, 0.5);
        resize(img2, nimg2, Size(), 0.5, 0.5);
        next_level_offset = offset(nimg1, nimg2, level + 1);
    }
    next_level_offset *= 2;
    int direction[] = {-1, 0, 1};
    Point ans = next_level_offset;
    double maxrate = 0;
    cout << "offset level = " << level << endl;
    for (int i: direction) {
        for (int j: direction) {
            int ri = next_level_offset.x + i;
            int rj = next_level_offset.y + j;
            int new_width = cols - abs(ri);
            int new_height= rows - abs(rj);
            Mat cut1 = Mat(img1, Rect(to_pos(ri), to_pos(rj) , new_width, new_height));
            Mat cut2 = Mat(img2, Rect(to_pos(-ri), to_pos(-rj) ,  new_width, new_height));
           double diff = diff_grade(cut1, cut2);
            cout << "offset = " << ri << ", " << rj << endl;
            cout << "rate = " << diff << endl;
           if (maxrate < diff) {
               maxrate = diff;
               ans = Point(ri, rj);
           }
        }
    }
    cout << "offset = " << ans << endl;
    cout << "============== a level end =====================" << endl;
    return ans;
}


void align_all(vector<Mat> image_list) {
    vector<Mat> origin_list = image_list;
    // 先黑白化
    for (Mat &img: image_list) {
        img = black_white(img);
    }

    vector<Point> offset_list = {Point(0, 0)}; // 相對於第一張的offset
    int minx = 0, miny = 0, maxx = 0, maxy = 0;
    for (int i = 0; i < image_list.size() - 1; i++) {
        Point off = offset(image_list[i], image_list[i + 1], 0);
        cout << "offset = " << off << endl;
        off += offset_list[i]; // 求相對第一張
        offset_list.push_back(off);
        minx = min(minx, off.x);
        miny = min(miny, off.y);
        maxx = max(maxx, off.x);
        maxy = max(maxy, off.y);
    }
    int new_width = image_list[0].cols - (maxx - minx);
    int new_height = image_list[0].rows - (maxy - miny);
    for(int i = 0; i < image_list.size(); i++) {
        int x = maxx - offset_list[i].x, y = maxy - offset_list[i].y;
        Mat cutoff = Mat(origin_list[i], Rect(x, y, new_width, new_height));
        imwrite("aligned" + to_string(i) + ".jpg", cutoff);
    }
}
