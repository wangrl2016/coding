//
// Created by wangrl on 2021/4/8.
//

#pragma once

#include <string>

namespace sf {

    namespace priv {
        /**
         * High-level wrapper around the audio API, it manages
         * the creation and destruction of the audio device and
         * context and stores the device capabilities.
         */
        class AudioDevice {
            AudioDevice();

            ~AudioDevice();

            /**
             * Check if an OpenAL extension is supported.
             *
             * This function automatically finds whether it is an AL or ALC
             * extension, and calls the corresponding function.
             *
             * @param extension Name of the extension to test
             * @return True if the extension is supported, false if not
             */
            static bool isExtensionSupported(const std::string& extension);

            /**
             * Get the OpenAL format that matches the given number of channels.
             *
             * @param channelCount Number of channels
             * @return Corresponding format
             */
            static int getFormatFromChannelCount(unsigned int channelCount);

            /**
             * Change the global volume of all the sounds and musics.
             *
             * The volume is a number between 0 and 100; it is combined with
             * the individual volume of each sound/music.
             * The default value for the volume is 100 (maximum).
             *
             * @param volume New global volume, in the range[0, 100]
             */
            static void setGlobalVolume(float volume);

            /**
             * Get the current value of the global volume.
             *
             * @return Current global volume, in the range[0, 100]
             */
            static float getGlobalVolume();
        };
    }
}
