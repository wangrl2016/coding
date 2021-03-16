//
// Created by wangrl on 2021/3/16.
//

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

bool listContains(int valueToFind) {
    std::lock_guard<std::mutex> guard(someMutex);
    return std::find(someList.begin(), someList.end(), valueToFind) != someList.end();
}

int main(int argc, char** argv) {
    addToList(42);

    std::cout << "Contains(1) = " << listContains(1) << ", contains(42) = "
              << listContains(42) << std::endl;
    return 0;
}