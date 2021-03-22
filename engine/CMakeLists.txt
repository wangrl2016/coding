cmake_minimum_required(VERSION 3.15)

project(Engine)

# 增加googletest测试库
add_subdirectory(googletest)

include_directories(${CMAKE_SOURCE_DIR})

add_library(engine src/String.cpp)

add_executable(Test test/Test.cpp)
target_link_libraries(Test gtest_main engine)