//
// Created by wangrl2016 on 2021/5/6.
//

#pragma once

#include <cstddef>

namespace agn {
    namespace vm {
        void VectorMathMul(const float* source, int sourceStride, const float* scale, float* dest,
                           int destStride, int framesToProcess);
    }
}
