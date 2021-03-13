//
// Created by wangrl on 2021/3/13.
//
#include <thread>
#include <numeric>
#include <algorithm>
#include <functional>
#include <vector>
#include <iostream>

/**
 * std::ref的介绍
 *
 * https://www.cnblogs.com/wangshaowei/p/8874518.html
 *
 * 在函数式编程(std::bind)使用时，是对参数的直接拷贝，而不是引用。
 *
 * void f(int& n1, int& n2, const int& n3);
 *
 * std::function<void()> bound_f = std::bind(f, n1, std::ref(n2), std::cref(n3));
 */

template<typename Iterator, typename T>
struct accumulateBlock {
    void operator()(Iterator first, Iterator last, T& result) {
        result = std::accumulate(first, last, result);
    }
};

template<typename Iterator, typename T>
T parallelAccumulate(Iterator first, Iterator last, T init) {
    unsigned long const length = std::distance(first, last);
    if (!length)
        return init;

    unsigned long const minPerThread = 25;
    unsigned long const maxThreads =
            (length + minPerThread - 1) / minPerThread;

    unsigned long const hardwareThreads = std::thread::hardware_concurrency();

    unsigned long const numThreads =
            std::min(hardwareThreads != 0 ? hardwareThreads : 2, maxThreads);

    unsigned long const blockSize = length / numThreads;

    std::vector<T> results(numThreads);
    std::vector<std::thread> threads(numThreads - 1);

    Iterator blockStart = first;
    for (unsigned long i = 0; i < numThreads - 1; ++i) {
        Iterator blockEnd = blockStart;
        std::advance(blockEnd, blockSize);
        threads[i] = std::thread(
                accumulateBlock<Iterator, T>(),
                blockStart, blockEnd, std::ref(results[i]));
        blockStart = blockEnd;
    }

    accumulateBlock<Iterator, T>()(blockStart, last, results[numThreads - 1]);
    std::for_each(threads.begin(), threads.end(),
                  std::mem_fn(&std::thread::join));

    return std::accumulate(results.begin(), results.end(), init);
}

int main(int argc, char** argv) {
    std::vector<int> vi;

    for (int i = 0; i < 1000; ++i)
        vi.push_back(i);

    int sum = parallelAccumulate(vi.begin(), vi.end(), 5);
    std::cout << "sum = " << sum << std::endl;
    return 0;
}