//
// Created by wangrl on 2021/3/28.
//

#include "gtest/gtest.h"

#include "include/NonCopyable.h"


void func1(NonCopyable var1) {

}

NonCopyable func2() {
    NonCopyable var1;
    return var1;
}

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
}


TEST(RefCnt, RefCntBase) {  // NOLINT

}
