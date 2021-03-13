//
// Created by wangrl on 2021/3/13.
//

#include <vector>
#include <thread>
#include <algorithm>
#include <functional>

void doWork(unsigned id) {}

int main(int argc, char** argv) {
    std::vector<std::thread> threads;
    for (unsigned i = 0; i < 20; ++i)
        threads.push_back(std::thread(doWork, i));

    std::for_each(threads.begin(), threads.end(),
                  std::mem_fn(&std::thread::join));
    return 0;
}