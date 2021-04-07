//
// Created by wangrl on 2021/4/8.
//

#include "include/system/MutexImpl.h"

namespace sf {

    namespace priv {

        MutexImpl::MutexImpl() {
            // Make it recursive to follow the expected behavior.
            pthread_mutexattr_t attribute;
            pthread_mutexattr_init(&attribute);
            pthread_mutexattr_settype(&attribute, PTHREAD_MUTEX_RECURSIVE);
            pthread_mutex_init(&mMutex, &attribute);
        }

        MutexImpl::~MutexImpl() {
            pthread_mutex_destroy(&mMutex);
        }

        void MutexImpl::lock() {
            pthread_mutex_lock(&mMutex);
        }

        void MutexImpl::unlock() {
            pthread_mutex_unlock(&mMutex);
        }

    }
}