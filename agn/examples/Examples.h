//
// Created by wangrl2016 on 2021/5/4.
//

#pragma once

#include <algorithm>
#include <random>
#include <string>
#include <vector>

#include "AudioBus.h"
#include "AudioFileReader.h"

struct ExampleInterface {
    std::mt19937 randomGenerator;

    virtual void play(int argc, char** argv) = 0;

    inline std::vector<std::string> splitCommandLine(int argc, char** argv) {
        // Takes a string, and separates out according to embedded quoted strings.
        // The quotes are preserved, and quotes are escaped.
        // examples
        // * abc            > abc
        // * abc "def"      > abc, "def"
        // * a "def" ghi    > a, "def", ghi
        // * a\"bc          > a\"bc
        auto separate = [](const std::string& input) -> std::vector<std::string> {
            std::vector<std::string> output;

            size_t curr = 0;
            size_t start = 0;
            size_t end = input.length();
            bool inQuotes = false;

            while (curr < end) {
                if (input[curr] == '\\') {
                    curr++;
                    if (curr != end && input[curr] == '\"')
                        curr++;
                } else {
                    if (input[curr] == '\"') {
                        // No empty string if not in quotes, otherwise preserve it.
                        if (inQuotes || start != curr) {
                            output.push_back(input.substr(start - (inQuotes ? 1 : 0),
                                                          curr - start + (inQuotes ? 2 : 0)));
                        }
                        inQuotes = !inQuotes;
                        start = curr + 1;
                    }
                    curr++;
                }
            }

            // Catch the case of a trailing substring that was not quoted, or a completely unquoted string.
            if (curr - start > 0)
                output.push_back(input.substr(start, curr - start));
            return output;
        };

        // Join the command line together so quoted strings can be found.
        std::string cmd;
        for (int i = 1; i < argc; i++) {
            if (i > 1)
                cmd += " ";
            cmd += std::string(argv[i]);
        }

        // Separate the command line, respecting quoted strings.
        std::vector<std::string> result = separate(cmd);
        result.insert(result.begin(), std::string{argv[0]});
        return result;
    }

    inline std::shared_ptr<agn::AudioBus> makeBusFromSampleFile(const char* const name, int argc, char** argv) {
        std::string pathPrefix;
        auto cmds = splitCommandLine(argc, argv);
        if (cmds.size() > 1)
            pathPrefix = cmds[1] + "/"; // cmds[0] is the path to the exe

        const std::string path = pathPrefix + name;
        std::shared_ptr<agn::AudioBus> bus = agn::makeBusFromFile(path, false);
        if (!bus)
            throw std::runtime_error("Could not open " + path);
        return bus;
    }
};

struct ExSimple : public ExampleInterface {
    void play(int argc, char** argv) final {

        auto musicClip = makeBusFromSampleFile("samples/stereo-music-clip.wav", argc, argv);
    }
};
