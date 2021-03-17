//
// Created by wangrl on 2021/3/16.
//

/**
 * If all shared data is read-only, there's no problem, because the data read by
 * one thread is unaffected by whether or not another thread is reading the
 * same data.
 *
 * 删除双向链表的步骤
 *
 * 1. Identify then ode to delete: N.
 * 2. Update the link from the node prior to N to point to the node after N.
 * 3. Update the link from he node after N to point to the node prior to N.
 * 3. Delete node N.
 *
 * 条件竞争(Race conditions)
 *
 * In concurrency, a race condition is anything where the outcome depends on the
 * relative ordering of execution of operations on two or more threads.
 *
 * 锁 mutex (mutual exclusion)
 *
 * The most basic mechanism for protecting shared data provided by the C++ Standard
 * is the mutex.
 *
 * std::mutex类
 *
 * In C++, you create a mutex by constructing an instance of std::mutex, lock it
 * with a call to the lock() member function, and unlock it with a call to the
 * unlock() member function.
 *
 * std::lock_guard
 *
 * Instead, the Standard C++ library provide the std::lock_guard class template,
 * which implements that RAII idiom for the mutex. It locks the supplied mutex
 * on construction and unlocks it on destruction, ensuring a locked mutex is
 * always correctly unlocked.
 *
 * 不要返回引用和指针
 *
 * Any code that has access to that pointer or reference can noe access (and potentially
 * modify) the protected data without locking the mutex.
 */

#include <list>
#include <mutex>
#include <algorithm>
#include <iostream>

std::list<int> someList;
std::mutex someMutex;

void addToList(int newValue) {
    std::lock_guard<std::mutex> guard(someMutex);
    someList.push_back(newValue);
}

void reverseList() {
    someMutex.lock();
    someList.reverse();
    someMutex.unlock();
}

bool listContains(int valueToFind) {
    std::lock_guard<std::mutex> guard(someMutex);
    return std::find(someList.begin(), someList.end(), valueToFind) != someList.end();
}

int main(int argc, char** argv) {
    addToList(42);
    addToList(43);

    std::cout << "Contains(1) = " << listContains(1) << ", contains(42) = "
              << listContains(42) << std::endl;

    reverseList();
    std::all_of(someList.begin(), someList.end(), [](int i) {
        printf("%d ", i);
        return true;
    });

    return 0;
}
