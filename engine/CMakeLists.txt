cmake_minimum_required(VERSION 3.15)

project(Engine)

include_directories(${CMAKE_SOURCE_DIR})

add_library(engine src/String.cpp)