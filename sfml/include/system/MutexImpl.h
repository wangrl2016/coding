//
// Created by wangrl on 2021/4/8.
//

#pragma once

#include <pthread.h>
#include "include/system/NonCopyable.h"

namespace sf {
    namespace priv {
        class MutexImpl : NonCopyable {
        public:
            MutexImpl();

            ~MutexImpl();

            void lock();

            void unlock();

        private:
            pthread_mutex_t mMutex;
        };
    }
}
