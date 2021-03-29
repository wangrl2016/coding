//
// Created by wangrl on 2021/3/28.
//

#include <thread>

#include "gtest/gtest.h"

#include "include/Types.h"
#include "include/NonCopyable.h"
#include "include/RefCnt.h"
#include "include/WeakRefCnt.h"

// 测试变量在函数中传递函数
void func1(NonCopyable var1) {

}

// 测试变量返回函数
NonCopyable func2() {
    NonCopyable var1;
    return var1;
}

// 测试指针在函数中传递函数
void func3(NonCopyable* ptr1) {

}

TEST(RefCnt, NonCopyable) { // NOLINT
    // 调用默认构造器
    NonCopyable var1;

    // 无法调用NonCopyable(const NonCopyable&)
    // NonCopyable var2(var1);

    // 无法调用NonCopyable& operator=(const NonCopyable&)
    // NonCopyable var3;
    // var3 = var1;

    // 无法调用NonCopyable(const NonCopyable&)
    // func1(var1);

    // 调用默认构造器
    NonCopyable var4 = func2();

    // 无法调用NonCopyable(const NonCopyable&)
    // NonCopyable var5 = var1;

    // 调用NonCopyable(NonCopyable&&)
    NonCopyable var6 = std::move(var1);

    // 调用默认构造器
    NonCopyable var7;

    // 调用NonCopyable& operator=(NonCopyable&&)
    var7 = std::move(var6);

    // 调用NonCopyable(NonCopyable&&)
    func1(std::move(var7));

    auto* ptr1 = new NonCopyable();
    NonCopyable* ptr2 = ptr1;

    delete ptr1;

    // double free detected
    // delete ptr2;

    auto* ptr3 = new NonCopyable();

    NonCopyable* ptr4(ptr3);

    NonCopyable* ptr5 = ptr3;

    NonCopyable* ptr6;

    ptr6 = ptr3;

    func3(ptr6);

    delete ptr3;

    // double free detected
    // delete ptr4;
    // delete ptr5;
    // delete ptr6;
}

/**
 * https://en.cppreference.com/w/cpp/thread/thread
 *
 * The class thread represents a single thread of execution. Threads allow multiple
 * functions to execute concurrently.
 *
 * Threads begin execution immediately upon construction of the associated thread object,
 * starting at the top-level function provided as construction argument.
 */

void func4(int n) {
    for (int i = 0; i < 5; i++) {
        n++;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void func5(int& n) {
    for (int i = 0; i < 5; i++) {
        n++;
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

class Foo {
public:
    void bar() {
        for (int i = 0; i < 5; i++) {
            n++;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    int n = 0;
};

class Baz {
public:
    void operator()() {
        for (int i = 0; i < 5; i++) {
            ++n;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    int n = 0;
};

TEST(RefCnt, thread) {  // NOLINT
    int n = 0;
    Foo f;
    Baz b;
    // 线程构造
    std::thread t1; // t1 is not a thread
    std::thread t2(func4, n + 1);   // pass by value
    std::thread t3(func5, std::ref(n)); // pass by reference
    std::thread t4(std::move(t3));  // t4 is now running fun5(), t3 is no longer a thread
    std::thread t5(&Foo::bar, &f);  // t5 runs Foo::bar() on object f
    std::thread t6(b);  // t6 runs Baz::operator() on a copy of object b

    ASSERT_TRUE(t2.joinable());

    DEBUG_F("Thread id %ld\n", t4.get_id());

    t2.join();
    t4.join();
    t5.join();
    t6.join();

    ASSERT_EQ(5, n);
    ASSERT_EQ(5, f.n);
    ASSERT_EQ(0, b.n);

    // 检测joinable函数
    ASSERT_FALSE(t4.joinable());

}

/**
 * https://en.cppreference.com/w/cpp/atomic/atomic
 *
 * Each instantiation and full specification of the std::atomic template defines an
 * atomic type. If one thread writes to an atomic object while another thread reads
 * from it, the behavior is well-defined.
 */

struct Counters {
    int a;
    int b;
};

std::atomic<Counters> cnt;

TEST(RefCnt, atomic) {  // NOLINT

}

std::weak_ptr<int> wp;

TEST(RefCnt, WeakRefCnt) {      // NOLINT
    // C++标准中的强指针和弱指针
    // 弱指针需要转化为share_ptr进行调用
    {
        auto sp = std::make_shared<int>(42);
        wp = sp;

        ASSERT_EQ(wp.use_count(), 1);
        std::shared_ptr<int> swp = wp.lock();
        ASSERT_EQ(*swp, 42);
    }
    ASSERT_EQ(wp.use_count(), 0);

    auto* ref = new WeakRefCnt();
    ref->weakRef();
    ref->weakUnref();
}

TEST(RefCnt, virtual) { // NOLINT

}

