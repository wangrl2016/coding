//
// Created by wangrl on 2021/3/3.
//

#include <opencv2/opencv.hpp>

static void Sharpen(const cv::Mat& image, cv::Mat& result) {
    CV_Assert(image.depth() == CV_8U);

    const int nChannels = image.channels();
    result.create(image.size(), image.type());

    for (int j = 1; j < image.rows - 1; ++j) {
        const uchar* previous = image.ptr<uchar>(j - 1);
        const uchar* current = image.ptr<uchar>(j);
        const uchar* next = image.ptr<uchar>(j + 1);

        uchar* output = result.ptr<uchar>(j);

        for (int i = nChannels; i < nChannels * (image.cols - 1); ++i) {
            *output++ = cv::saturate_cast<uchar>(5 * current[i] -
                                                 current[i - nChannels] - current[i + nChannels] - previous[i] -
                                                 next[i]);
        }
    }
    result.row(0).setTo(cv::Scalar(0));
    result.row(result.rows - 1).setTo(cv::Scalar(0));
    result.col(0).setTo(cv::Scalar(0));
    result.col(result.cols - 1).setTo(cv::Scalar(0));
}

int main(int argc, char** argv) {
    cv::Mat src, dst0, dst1;

    src = cv::imread(argv[1], cv::IMREAD_UNCHANGED);

    // Accept only char type matrices.
    assert(src.depth() == CV_8U);

    int channels = src.channels();
    int nRows = src.rows;
    int nCols = src.cols * channels;

    if (src.isContinuous()) {
        nCols *= nRows;
        nRows = 1;
    }

    uchar* p;
    for (int i = 0; i < nRows; ++i) {
        p = src.ptr<uchar>(i);
        for (int j = 0; j < nCols; j += 4) {
            // RGBA格式
            if (p[j + 3] == 0) {
                p[j] = 255;
                p[j + 1] = 255;
                p[j + 2] = 255;
            }
        }
    }

    Sharpen(src, dst0);
    cv::imwrite(argv[2], dst0);

    cv::Mat kernel = (cv::Mat_<char>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);

    cv::filter2D(src, dst1, src.depth(), kernel);
    cv::imwrite(argv[3], dst1);
    return EXIT_SUCCESS;
}
