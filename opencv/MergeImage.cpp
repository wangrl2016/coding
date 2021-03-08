//
// Created by wangrl on 2021/3/8.
//

#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "Usage: %s img1 img2" << argv[0] << std::endl;
        return 0;
    }

    cv::Mat img1 = cv::imread(argv[1]);
    cv::Mat img2 = cv::imread(argv[2]);

    assert(img1.rows == img2.rows);
    assert(img1.channels() == img2.channels());

    int cols = img1.cols + img2.cols;
    int channels = img1.channels();

    cv::Mat dst(img1.rows, cols, CV_8UC3, cv::Scalar(255, 255, 255));

    uchar* p, * p1, * p2;
    for (int i = 0; i < img1.rows; ++i) {
        p = dst.ptr<uchar>(i);
        p1 = img1.ptr<uchar>(i);
        p2 = img2.ptr<uchar>(i);
        for (int j = 0; j < cols * channels; j += 3) {
            if (j < img1.cols * channels) {
                p[j] = p1[j];
                p[j + 1] = p1[j + 1];
                p[j + 2] = p1[j + 2];
            } else {
                p[j] = p2[j - img1.cols * channels];
                p[j + 1] = p2[j - img1.cols * channels + 1];
                p[j + 2] = p2[j - img1.cols * channels + 2];
            }
        }
    }

    cv::imwrite("../out/merge_image.png", dst);
    return 0;
}
