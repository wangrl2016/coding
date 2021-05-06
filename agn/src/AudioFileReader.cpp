//
// Created by wangrl2016 on 2021/5/4.
//

#include <memory>
#include <mutex>

#include "AudioBus.h"
#include "AudioFileReader.h"

#include "libnyquist/Decoders.h"

namespace detail {
    std::shared_ptr<agn::AudioBus> loadInternal(nqr::AudioData* audioData, bool mixToMono) {
        int numSamples = static_cast<int>(audioData->samples.size());
        if (!numSamples)
            return nullptr;

        int length = int(numSamples / audioData->channelCount);
        const int busChannelCount = mixToMono ? 1 : (audioData->channelCount);

        std::vector<float> planarSamples(numSamples);

        // Create AudioBus where we'll put the PCM audio data.
        std::shared_ptr<agn::AudioBus> audioBus(new agn::AudioBus(busChannelCount, length));
    }
}

namespace agn {
    nqr::NyquistIO nyquistIO;
    std::mutex gFileIOMutex;
    std::shared_ptr<AudioBus> makeBusFromFile(const char* filePath, bool mixToMono) {
        std::lock_guard<std::mutex> lock(gFileIOMutex);
        auto* audioData = new nqr::AudioData();
        try {
            nyquistIO.Load(audioData, std::string(filePath));
        } catch (...) {
            // Use empty pointer as load failure sentinel.
            return {};
        }
        return detail::loadInternal(audioData, mixToMono);
    }

    std::shared_ptr<AudioBus> makeBusFromFile(const std::string& path, bool mixToMono) {
        return makeBusFromFile(path.c_str(), mixToMono);
    }
}
