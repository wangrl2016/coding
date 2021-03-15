//
// Created by wangrl on 2021/3/8.
//

#include <fstream>
#include <iostream>
#include "GifCodec.h"

#define GIF87_STAMP "GIF87a"
#define GIF89_STAMP "GIF89a"

GifDecoder::GifDecoder() :
        mVersion(0) {

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
    std::string tag(fileData.begin(), fileData.begin() + 6);

    if (tag == GIF87_STAMP)
        mVersion = 87;
    else if (tag == GIF89_STAMP)
        mVersion = 89;
    else {
        // This prevent attempting to continue reading this invalid stream.
        return false;
    }

    // Reads logical screen descriptor


    return false;
}

bool GifDecoder::readContent() {
    return false;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Usage: %s input_gif_file\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    auto* decoder = new GifDecoder();
    decoder->read(argv[1]);
    for (int i = 0; i < decoder->getFrameCount(); i++) {
        decoder->getFrame(i);    // frame i
        decoder->getDelay(i);    // duration of frame in milliseconds
    }
    return 0;
}
