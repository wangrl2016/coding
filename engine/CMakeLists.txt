cmake_minimum_required(VERSION 3.15)

project(Engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 增加googletest测试库
# https://google.github.io/googletest/primer.html
add_subdirectory(googletest/)
include_directories(${CMAKE_SOURCE_DIR}/)

add_library(engine
        src/Data.cpp
        src/String.cpp
        src/Types.cpp)

add_executable(Test
        test/JsonTest.cpp
        test/MathTest.cpp
        test/RefCntTest.cpp
        test/SortTest.cpp
        test/StringTest.cpp
        test/StdTest.cpp

        # std lib test
        test/VectorTest.cpp
        )
target_link_libraries(Test gtest_main engine)
