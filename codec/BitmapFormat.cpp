//
// Created by wangrl on 2021/3/5.
//

#include "BitmapFormat.h"

int main(int argc, char** argv) {
    Bitmap bitmap(argv[1]);
    std::cout << "Size " << bitmap.getWidth() << "x" << bitmap.getHeight() << std::endl;
    bitmap.savePPMImage(argv[2]);
    return 0;
}
