#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include "alignment.h"
using namespace std;
using namespace cv;

vector<Mat> make_image_list(vector<char *> &name_list) {
    vector<Mat> ret;
    for (char *name: name_list){
        Mat image = imread(name);
        if (image.empty()) {
            cerr << "load image " << name << " fail" << endl;
            exit(0);
        }
        ret.push_back(image);
    }
    return ret;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        cerr << "usage: align image1 image2 image3 ..." << endl;
        return 0;
    }
    vector<char*> name_list;
    for (int i = 1; i < argc; i++) {
        name_list.push_back(argv[i]);
    }
    vector<Mat> image_list = make_image_list(name_list);

    align_all(image_list);
    return 0;
}




