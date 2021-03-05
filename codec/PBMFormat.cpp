//
// Created by wangrl on 2021/3/5.
//

#include<vector>
#include <fstream>
#include <iostream>

int main(int argc, char* argv[]) {
    const int width = 1024;
    const int height = 768;

    std::ofstream ofs;

    // P4
    ofs.open("../out/format_pbm_p4.pbm");
    ofs << "P4\n" << width << " " << height << "\n";

    std::vector<float> buffer1(width * height);
    for (size_t j = 0; j < height; j++) {
        for (size_t i = 0; i < width; i++) {
            buffer1[i + j * width] = (i % 5 == 0 || i % 6 == 0) ? 1 : 0;
        }
    }

    for (size_t i = 0; i < width * height; ++i)
        ofs << buffer1[i];
    ofs.close();

    // P5
    ofs.open("../out/format_pgm_p5.pgm");
    ofs << "P5\n" << width << " " << height << "\n255\n";

    std::vector<float> buffer2(width * height);
    for (size_t j = 0; j < height; j++)
        for (size_t i = 0; i < width; i++)
            buffer2[i + j * width] = j / float(height);

    for (size_t i = 0; i < width * height; ++i)
        ofs << (char) (255 * std::max(0.0f, std::min(1.0f, buffer2[i])));
    ofs.close();

    // P6
    ofs.open("../out/format_ppm_p6.ppm");
    ofs << "P6\n" << width << " " << height << "\n255\n";

    std::vector<float> buffer3(width * height * 3);
    for (size_t j = 0; j < height; j++) {
        for (size_t i = 0; i < width; i++) {
            buffer3[3 * (i + j * width)] = j / float(height);
            buffer3[3 * (i + j * width) + 1] = i / float(width);
            buffer3[3 * (i + j * width) + 2] = 0;
        }
    }

    for (size_t i = 0; i < height * width * 3; ++i) {
        ofs << (char) (255 * std::max(0.f, std::min(1.f, buffer3[i])));
    }
    ofs.close();

    return 0;
}
