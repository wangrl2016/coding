//
// Created by wangrl on 21-4-15.
//

#pragma once

#include <cstdint>
#include <android/asset_manager.h>
#include "GameConstants.h"

class NDKExtractor {
public:
    static int32_t decode(AAsset *asset, uint8_t *targetData, AudioProperties targetProperties);
};
