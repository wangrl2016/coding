//
// Created by wangrl on 2021/4/6.
//

#pragma once

#include "include/system/NonCopyable.h"

namespace sf {
    class Mutex : NonCopyable {
    public:
        Mutex();

        ~Mutex();

        void lock();

        void unlock();
    };
}