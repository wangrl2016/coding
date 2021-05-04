cmake_minimum_required(VERSION 3.15)
include_guard()

project(AudioGraphNode)

set(AUDIO_GRAPH_NODE "${CMAKE_CURRENT_SOURCE_DIR}")
set(CMAKE_CXX_STANDARD 14)

include_directories(${AUDIO_GRAPH_NODE}/include)

set(SRC
        src/AudioFileReader.cpp
        )
add_library(AudioGraphNode ${SRC})

add_executable(MainExamples examples/MainExamples.cpp)
target_link_libraries(MainExamples AudioGraphNode)



