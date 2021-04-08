//
// Created by wangrl on 2021/4/6.
//

#pragma once

#include "include/system/NonCopyable.h"

namespace sf {
    namespace priv {
        class MutexImpl;
    }

    class Mutex : NonCopyable {
    public:
        Mutex();

        ~Mutex();

        void lock();

        void unlock();

    private:
        priv::MutexImpl* mMutexImpl;    // OS specific implementation
    };
}
