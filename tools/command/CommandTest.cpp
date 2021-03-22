//
// Created by wangrl on 2021/3/21.
//

#include <iostream>
#include "CommandLineFlags.h"


static DEFINE_bool(loop, false, "Loop mode for profiling");
static DEFINE_bool_s(verbose, v, false, "Verbose mode");
static DEFINE_int(width, 1280, "Animation width");
static DEFINE_int(height, 720, "Animation height");
static DEFINE_int_s(fps, f, 24, "FPS");

static DEFINE_double(seek, 5.0, "Animation seek time");

static DEFINE_string(assetPath, "", "path to assets needed for json file");
static DEFINE_string_s(input, i, "", "skottie animation to render");
static DEFINE_string_s(output, o, "", "mp4 file to create");
static DEFINE_extended_string(audio, "", "Audio path", "The Animation audio path");


int main(int argc, char** argv) {
    CommandLineFlags::SetUsage("Convert skottie to a mp4");
    CommandLineFlags::Parse(argc, argv);

    std::cout << FLAGS_loop << std::endl;
    std::cout << FLAGS_verbose << std::endl;
}
