//
// Created by wangrl on 2021/3/15.
//

#include <cstdio>
#include <cstdint>
#include <cmath>

/**
 * 大小端的介绍
 *
 * https://www.toutiao.com/i6939835652913578499/
 *
 * FFmpeg中音视频有Packed和Planar两种存储模式。
 *
 * 对于双声道来说，Packed为两个声道的数据交错存储，Planar为两个声道分开存储。
 *
 * Packed: L R L R L R L R
 * Planar: L L L L R R R R
 *
 * Packed模式frame.data[0]包含所有的音频数据
 * Planar模式frame.data[i]表示第几声道的数据
 */


/**
 * The data describe by the sample format is always in native-endian order.
 *
 * For planar sample formats, each audio channel is in a speparate data plane,
 * and linesize is the buffer size, in bytes, for a single plane. All data
 * planes must be the same size. For packed sample formats, only the first data
 * plane is used, and samples for each channel are interleaved. In this case,
 * linesize is the buffer size, in bytes, for the 1 plane.
 */
enum AVSampleFormat {
    AV_SAMPLE_FMT_NONE = -1,
    AV_SAMPLE_FMT_U8,       // unsigned 8 bits
    AV_SAMPLE_FMT_S16,      // signed 16 bits
    AV_SAMPLE_FMT_S32,      // signed 32 bits
    AV_SAMPLE_FMT_FLT,      // float
    AV_SAMPLE_FMT_DBL,      // double

    AV_SAMPLE_FMT_U8P,      // unsigned 8 bits, planar
    AV_SAMPLE_FMT_S16P,     // signed 16 bits, planar
    AV_SAMPLE_FMT_S32P,     // signed 32 bits, planar
    AV_SAMPLE_FMT_FLTP,     // float, planar
    AV_SAMPLE_FMT_DBLP,     // double, planar
    AV_SAMPLE_FMT_S64,      // signed 64 bits
    AV_SAMPLE_FMT_S64P,     // signed 64 bits, planar

    AV_SAMPLE_FMT_NB        // number of sample formats. DO NOT USE if linking dynamically
};

/**
 * A channel layout is a 64-bits integer with a bit set for every channel.
 * The number of bits set must be equal to the number of channels.
 * The value 0 means that the channel layout is not known.
 */
#define AV_CH_FRONT_LEFT             0x00000001
#define AV_CH_FRONT_RIGHT            0x00000002
#define AV_CH_FRONT_CENTER           0x00000004
#define AV_CH_LOW_FREQUENCY          0x00000008
#define AV_CH_BACK_LEFT              0x00000010
#define AV_CH_BACK_RIGHT             0x00000020

#define AV_CH_LAYOUT_MONO           (AV_CH_FRONT_CENTER)
#define AV_CH_LAYOUT_STEREO         (AV_CH_FRONT_LEFT|AV_CH_FRONT_RIGHT)
#define AV_CH_LAYOUT_QUAD           (AV_CH_LAYOUT_STEREO|AV_CH_BACK_LEFT|AV_CH_BACK_RIGHT)


static int split_pcm_f32le(const char* pcmPath, const char* leftPath, const char* rightPath,
                           AVSampleFormat sampleFormat,
                           int channel) {
    FILE* pcmFile = fopen(pcmPath, "rb+");
    FILE* leftFile = fopen(leftPath, "wb");
    FILE* rightFile = fopen(rightPath, "wb");

    if (!pcmFile || !leftFile || !rightFile) {
        printf("Error open file\n");
        return -1;
    }


    char* sample = (char*) malloc(2 * sizeof(float));
    while (!feof(pcmFile)) {
        fread(sample, 2, sizeof(float), pcmFile);
        // L
        fwrite(sample, 1, sizeof(float), leftFile);
        // R
        fwrite(sample + sizeof(float), 1, sizeof(float), rightFile);
    }
    return 0;
}

int main(int argc, char** argv) {

    if (argc < 2) {
        printf("Need more argument");
        exit(EXIT_FAILURE);
    }

    int sampleRate = 44100;     // 采样率
    int numChannels = 1;    // 声道数
    char* sampleFormat = "s16le";
    char* outputFile = argv[1];
    int time = 5;
    int16_t q[sampleRate * time];
    for (int i = 0; i < sampleRate * time; i++)
        q[i] = sin(i) * 50000;

    FILE* file = fopen(outputFile, "wb");

    fwrite(q, sampleRate * time, sizeof(int16_t), file);

    // ffplay -f s16le -ac 1 -ar 44100 out.pcm
    printf("ffplay -f %s -ac %d -ar %d %s", sampleFormat, numChannels, sampleRate, outputFile);

    return 0;
}
