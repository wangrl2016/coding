cmake_minimum_required(VERSION 3.15)

project(CodecTutorials)


SET(EXAMPLES
        PBMFormat
        BitmapFormat
        WavCodec
        PcmCodec
        GifCodec
        Mp3Codec
        H264Codec
        )

foreach (EXAMPLE ${EXAMPLES})
    add_executable(${EXAMPLE} ${EXAMPLE}.cpp
            gif_decoder/GifImageReader.cpp)
endforeach ()


