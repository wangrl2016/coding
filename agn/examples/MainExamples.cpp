//
// Created by wangrl2016 on 2021/5/4.
//

#include <cstdlib>

#include "Examples.h"

static constexpr int kIterations = 1;

// A simple memory manager for the examples, as they are too large to
// instantiate all at once on the stack on small machines.
template<typename T>
class Example {
public:
    T* ex;

    Example() { ex = new T; }

    ~Example() { delete ex; }
};

int main(int argc, char** argv) {
    Example<ExSimple> simple;

    return EXIT_SUCCESS;
}
