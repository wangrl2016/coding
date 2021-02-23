cmake_minimum_required(VERSION 3.15)

# set the project name
project(FFmpegTutorials)

# add the executable

add_executable(metadata Metadata.cpp)
target_link_libraries(metadata avformat avcodec swresample swscale avutil avfilter avdevice)
