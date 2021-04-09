#include <jni.h>
#include <string>

#include "LogMacros.h"
#include "HelloOboeEngine.h"

/**
 * Creates the audio engine
 *
 * @return a pointer to the audio engine. This should be passed to other methods
 */
extern "C"
JNIEXPORT jlong JNICALL
Java_com_android_simple_oboea_PlaybackEngine_native_1createEngine(JNIEnv *env, jclass clazz) {
    // We use std::nothrow so new returns a nullptr if the engine creation fails.
    HelloOboeEngine *engine = new(std::nothrow) HelloOboeEngine();
    if (engine == nullptr) {
        LOGE("Could not instantiate HelloOboeEngine");
        return 0;
    }
    return 0;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_android_simple_oboea_PlaybackEngine_native_1deleteEngine(JNIEnv *env, jclass clazz,
                                                                  jlong engine_handle) {
    // TODO: implement native_deleteEngine()
}

extern "C"
JNIEXPORT void JNICALL
Java_com_android_simple_oboea_PlaybackEngine_native_1setToneOn(JNIEnv *env, jclass clazz,
                                                               jlong engine_handle,
                                                               jboolean is_tone_on) {
    // TODO: implement native_setToneOn()
}

extern "C"
JNIEXPORT void JNICALL
Java_com_android_simple_oboea_PlaybackEngine_native_1setAudioApi(JNIEnv *env, jclass clazz,
                                                                 jlong engine_handle,
                                                                 jint audio_api) {
    // TODO: implement native_setAudioApi()
}

extern "C"
JNIEXPORT void JNICALL
Java_com_android_simple_oboea_PlaybackEngine_native_1setAudioDeviceId(JNIEnv *env, jclass clazz,
                                                                      jlong engine_handle,
                                                                      jint device_id) {
    // TODO: implement native_setAudioDeviceId()
}

extern "C"
JNIEXPORT void JNICALL
Java_com_android_simple_oboea_PlaybackEngine_native_1setChannelCount(JNIEnv *env, jclass clazz,
                                                                     jlong m_engine_handle,
                                                                     jint channel_count) {
    // TODO: implement native_setChannelCount()
}

extern "C"
JNIEXPORT void JNICALL
Java_com_android_simple_oboea_PlaybackEngine_native_1setBufferSizeInBursts(JNIEnv *env,
                                                                           jclass clazz,
                                                                           jlong engine_handle,
                                                                           jint buffer_size_in_bursts) {
    // TODO: implement native_setBufferSizeInBursts()
}

extern "C"
JNIEXPORT jdouble JNICALL
Java_com_android_simple_oboea_PlaybackEngine_native_1getCurrentOutputLatencyMillis(JNIEnv *env,
                                                                                   jclass clazz,
                                                                                   jlong engine_handle) {
    // TODO: implement native_getCurrentOutputLatencyMillis()
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_android_simple_oboea_PlaybackEngine_native_1isLatencyDetectionSupported(JNIEnv *env,
                                                                                 jclass clazz,
                                                                                 jlong engine_handle) {
    // TODO: implement native_isLatencyDetectionSupported()
}

extern "C"
JNIEXPORT void JNICALL
Java_com_android_simple_oboea_PlaybackEngine_native_1setDefaultStreamValues(JNIEnv *env,
                                                                            jclass clazz,
                                                                            jint sample_rate,
                                                                            jint frames_per_burst) {
    // TODO: implement native_setDefaultStreamValues()
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_android_simple_oboea_PlaybackEngine_native_1stringFromJNI(JNIEnv *env, jclass clazz) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
