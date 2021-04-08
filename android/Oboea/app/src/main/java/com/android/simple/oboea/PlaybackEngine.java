package com.android.simple.oboea;

import android.content.Context;
import android.media.AudioManager;
import android.os.Build;

public class PlaybackEngine {
    static long mEngineHandle = 0;

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    static boolean create(Context context) {
        if (mEngineHandle == 0) {
            setDefaultStreamValues(context);
            mEngineHandle = native_createEngine();
        }
        return (mEngineHandle != 0);
    }

    private static void setDefaultStreamValues(Context context) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN_MR1) {
            AudioManager myAudioMgr = (AudioManager) context.getSystemService(Context.AUDIO_SERVICE);
            String sampleRateStr = myAudioMgr.getProperty(AudioManager.PROPERTY_OUTPUT_SAMPLE_RATE);
            int defaultSampleRate = Integer.parseInt(sampleRateStr);
            String framesPerBurstStr = myAudioMgr.getProperty(AudioManager.PROPERTY_OUTPUT_FRAMES_PER_BUFFER);
            int defaultFramesPerBurst = Integer.parseInt(framesPerBurstStr);

            native_setDefaultStreamValues(defaultSampleRate, defaultFramesPerBurst);
        }
    }

    static void delete() {
        if (mEngineHandle != 0) {
            native_deleteEngine(mEngineHandle);
        }
        mEngineHandle = 0;
    }

    static void setToneOn(boolean isToneOn) {

    }

    // Native methods
    private static native long native_createEngine();

    private static native void native_deleteEngine(long engineHandle);

    private static native void native_setToneOn(long engineHandle, int audioApi);

    private static native void native_setAudioApi(long engineHandle, int deviceId);

    private static native void native_setAudioDeviceId(long engineHandle, int deviceId);

    private static native void native_setChannelCount(long mEngineHandle, int channelCount);

    private static native void native_setBufferSizeInBursts(long engineHandle, int bufferSizeInBursts);

    private static native double native_getCurrentOutputLatencyMillis(long engineHandle);

    private static native boolean native_isLatencyDetectionSupported(long engineHandle);

    private static native void native_setDefaultStreamValues(int sampleRate, int framesPerBurst);
}
