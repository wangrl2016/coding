//
// Created by wangrl on 2021/3/13.
//

#include <thread>

void doSomething(int& i) {
    ++i;
}

struct func {
    int& i;

    func(int& index) : i(index) {}

    void operator()() {
        for (int j = 0; j < 1000000; ++j)
            doSomething(i);
    }
};

void doSomethingInCurrentThread() {}

int main(int argc, char** argv) {

    int someLocalState = 0;
    func myFunc(someLocalState);
    std::thread t(myFunc);

    try {
        doSomethingInCurrentThread();
    } catch (...) {
        t.join();
        throw;
    }
    t.join();

    return 0;
}