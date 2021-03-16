//
// Created by wangrl on 2021/3/16.
//

#include <string>
#include <mutex>

/**
 * Don't pass pointers and references to protected data outside the scope of the lock,
 * whether by returning them from a function, storing them in externally visible
 * memory, or passing them as arguments to user-supplied functions.
 */

class SomeData {
    int a;
    std::string b;
public:
    void doSomething() {}
};

class DataWrapper {
private:
    SomeData data;
    std::mutex m;

public:
    template<typename Function>
    void processData(Function func) {
        std::lock_guard<std::mutex> l(m);
        func(data);
    }
};

SomeData* unprotected;

void maliciousFunction(SomeData& protectedData) {
    unprotected = &protectedData;
}

DataWrapper x;

int main(int argc, char** argv) {
    x.processData(maliciousFunction);
    unprotected->doSomething();
    return 0;
}