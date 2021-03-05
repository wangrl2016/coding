//
// Created by wangrl on 2021/3/5.
//



#include "BitmapFormat.h"

int main(int argc, char** argv) {
    Bitmap bitmap("../../res/sample-640x426.bmp");
    std::cout << "Size " << bitmap.getWidth() << "x" << bitmap.getHeight() << std::endl;
    return 0;
}