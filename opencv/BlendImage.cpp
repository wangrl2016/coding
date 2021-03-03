//
// Created by wangrl on 2021/3/3.
//

#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
    double alpha = 0.5;

    // BGR
    cv::Mat src = cv::imread(argv[1], cv::IMREAD_COLOR);

    CV_Assert(src.depth() == CV_8U);

    int channels = src.channels();
    int nRows = src.rows;
    int nCols = src.cols * channels;

//    if (src.isContinuous()) {
//        nCols *= nRows;
//        nRows = 1;
//    }

    cv::Mat img1, img2;

    cv::Size size = src.size();
    printf("Size %dx%d\n", size.width, size.height);

    img1.create(nRows / 2, nCols / channels, src.type());
    img2.create(nRows / 2, nCols / channels, src.type());

    uchar* p, * m, * n;
    for (int i = 0; i < nRows; ++i) {
        if (i < nRows / 2) {
            p = src.ptr<uchar>(i);
            m = img1.ptr<uchar>(i);
            for (int j = 0; j < nCols; ++j) {
                m[j] = p[j];
            }
        } else {
            p = src.ptr<uchar>(i);
            n = img2.ptr<uchar>(i - nRows / 2);
            for (int j = 0; j < nCols; ++j) {
                n[j] = p[j];
            }
        }
    }

    double beta = 1.0 - alpha;
    cv::Mat dst;
    cv::addWeighted(img1, alpha, img2, beta, 0.0, dst);

    cv::imshow("Linear Blend", dst);
    cv::waitKey();

    return EXIT_SUCCESS;
}
