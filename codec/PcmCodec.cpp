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
 */


int main(int argc, char** argv) {
    int sampleRate = 44100;     // 采样率
    int numChannels = 1;    // 声道数
    char* sampleFormat = "s16le";
    char* outputFile = "out.pcm";
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
