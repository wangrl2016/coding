//
// Created by wangrl on 2021/3/9.
//

#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Usage: %s input_image" << argv[0] << std::endl;
        return 0;
    }
    cv::Mat src = cv::imread(argv[1]);

    cv::Mat dst;

    cv::resize(src, dst, cv::Size(src.cols * 2, src.rows * 2), 0, 0, cv::INTER_LINEAR);



    return 0;
}