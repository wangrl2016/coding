//
// Created by wangrl on 2021/3/8.
//

#include <fstream>
#include <iostream>
#include "GifCodec.h"

GifDecoder::GifDecoder() {

}

bool GifDecoder::read(std::string path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.good()) {
        std::cout << "Cannot load file " << path << std::endl;
        return false;
    }

    file.unsetf(std::ios::skipws);
    file.seekg(0, std::ios::end);
    size_t length = file.tellg();
    file.seekg(0, std::ios::beg);

    fileData.resize(length);
    file.read(reinterpret_cast<char*>(fileData.data()), length);
    file.close();

    if (file.gcount() != length) {
        std::cout << "Could not read entire file " << path << std::endl;
        return false;
    }

    bool status;

    status = readHeader();

    return status;
}

int GifDecoder::getFrameCount() {
    return 0;
}

char* GifDecoder::getFrame(int i) {
    return nullptr;
}

int64_t GifDecoder::getDelay(int i) {
    return 0;
}

bool GifDecoder::readHeader() {
    std::string tag(fileData.begin(), fileData.begin() + 3);
    if (tag == "GIF" || tag == "gif") {
        std::cout << tag.c_str() << std::endl;
    }
    return false;
}

bool GifDecoder::readContent() {
    return false;
}

int main(int argc, char** argv) {
    auto* decoder = new GifDecoder();
    decoder->read("../../res/shellsort.gif");
    for (int i = 0; i < decoder->getFrameCount(); i++) {
        decoder->getFrame(i);    // frame i
        decoder->getDelay(i);    // duration of frame in milliseconds
    }
    return 0;
}
