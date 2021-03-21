cmake_minimum_required(VERSION 3.15)
project(CommandLine)

add_library(CommandLine CommandLineFlags.cpp)

add_executable(CommandTest CommandTest.cpp)
target_link_libraries(CommandTest CommandLine)