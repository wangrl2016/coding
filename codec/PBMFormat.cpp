//
// Created by wangrl on 2021/3/5.
//

#include<vector>
#include <fstream>
#include <iostream>

/**
 * 构造pbm图片格式
 */

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s output_file1 output_file2", argv[0]);
        exit(EXIT_SUCCESS);
    }

    const int width = 128;
    const int height = 76;

    std::ofstream ofs;

    // P5
    ofs.open(argv[1]);
    ofs << "P5\n" << width << " " << height << "\n255\n";

    std::vector<float> buffer1(width * height);
    for (size_t j = 0; j < height; j++)
        for (size_t i = 0; i < width; i++)
            buffer1[i + j * width] = j / float(height);

    for (size_t i = 0; i < width * height; ++i)
        ofs << (char) (255 * std::max(0.0f, std::min(1.0f, buffer1[i])));
    ofs.close();

    // P6
    ofs.open(argv[2]);
    ofs << "P6\n" << width << " " << height << "\n255\n";

    std::vector<float> buffer2(width * height * 3);
    for (size_t j = 0; j < height; j++) {
        for (size_t i = 0; i < width; i++) {
            buffer2[3 * (i + j * width)] = j / float(height);
            buffer2[3 * (i + j * width) + 1] = i / float(width);
            buffer2[3 * (i + j * width) + 2] = 0;
        }
    }

    for (size_t i = 0; i < height * width * 3; ++i) {
        ofs << (char) (255 * std::max(0.f, std::min(1.f, buffer2[i])));
    }
    ofs.close();

    return 0;
}
