//
// Created by wangrl on 21-4-16.
//

#include <chrono>

#include "UtilityFunctions.h"
#include "GameConstants.h"

int64_t nowUptimeMillis() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

void renderEvent(TapResult r){
    switch (r) {
        case TapResult::Success:
            SetGLScreenColor(kTapSuccessColor);
            break;
        case TapResult::Early:
            SetGLScreenColor(kTapEarlyColor);
            break;
        case TapResult::Late:
            SetGLScreenColor(kTapLateColor);
            break;
    }
}

