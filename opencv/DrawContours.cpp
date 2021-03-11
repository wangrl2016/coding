//
// Created by wangrl on 2021/3/4.
//

#include <opencv2/opencv.hpp>
#include <vector>
#include <fstream>
#include <string>

int main(int argc, char** argv) {
    std::string inputImage;
    std::string outputSvg;
    if (argc == 2) {
        inputImage = argv[1];
        size_t pos = inputImage.rfind('.');
        outputSvg = inputImage.substr(0, pos) + std::string(".svg");
    } else if (argc == 3) {
        inputImage = argv[1];
        outputSvg = argv[2];
    } else {
        printf("Usage: %s input_image output_file\n"
               "Use input path as output if only one argument", argv[0]);
        return EXIT_FAILURE;
    }

    cv::Mat src = cv::imread(inputImage, cv::IMREAD_UNCHANGED);
    CV_Assert(src.depth() == CV_8U);
    CV_Assert(src.channels() == 4);

    int width = src.cols;
    int height = src.rows;
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

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(img, contours, hierarchy,
                     cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    std::cout << "Contours size " << contours.size() << std::endl;

    std::string path;
    for (auto contour : contours) {
        for (int j = 0; j < contour.size(); j++) {
            std::string temp = std::to_string(contour[j].x) + std::string(" ") +
                               std::to_string(contour[j].y);
            if (j == 0) {
                path += std::string(" M ") + temp;
            } else {
                path += std::string(" L ") + temp;
            }
        }
        path += " Z ";
    }

    std::string svg = "<svg width=\"%d\" height=\"%d\" viewBox=\"0 0 %d %d\" version=\"1.1\" "
                      "xmlns=\"http://www.w3.org/2000/svg\" "
                      "xmlns:xlink=\"http://www.w3.org/1999/xlink\">"
                      "<path d=\"%s\" fill=\"#00ffcc\"></path></svg>";

    // C++中字符串以\0结束
    // -9 = -(len(%d) * 4 + len(%s)) + 1
    int size = svg.size() + path.size() - 9 +
               (std::to_string(width).size() + std::to_string(height).size()) * 2;

    char args[size];
    snprintf(args, size, svg.c_str(), width, height, width, height, path.c_str());

    std::ofstream file(outputSvg);
    // 不要将\0写入文件中
    file.write(args, size - 1);

    return EXIT_SUCCESS;
}
