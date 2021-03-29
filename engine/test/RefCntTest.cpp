//
// Created by wangrl on 2021/3/28.
//

#include <thread>
#include "gtest/gtest.h"
#include "include/RefCnt.h"

/**
 * https://en.cppreference.com/w/cpp/atomic/atomic
 *
 * Each instantiation and full specialization of the std::atomic template defines an aotmic type.
 * If one thread writes to an atomic object while another thread reads from it, the behavior
 * is well-defined.
 */

static long total = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

static void* SimpleAdd(void*) {
    for (long i = 0; i < 10000000; i++) {
        total++;
    }
    return nullptr;
}

static void* MutexAdd(void*) {
    for (long i = 0; i < 10000000; i++) {
        // 使用pthread_mutex_x互斥锁来保证两个线程互斥地访问total，从而得到正确的结果。
        pthread_mutex_lock(&mutex);
        total++;
        pthread_mutex_unlock(&mutex);
    }
    return nullptr;
}

std::atomic<long> atomicTotal{0};

static void AtomicAdd() {
    for (long i = 0; i < 10000000; i++)
        atomicTotal++;
}

TEST(std, atomic) {
    // Linux平台进程
    pthread_t thread1, thread2;
    pthread_create(&thread1, nullptr, &SimpleAdd, nullptr);
    pthread_create(&thread2, nullptr, &SimpleAdd, nullptr);
    pthread_join(thread1, nullptr);
    pthread_join(thread2, nullptr);
    ASSERT_NE(total, 20000000);

    total = 0;
    pthread_create(&thread1, nullptr, &MutexAdd, nullptr);
    pthread_create(&thread1, nullptr, &MutexAdd, nullptr);
    pthread_join(thread1, nullptr);
    pthread_join(thread2, nullptr);
    ASSERT_EQ(total, 20000000);

    std::thread t1(AtomicAdd);
    std::thread t2(AtomicAdd);
    t1.join();
    t2.join();
    ASSERT_EQ(atomicTotal, 20000000);

}