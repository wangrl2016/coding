//
// Created by wangrl on 2021/3/7.
//

/**
 * 协议
 *
 * http://soundfile.sapp.org/doc/WaveFormat/
 *
 * The WAVE file format is a subset of Microsoft's RIFF specification for the storage of
 * multimedia files. A RIFF file starts out with a file header followed by a sequence of
 * data chunks.
 *
 * A WAVE file is often just a RIFF file with a single "WAVE" chunk which consists of two
 * sub-chunks -- a "fmt" chunk specifying the data format and a "data" chunk containing
 * the actual sample data.
 */

#pragma once

#include <vector>

template <class T> class WavFormat {
public:
    typedef std::vector<std::vector<T>> AudioBuffer;

    WavFormat();

    // Constructor, using a given file path to load a file.
    WavFormat(std::string filePath);

    /**
     * Loads an audio file from a given file path.
     *
     * @return true if the file was successfully loaded
     */
    bool load(std::string filePath);
};


