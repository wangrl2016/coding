//
// Created by wangrl2016 on 2021/5/4.
//

#pragma once

#include "include/core/AudioChannel.h"

namespace agn {
    class AudioBus {
    public:
        // Allocate indicates whether or not to initially have the AudioChannels created with managed storage.
        // Normal usage is to pass true here, in which case the AudioChannels will memory-manage their own storage.
        // If allocate is false then setChannelMemory() has to be called later on for each channel before the AudioBus is usable.
        AudioBus(int numberOfChannels, int length, bool allocate = true);

    protected:
        AudioBus() = default;

        std::vector<std::unique_ptr<AudioChannel>> mChannels;

        int mLength = 0;
    };
}
