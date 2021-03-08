//
// Created by wangrl on 2021/3/7.
//

/**
 * 协议
 *
 * http://soundfile.sapp.org/doc/WaveFormat/
 *
 * The WAVE file format is a subset of Microsoft's RIFF specification for the storage of
 * multimedia files. A RIFF file starts out with a file header followed by a sequence of
 * data chunks.
 *
 * A WAVE file is often just a RIFF file with a single "WAVE" chunk which consists of two
 * sub-chunks -- a "fmt" chunk specifying the data format and a "data" chunk containing
 * the actual sample data.
 *
 * 参考实现
 *
 * https://github.com/adamstark/AudioFile
 */

#pragma once

#include <vector>
#include <string>

/**
 * The different types of audio file, plus some other types to
 * indicate a failure to load a file, or that one hasn't been
 * loaded yet.
 */
enum class AudioFileFormat {
    ERROR,
    NOT_LOADED,
    WAVE,
};

template<class T>
class WavCodec {
public:
    typedef std::vector<std::vector<T>> AudioBuffer;

    WavCodec();

    // Constructor, using a given file path to load a file.
    WavCodec(std::string filePath);

    /**
     * Loads an audio file from a given file path.
     *
     * @return true if the file was successfully loaded
     */
    bool load(std::string filePath);

private:

    AudioFileFormat determineAudioFileFormat(std::vector<uint8_t>& fileData);
    bool decodeWaveFile(std::vector<uint8_t>& fileData);

    AudioFileFormat mAudioFileFormat;
    uint32_t mSampleRate;
    int mBitDepth;
};


