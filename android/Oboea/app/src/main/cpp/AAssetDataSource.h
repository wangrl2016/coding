//
// Created by wangrl on 21-4-13.
//

#pragma once

#include <memory>
#include <android/asset_manager.h>
#include "DataSource.h"

class AAssetDataSource : public DataSource {
public:
    int64_t getSize() const override {
        return mBufferSize;
    }

    AudioProperties getProperties() const override {
        return mProperties;
    }

    const float *getData() const override {
        return mBuffer.get();
    }

    static AAssetDataSource *newFromCompressedAsset(
            AAssetManager &aAssetManager,
            const char *filename,
            AudioProperties targetProperties);

private:
    const std::unique_ptr<float[]> mBuffer;
    const int64_t mBufferSize;
    const AudioProperties mProperties;
};
