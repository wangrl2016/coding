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
enum AudioFileFormat {
    ERROR,
    NOT_LOADED,
    WAVE,
};

enum Endian {
    LittleEndianess,
    BigEndianess
};

enum WavAudioFormat {
    PCM = 0x0001,
    IEEEFloat = 0x0003,
    ALaw = 0x0006,
    MuLaw = 0x0007,
    Extensible = 0xFFFE
};

template<class T>
class WavCodec {
public:
    typedef std::vector<std::vector<T> > AudioBuffer;

    WavCodec();

    // Constructor, using a given file path to load a file.
    WavCodec(const std::string& filePath);

    /**
     * Loads an audio file from a given file path.
     *
     * @return true if the file was successfully loaded
     */
    bool load(const std::string& filePath);

    /**
     * A vector of vectors holding the audio samples for the WavCodec. You can
     * access the samples by channel and then by sample index.
     *
     * mSamples[channel][sampleIndex]
     */
    AudioBuffer mSamples;

private:

    AudioFileFormat determineAudioFileFormat(std::vector<uint8_t>& fileData);

    bool decodeWaveFile(std::vector<uint8_t>& fileData);

    /**
     * 寻找关键词的下标。
     *
     * @param source            原数据
     * @param chunkHeaderID     关键词
     * @param startIndex        起始点
     * @param endian            大小端
     * @return                  下标值，如果没有找到返回-1
     */
    int getIndexOfChunk(std::vector<uint8_t>& source, const std::string& chunkHeaderID, int startIndex,
                        Endian endian = Endian::LittleEndianess);

    /**
     * 将两个字节转化为整形数据。
     *
     * @param source        原数据
     * @param startIndex    起始点
     * @param endian        大小端
     * @return              整形数据
     */
    int16_t twoByteToInt(std::vector<uint8_t>& source, int startIndex,
                         Endian endian = Endian::LittleEndianess);

    /**
     * 将四个字节转化为整形数据。
     *
     * @param source        原数据　
     * @param startIndex    起始点
     * @param endian        大小端
     * @return              整形数据
     */
    int32_t fourBytesToInt(std::vector<uint8_t>& source, int startIndex,
                           Endian endian = Endian::LittleEndianess);

    AudioFileFormat mAudioFileFormat;
    uint32_t mSampleRate;
    int mBitDepth;
};


