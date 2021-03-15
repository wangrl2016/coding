//
// Created by wangrl on 2021/3/7.
//

#include <fstream>
#include <iostream>
#include <cassert>
#include <cstring>
#include "WavCodec.h"

template<class T>
WavCodec<T>::WavCodec() {
//    static_assert(std::is_floating_point<T>::value,
//                  "Error: this version of WavCodec only supports float sample formats");
}

template<class T>
WavCodec<T>::WavCodec(const std::string& filePath) {
    load(filePath);
}

template<class T>
bool WavCodec<T>::load(const std::string& filePath) {
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

    // get audio file format
    mAudioFileFormat = determineAudioFileFormat(fileData);
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
    // Contains the letters "RIFF" in ASCII form (0x52494646 big-endian form).
    std::string headerChunkID(fileData.begin(), fileData.begin() + 4);
    // Contains the letters "WAVE" (0x57415645 big-endian form).
    std::string format(fileData.begin() + 8, fileData.begin() + 12);

    // Try and find the start points of key chunks.
    int indexOfFormatChunk = getIndexOfChunk(fileData, "fmt ", 12);
    int indexOfDataChunk = getIndexOfChunk(fileData, "data", 12);

    if (headerChunkID != "RIFF" || format != "WAVE" ||
        indexOfFormatChunk == -1 || indexOfDataChunk == -1) {
        std::cout << "This does not seem to be a valid WAV file" << std::endl;
        return false;
    }

    int f = indexOfFormatChunk;

    std::string formatChunkID(fileData.begin() + f, fileData.begin() + f + 4);

    // PCM = 1 (i.e. Linear quantization)
    // Values other than 1 indicate some form of compression.
    uint16_t audioFormat = twoByteToInt(fileData, f + 8);
    // Mono = 1, Stereo = 2, etc.
    uint16_t numChannels = twoByteToInt(fileData, f + 10);
    // 8000, 44100, etc.
    mSampleRate = (uint32_t) fourBytesToInt(fileData, f + 12);
    // == SampleRate * NumChannels * BitsPerSample / 8
    uint32_t numBytesPerSecond = fourBytesToInt(fileData, f + 16);
    uint32_t numBytesPerBlock = twoByteToInt(fileData, f + 20);
    mBitDepth = (int) twoByteToInt(fileData, f + 22);

    uint16_t numBytesPerSample = static_cast<uint16_t>(mBitDepth) / 8;

    // Check that the audio format is PCM or Float or extensible.
    if (audioFormat != WavAudioFormat::PCM && audioFormat != WavAudioFormat::IEEEFloat &&
        audioFormat != WavAudioFormat::Extensible) {
        std::cout << "This WAV file is encoded in a format that this library does not support" << std::endl;
        return false;
    }

    // Check the number of channels is mono or stereo.
    if (numChannels < 1 || numChannels > 128) {
        std::cout << "This WAV file seems to be an invalid number of channels" << std::endl;
        return false;
    }

    // Check header data is consistent.
    if (numBytesPerSecond != static_cast<uint32_t>((numChannels * mSampleRate * mBitDepth) / 8) ||
        numBytesPerBlock != (numChannels * numBytesPerSample)) {
        std::cout << "The header data in this WAV file seems to be inconsistent" << std::endl;
        return false;
    }

    if (mBitDepth != 8 && mBitDepth != 16 && mBitDepth != 24 && mBitDepth != 32) {
        std::cout << "This file has a bit depth that is not 8, 16, 24, or 32 bits" << std::endl;
        return false;
    }

    // Data chunk
    int d = indexOfDataChunk;
    std::string dataChunkID(fileData.begin() + d, fileData.begin() + d + 4);
    int32_t dataChunkSize = fourBytesToInt(fileData, d + 4);

    int numSamples = dataChunkSize / (numChannels * mBitDepth / 8);
    int sampleStartIndex = indexOfDataChunk + 8;

    clearAudioBuffer();
    mSamples.resize(numChannels);

    for (int i = 0; i < numSamples; i++) {
        for (int channel = 0; channel < numChannels; channel++) {
            int sampleIndex = sampleStartIndex + (numBytesPerBlock * i) + channel * numBytesPerSample;

            if ((sampleIndex + (mBitDepth / 8) - 1) >= fileData.size()) {
                printf("Read file error as the metadata indicates more samples than "
                       "there are in the file data");
                return false;
            }

            // TODO: 只对于16位的进行操作
            if (mBitDepth == 16) {
                int16_t sampleAsInt = twoByteToInt(fileData, sampleIndex);
                mSamples[channel].push_back(sampleAsInt);
            }
        }
    }

    return true;
}

template<class T>
int WavCodec<T>::getIndexOfChunk(std::vector<uint8_t>& source, const std::string& chunkHeaderID, int startIndex,
                                 Endian endian) {
    int dataLen = 4;
    assert(chunkHeaderID.size() == dataLen);

    int i = startIndex;

    while (i < source.size() - dataLen) {
        if (memcmp(&source[i], chunkHeaderID.data(), dataLen) == 0)
            return i;   // 找到关键词的下标

        i += dataLen;
        // Subchunk1ID
        // Subchunk1Size
        // ...
        // Subchunk2ID
        // Subchunk2Size
        auto chunkSize = fourBytesToInt(source, i, endian);
        i += (dataLen + chunkSize);
    }
    return -1;
}

template<class T>
int32_t WavCodec<T>::fourBytesToInt(std::vector<uint8_t>& source, int startIndex, Endian endian) {
    int32_t result;
    if (endian == Endian::LittleEndianess)
        result = (source[startIndex + 3] << 24) | (source[startIndex + 2] << 16) |
                 (source[startIndex + 1] << 8) | source[startIndex];
    else
        result = (source[startIndex] << 24) | source[startIndex + 1] << 16 |
                 (source[startIndex + 2] << 8) | source[startIndex + 3];
    return result;
}

template<class T>
int16_t WavCodec<T>::twoByteToInt(std::vector<uint8_t>& source, int startIndex, Endian endian) {
    int16_t result;
    if (endian == Endian::LittleEndianess)
        result = (source[startIndex + 1] << 8) | source[startIndex];
    else
        result = (source[startIndex] << 8) | source[startIndex + 1];
    return result;
}

template<class T>
void WavCodec<T>::clearAudioBuffer() {
    for (size_t i = 0; i < mSamples.size(); i++)
        mSamples[i].clear();
    mSamples.clear();
}

template<class T>
bool WavCodec<T>::pcmSave(std::string filePath) {
    return false;
}

int main(int argc, char** argv) {

    if (argc != 2) {
        printf("Usage: %s input_wav_file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    WavCodec<float>* wav = new WavCodec<float>();
    wav->load(argv[1]);

    return 0;
}
