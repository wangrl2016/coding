//
// Created by wangrl on 2021/3/13.
//

#include <thread>

class ScopedThread {
    std::thread t;
public:
    explicit ScopedThread(std::thread td) : t(std::move(t)) {
        if (!t.joinable())
            throw std::logic_error("No thread");
    }

    ~ScopedThread() {
        t.join();
    }

    ScopedThread(ScopedThread const&) = delete;

    ScopedThread& operator=(ScopedThread const&) = delete;
};

void doSomething(int& i) {
    ++i;
}

struct Func {
    int& i;

    Func(int& index) : i(index) {}

    void operator()() {
        for (unsigned j = 0; j < 1000000; ++j) {
            doSomething(i);
        }
    }
};

void doSomethingInCurrentThread() {}

int main(int argc, char** argv) {
    int someLocalState;
    ScopedThread t(std::thread(Func(someLocalState)));
    doSomethingInCurrentThread();
}
