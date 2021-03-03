//
// Created by wangrl on 2021/3/3.
//

#include <cstdio>
#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: %s image_path\n", argv[0]);
        return EXIT_FAILURE;
    }

    cv::Mat image = cv::imread(argv[1], cv::IMREAD_UNCHANGED);

    if (!image.data) {
        printf("No image data\n");
        return -1;
    }

    cv::namedWindow("Display image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Display image", image);

    cv::waitKey(0);

    return 0;
}
