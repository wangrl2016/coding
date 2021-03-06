cmake_minimum_required(VERSION 3.15)

# set the project name
project(FFmpegTutorials)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

include_directories(${CMAKE_SOURCE_DIR}/../tools)
add_subdirectory(${CMAKE_SOURCE_DIR}/../tools build)

# add the executable
SET(
        EXAMPLES
        DecodeVideo
        EncodeVideo
        EncodeAudio
        DecodeAudio
        ScaleVideo
        DemuxDecode
        FilteringVideo
        FilteringAudio
        TranscodeAAC
        Muxing
        Remuxing
        FFPlayer
)

foreach (EXAMPLE ${EXAMPLES})
    add_executable(${EXAMPLE} ${EXAMPLE}.cpp)
    target_link_libraries(${EXAMPLE}
            ${SDL2_LIBRARIES}
            avformat avcodec swresample
            swscale avutil avfilter avdevice
            CommandLine
            )
endforeach ()

