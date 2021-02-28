cmake_minimum_required(VERSION 3.15)

# set the project name
project(FFmpegTutorials)

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
        )

foreach (EXAMPLE ${EXAMPLES})
    add_executable(${EXAMPLE} ${EXAMPLE}.cpp)
    target_link_libraries(${EXAMPLE} avformat avcodec swresample
            swscale avutil avfilter avdevice)
endforeach ()
