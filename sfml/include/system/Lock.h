//
// Created by wangrl on 2021/4/6.
//

#pragma once

#include "include/system/NonCopyable.h"
#include "include/system/Mutex.h"

namespace sf {
    /**
     * sf::Lock is a RAII wrapper for sf::Mutex. By unlocking it in its destructor, it ensures that the
     * mutex will always be released when the current scope (most likely a function) ends.
     *
     * This is even more important when an exception or an early return statement can interrupt the
     * execution flow of the function.
     *
     * For maximum robustness, sf::Lock should always be used to lock/unlock a mutex.
     *
     * Usage example:
     *     sf::Mutex mutex;
     *     void function {
     *         sf::Lock lock(mutex);    // mutex is now locked
     *         functionThatMayThrowAnException();   // mutex is unlocked if this function throws.
     *         if (someCondition)
     *             return;  // mutex is unlocked
     *     }    // mutex is unlocked
     *
     * Because the mutex is not explicitly unlocked in the code, it may remain
     * locked longer that needed. If the region of the code that needs to be
     * protected by the mutex is not the entire function, a good practice is to
     * create a smaller, inner scope so that the lock is limited to this part
     * of the code.
     *
     * Usage example:
     *     sf::Mutex mutex;
     *     void function() {
     *         {
     *             sf::Lock lock(mutex);
     *             codeThatRequiresProtection();
     *         }    // mutex is unlocked here
     *         codeThatDeesNotCareAboutTheMutex();
     *     }
     *
     * Having a mutex locked longer than required is a bad practice which
     * can lead to bad performances. Don't forget that when a mutex is locked, other
     * threads may be waiting doing nothing until it is released.
     */
    class Lock : NonCopyable {
    public:
        /**
         * Construct the lock with a target mutex.
         * The mutex passed to sf::Lock is automatically locked.
         *
         * @param mutex Mutex to lock
         */
        explicit Lock(Mutex& mutex);

        /**
         * The destructor of sf::Lock automatically unlocks its mutex.
         */
        ~Lock();

    private:
        Mutex& mMutex;
    };

}