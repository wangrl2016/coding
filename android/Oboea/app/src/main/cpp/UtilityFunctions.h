//
// Created by wangrl on 21-4-16.
//

#pragma once


#include <cstdint>

constexpr int64_t kMillisecondsInSecond = 1000;
constexpr int64_t kNanosecondsInMillisecond = 1000000;

enum class TapResult {
    Early,
    Late,
    Success
};

int64_t nowUptimeMillis();

constexpr int64_t convertFramesToMillis(const int64_t frames, const int sampleRate) {
    // 帧数 / 采样率 = 时间
    return static_cast<int64_t>((static_cast<double>(frames) / sampleRate) * kMillisecondsInSecond);
}

// 设置屏幕的颜色
void renderEvent(TapResult r);
