//
// Created by wangrl on 2021/3/21.
//

#include "CommandLineFlags.h"

static DEFINE_string2(input,i, "", "skottie animation to render");
static DEFINE_string2(output, o, "", "mp4 file to create");
static DEFINE_string(assetPath, "", "path to assets needed for json file");

int main(int argc, char** argv) {
    CommandLineFlags::SetUsage("Convert skottie to a mp4");
    CommandLineFlags::Parse(argc, argv);
}