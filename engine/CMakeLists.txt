cmake_minimum_required(VERSION 3.15)

project(Engine)

# 增加googletest测试库
# https://google.github.io/googletest/primer.html
add_subdirectory(googletest/)
include_directories(${CMAKE_SOURCE_DIR}/)

add_library(engine src/String.cpp include/Types.h include/NonCopyable.h src/Types.cpp)

add_executable(Test
        test/MathTest.cpp
        test/RefCntTest.cpp
        test/SortTest.cpp
        test/StringTest.cpp
        test/StdTest.cpp)
target_link_libraries(Test gtest_main engine)

add_executable(Con src/Types.cpp)