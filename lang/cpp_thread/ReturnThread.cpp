//
// Created by wangrl on 2021/3/13.
//

#include <thread>

void someFunction() {}

void someOtherFunction(int) {}

std::thread f() {
    return std::thread(someFunction);
}

std::thread g() {
    std::thread t(someOtherFunction, 42);
    return t;
}

int main(int argc, char** argv) {
    std::thread t1 = f();
    t1.join();
    std::thread t2 = g();
    t2.join();
    return 0;
}