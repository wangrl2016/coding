//
// Created by wangrl on 2021/3/5.
//

#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {

    cv::Mat image = cv::imread("../../res/foreign-girl.jpeg");
    cv::Mat newImage = cv::Mat::zeros(image.size(), image.type());

    double alpha = 2.2;
    int beta = 50;

    for (int y = 0; y < image.rows; y++) {
        for (int x = 0; x < image.cols; x++) {
            for (int c = 0; c < image.channels(); c++) {
                newImage.at<cv::Vec3b>(y, x)[c] =
                        cv::saturate_cast<uchar>(alpha * image.at<cv::Vec3b>(y, x)[c] + beta);
            }
        }
    }

    cv::imwrite("../out/contrast_bright.png", newImage);

    return 0;
}