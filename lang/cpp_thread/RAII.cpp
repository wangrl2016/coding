//
// Created by wangrl on 2021/3/13.
//

#include <thread>

class ThreadGuard {
    std::thread& t;
public:
    explicit ThreadGuard(std::thread& td) : t(td) {}

    ~ThreadGuard() {
        if (t.joinable())
            t.join();
    }

    ThreadGuard(ThreadGuard const&) = delete;

    ThreadGuard& operator=(ThreadGuard const&) = delete;
};

void doSomething(int& i) {
    ++i;
}

struct func {
    int& i;

    func(int& index) : i(index) {}

    void operator()() {
        for (int j = 0; j < 1000000; ++j) {
            doSomething(i);
        }
    }
};

void doSomethingInCurrentThread() {}

int main(int argc, char** argv) {

    int someLocalState;
    func myFunc(someLocalState);
    // The local objects are destroyed in reserve order of construction.
    std::thread t(myFunc);

    ThreadGuard g(t);

    doSomethingInCurrentThread();
    return 0;
}