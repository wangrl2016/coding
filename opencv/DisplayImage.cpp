//
// Created by wangrl on 2021/3/3.
//

#include <cstdio>
#include <opencv2/opencv.hpp>
#include <opencv2/core/hal/interface.h>

static cv::Mat& ScanImageAndReduceIterator(cv::Mat& I, const uchar* const table) {
    // Accept only char type matrices.
    CV_Assert(I.depth() == CV_8U);

    const int channels = I.channels();

    switch (channels) {
        case 1: {
            cv::MatIterator_<uchar> it, end;
            for (it = I.begin<uchar>(), end = I.end<uchar>(); it != end; ++it)
                *it = table[*it];
            break;
        }
        case 3: {
            cv::MatIterator_<cv::Vec3b> it, end;
            for (it = I.begin<cv::Vec3b>(), end = I.end<cv::Vec3b>(); it != end; ++it) {
                for (int k = 0; k < 3; k++) {
                    (*it)[k] = table[(*it)[k]];
                }
            }
            break;
        }
        case 4: {
            cv::MatIterator_<cv::Vec4b> it, end;
            for (it = I.begin<cv::Vec4b>(), end = I.end<cv::Vec4b>(); it != end; ++it) {
                for (int k = 0; k < 4; k++) {
                    (*it)[k] = table[(*it)[k]];
                }
            }
            break;
        }
        default:
            break;
    }
    return I;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: %s image_path\n", argv[0]);
        return EXIT_FAILURE;
    }

    cv::Mat image = cv::imread(argv[1], cv::IMREAD_UNCHANGED);

    // Accept only char type matrices.
    assert(image.depth() == CV_8U);

    int channels = image.channels();
    int nRows = image.rows;
    int nCols = image.cols * channels;

    if (image.isContinuous()) {
        nCols *= nRows;
        nRows = 1;
    }

    uchar* p;
    for (int i = 0; i < nRows; ++i) {
        p = image.ptr<uchar>(i);
        for (int j = 0; j < nCols; j += 4) {
            // RGBA格式
            if (p[j + 3] == 0) {
                p[j] = 255;
                p[j + 1] = 255;
                p[j + 2] = 255;
            }
        }
    }

    uchar table[256];
    for (int i = 0; i < 256; ++i)
        table[i] = (uchar) (10 * (i / 10));

    ScanImageAndReduceIterator(image, table);

    if (!image.data) {
        printf("No image data\n");
        return -1;
    }

    cv::namedWindow("Display image", cv::WINDOW_AUTOSIZE);
    cv::imshow("Display image", image);
    cv::waitKey(1000);

    return 0;
}
