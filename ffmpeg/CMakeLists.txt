cmake_minimum_required(VERSION 3.15)

# set the project name
project(FFmpegTutorials)

find_package(SDL2 REQUIRED)

# add the executable

SET(EXAMPLES
        Dump
        Metadata
        DecodeVideo
        EncodeVideo
        RGBA2Video
        EncodeAudio
        DecodeAudio
        ScaleVideo
        DemuxDecode
        FilteringVideo
        FilteringAudio
        TranscodeAAC
        Muxing
        Remuxing
        FFPlay
        )

foreach (EXAMPLE ${EXAMPLES})
    add_executable(${EXAMPLE} ${EXAMPLE}.cpp)
    target_link_libraries(${EXAMPLE}
            ${SDL2_LIBRARIES}
            avformat avcodec swresample
            swscale avutil avfilter avdevice)
endforeach ()

