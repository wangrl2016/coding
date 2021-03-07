//
// Created by wangrl on 2021/3/7.
//

#include "WavFormat.h"

int main(int argc, char** argv) {

    WavFormat<float>* wav = new WavFormat<float>();
    wav->load("../../res/sample-wav-44k.wav");

    return 0;
}