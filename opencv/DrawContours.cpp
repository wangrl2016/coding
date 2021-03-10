//
// Created by wangrl on 2021/3/4.
//

#include <opencv2/opencv.hpp>
#include <vector>

int main(int argc, char** argv) {
    if (argc != 4) {
        printf("Usage: %s input output1 output2\n", argv[0]);
        return EXIT_FAILURE;
    }

    cv::Mat src = cv::imread(argv[1], cv::IMREAD_UNCHANGED);

    CV_Assert(src.depth() == CV_8U);

    int channels = src.channels();

    cv::Mat img;
    img.create(src.rows, src.cols, CV_8U);

    uchar* p, * q;
    // 二值化处理
    for (int i = 0; i < src.rows; ++i) {
        p = src.ptr<uchar>(i);
        q = img.ptr<uchar>(i);
        for (int j = 0; j < src.cols * channels; j += 4) {
            if (p[j + 3] < 127)
                q[j / channels] = 0;
            else
                q[j / channels] = 255;
        }
    }

    cv::imwrite(argv[2], img);

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(img, contours, hierarchy,
                     cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    cv::Mat drawing = cv::Mat::zeros(img.size(), CV_8UC3);

    for (size_t i = 0; i < contours.size(); i++) {
        cv::Scalar color = cv::Scalar(255, 255, 0);
        cv::drawContours(drawing, contours, (int) i, color,
                         2, cv::LINE_8, hierarchy, 0);
    }

    cv::imwrite(argv[3], drawing);

    return EXIT_SUCCESS;
}
