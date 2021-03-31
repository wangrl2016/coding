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
            std::this_thread::sleep_for(
                    std::chrono::milliseconds(10));
        }
    }

    int n = 0;
};

class Baz {
public:
    void operator()() {
        for (int i = 0; i < 5; i++) {
            ++n;
            std::this_thread::sleep_for(
                    std::chrono::milliseconds(10));
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

TEST(RefCnt, atomic) {  // NOLINT

}

/**
 * 类型转换 static_cast
 *
 * 1. 用于类层次结构中，基类和子类之间指针和引用的转换
 * 2. 用于基础数据类型的转换，int转换为float
 * 3. 将void指针转化为目标类型的指针
 *
 * dynamic_cast 类型之间转换，比static_cast更安全
 *
 * const_cast 常量和非常量指针之间的转换
 */

static void bounceRef(void* data) {
    auto* ref = static_cast<RefCntBase*>(data);
    ref->ref();
    ref->unref();
}

static void bounceWeakRef(void* data) {
    auto* ref = static_cast<WeakRefCnt*>(data);
    ref->tryRef();
    ref->unref();
}

static void bounceWeakWeakRef(void* data) {
    auto* ref = static_cast<WeakRefCnt*>(data);
    ref->weakRef();
    ref->weakUnref();
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

    // 先调用父类构造器
    auto* ref1 = new WeakRefCnt();
    std::thread td1(bounceRef, ref1);
    std::thread td2(bounceRef, ref1);
    std::thread td3(bounceWeakRef, ref1);
    std::thread td4(bounceWeakWeakRef, ref1);

    td1.join();
    td2.join();
    td3.join();
    td4.join();

    assert(ref1->unique());
    ASSERT_EQ(ref1->getWeakCnt(), 1);

    // 先释放子类
    delete ref1;

    // 测试RefCntBase类
    auto* ref = new RefCntBase();
    std::thread td5(bounceRef, ref);
    std::thread td6(bounceRef, ref);
    td5.join();
    td6.join();

    ASSERT_TRUE(ref->unique());
    ref->unref();
}

static int gRefCounter;
static int gUnrefCounter;
static int gNewCounter;
static int gDeleteCounter;

class Effect {
public:
    Effect() : fRefCnt(1) {
        gNewCounter += 1;
    }

    virtual ~Effect() = default;

    int fRefCnt;

    void ref() {
        gRefCounter += 1;
        fRefCnt += 1;
    }

    void unref() {
        gUnrefCounter += 1;
        assert(fRefCnt > 0);
        if (0 == --fRefCnt) {
            gDeleteCounter += 1;
            delete this;
        }
    }

    static int* method() {
        return new int;
    }
};

static SharedPtr<Effect> Create() {
    return makeSharedPtr<Effect>();
}

class Paint {
public:
    SharedPtr<Effect> fEffect;

    const SharedPtr<Effect>& get() const {
        return fEffect;
    }

    void set(SharedPtr<Effect> value) {
        fEffect = std::move(value);
    }
};

struct EffectImpl : public Effect {
    ~EffectImpl() override = default;

    static SharedPtr<EffectImpl> Create() {
        return SharedPtr<EffectImpl>(new EffectImpl);
    }

    int fValue;
};

static SharedPtr<Effect> makeEffect() {
    auto foo = EffectImpl::Create();
    foo->fValue = 42;
    return std::move(foo);
}

static void resetCounters() {
    gRefCounter = 0;
    gUnrefCounter = 0;
    gNewCounter = 0;
    gDeleteCounter = 0;
}

TEST(RefCnt, SharedPtr) {   // NOLINT
    resetCounters();

    Paint paint;
    ASSERT_EQ(paint.fEffect.get(), nullptr);
    ASSERT_TRUE(!paint.get());

    
}

TEST(RefCnt, virtual) { // NOLINT

}
