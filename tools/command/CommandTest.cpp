//
// Created by wangrl on 2021/3/21.
//

#include <iostream>
#include "CommandLineFlags.h"

static DEFINE_string_s(input, i, "", "skottie animation to render");
static DEFINE_string_s(output, o, "", "mp4 file to create");
static DEFINE_string(assetPath, "", "path to assets needed for json file");
static DEFINE_int_s(fps, f, 23, "FPS");
static DEFINE_bool(loop, false, "Loop mode for profiling");
static DEFINE_bool_s(verbose, v, false, "Verbose mode");

int main(int argc, char** argv) {
    CommandLineFlags::SetUsage("Convert skottie to a mp4");
    CommandLineFlags::Parse(argc, argv);

    std::cout << FLAGS_loop << std::endl;
    std::cout << FLAGS_verbose << std::endl;
}
