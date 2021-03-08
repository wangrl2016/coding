//
// Created by wangrl on 2021/3/7.
//

#include <fstream>
#include <iostream>
#include "WavCodec.h"

template<class T>
WavCodec<T>::WavCodec() {
    static_assert(std::is_floating_point<T>::value,
                  "Error: this version of WavCodec only supports float sample formats");


}

template<class T>
WavCodec<T>::WavCodec(std::string filePath) {
    load(filePath);
}

template<class T>
bool WavCodec<T>::load(std::string filePath) {
    std::ifstream file(filePath, std::ios::binary);
    if (!file.good()) {
        std::cout << "Cannot load file " << filePath << std::endl;
        return false;
    }

    std::vector<uint8_t> fileData;

    file.unsetf(std::ios::skipws);
    file.seekg(0, std::ios::end);
    size_t length = file.tellg();
    file.seekg(0, std::ios::beg);

    // allocate
    fileData.resize(length);

    file.read(reinterpret_cast<char*>(fileData.data()), length);
    file.close();

    if (file.gcount() != length) {
        std::cout << "Could not read entire file " << filePath << std::endl;
        return false;
    }

    if (mAudioFileFormat == AudioFileFormat::WAVE) {
        return decodeWaveFile(fileData);
    } else {
        std::cout << "Audio file type error" << std::endl;
        return false;
    }
}

template<class T>
AudioFileFormat WavCodec<T>::determineAudioFileFormat(std::vector<uint8_t>& fileData) {
    std::string header(fileData.begin(), fileData.begin() + 4);
    if (header == "RIFF")
        return AudioFileFormat::WAVE;
    else
        return AudioFileFormat::ERROR;
}

template<class T>
bool WavCodec<T>::decodeWaveFile(std::vector<uint8_t>& fileData) {
    // Header chunk
    std::string headerChunkID(fileData.begin(), fileData.begin() + 4);

    return false;
}


int main(int argc, char** argv) {
    WavCodec<float>* wav = new WavCodec<float>();
    wav->load("../../res/sample-wav-44k.wav");

    return 0;
}