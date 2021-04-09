//
// Created by wangrl on 21-4-9.
//

#pragma once

#include "TappableAudioSource.h"
#include "Oscillator.h"

/**
 * Generates a fixed frequency tone for each channel.
 * Implements RenderableTap (sound source with toggle) which is required for AudioEngines.
 */
class SoundGenerator : public TappableAudioSource {
    static constexpr size_t kSharedBufferSize = 1024;

public:
    SoundGenerator(int32_t sampleRate, int32_t channelCount);

    ~SoundGenerator() = default;

    SoundGenerator(SoundGenerator &&other) = default;

    SoundGenerator &operator=(SoundGenerator &&other) = default;

    // Switch the tones on.
    void tap(bool isOn) override;

    void renderAudio(float *audioData, int32_t numFrames) override;

private:
    std::unique_ptr<Oscillator[]> mOscillators;
    std::unique_ptr<float[]> mBuffer = std::make_unique<float[]>(kSharedBufferSize);
};