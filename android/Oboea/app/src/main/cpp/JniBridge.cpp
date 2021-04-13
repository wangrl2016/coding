#include <jni.h>
#include <string>
#include <memory>
#include <android/asset_manager_jni.h>

#include "LogMacros.h"
#include "HelloOboeEngine.h"
#include "Game.h"

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
    auto result = engine->start();
    if (result != oboe::Result::OK) {
        LOGE("Opening and starting stream failed. Returned %d", result);
        engine->stop();
        delete engine;
        return 0;
    }
    return reinterpret_cast<jlong>(engine);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_android_simple_oboea_PlaybackEngine_native_1deleteEngine(
        JNIEnv *env, jclass clazz,
        jlong engineHandle) {
    HelloOboeEngine *engine = reinterpret_cast<HelloOboeEngine *>(engineHandle);
    engine->stop();
    delete engine;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_android_simple_oboea_PlaybackEngine_native_1setToneOn(
        JNIEnv *env, jclass clazz,
        jlong engineHandle,
        jboolean isToneOn) {
    HelloOboeEngine *engine = reinterpret_cast<HelloOboeEngine *>(engineHandle);
    if (engine == nullptr) {
        LOGE("Engine handle is invalid, call createHandle() to create a new one");
        return;
    }
    engine->tap(isToneOn);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_android_simple_oboea_PlaybackEngine_native_1setAudioApi(
        JNIEnv *env, jclass clazz,
        jlong engineHandle,
        jint audioApi) {
    HelloOboeEngine *engine = reinterpret_cast<HelloOboeEngine *>(engineHandle);
    if (engine == nullptr) {
        LOGE("Engine handle is invalid, call createHandle() to create a new one");
        return;
    }

    oboe::AudioApi api = static_cast<oboe::AudioApi>(audioApi);
    engine->setAudioApi(api);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_android_simple_oboea_PlaybackEngine_native_1setAudioDeviceId(
        JNIEnv *env, jclass clazz,
        jlong engineHandle,
        jint deviceId) {
    HelloOboeEngine *engine = reinterpret_cast<HelloOboeEngine *>(engineHandle);
    if (engine == nullptr) {
        LOGE("Engine handle is invalid, call createHandle() to create a new one");
        return;
    }
    engine->setDeviceId(deviceId);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_android_simple_oboea_PlaybackEngine_native_1setChannelCount(
        JNIEnv *env, jclass clazz,
        jlong engineHandle,
        jint channelCount) {
    HelloOboeEngine *engine = reinterpret_cast<HelloOboeEngine *>(engineHandle);
    if (engine == nullptr) {
        LOGE("Engine handle is invalid, call createHandle() to create a new one");
        return;
    }
    engine->setChannelCount(channelCount);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_android_simple_oboea_PlaybackEngine_native_1setBufferSizeInBursts(
        JNIEnv *env,
        jclass clazz,
        jlong engineHandle,
        jint bufferSizeInBursts) {
    HelloOboeEngine *engine = reinterpret_cast<HelloOboeEngine *>(engineHandle);
    if (engine == nullptr) {
        LOGE("Engine handle is invalid, call createHandle() to create a new one");
        return;
    }
    engine->setBufferSizeInBursts(bufferSizeInBursts);
}

extern "C"
JNIEXPORT jdouble JNICALL
Java_com_android_simple_oboea_PlaybackEngine_native_1getCurrentOutputLatencyMillis(
        JNIEnv *env,
        jclass clazz,
        jlong engineHandle) {
    HelloOboeEngine *engine = reinterpret_cast<HelloOboeEngine *>(engineHandle);
    if (engine == nullptr) {
        LOGE("Engine is null, you must call createEngine before calling this method");
        return static_cast<jdouble>(-1.0);
    }
    return static_cast<jdouble>(engine->getCurrentOutputLatencyMillis());
}

extern "C"
JNIEXPORT jboolean JNICALL
Java_com_android_simple_oboea_PlaybackEngine_native_1isLatencyDetectionSupported(
        JNIEnv *env,
        jclass clazz,
        jlong engineHandle) {
    HelloOboeEngine *engine = reinterpret_cast<HelloOboeEngine *>(engineHandle);
    if (engine == nullptr) {
        LOGE("Engine is null, you must call createEngine before calling this method");
        return JNI_FALSE;
    }
    return (engine->isLatencyDetectionSupported() ? JNI_TRUE : JNI_FALSE);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_android_simple_oboea_PlaybackEngine_native_1setDefaultStreamValues(
        JNIEnv *env,
        jclass clazz,
        jint sampleRate,
        jint framesPerBurst) {
    oboe::DefaultStreamValues::SampleRate = (int32_t) sampleRate;
    oboe::DefaultStreamValues::FramesPerBurst = (int32_t) framesPerBurst;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_android_simple_oboea_PlaybackEngine_native_1stringFromJNI(JNIEnv *env, jclass clazz) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

// Rhythm Game

extern "C" {
std::unique_ptr<Game> game;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_android_simple_oboea_RhythmGameActivity_native_1onStart(JNIEnv *env, jobject thiz,
                                                                 jobject jAssetManager) {
    AAssetManager *assetManager = AAssetManager_fromJava(env, jAssetManager);
    if (assetManager == nullptr) {
        LOGE("Could not obtain the AAssetManager");
        return;
    }

    game = std::make_unique<Game>(*assetManager);
    game->start();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_android_simple_oboea_RhythmGameActivity_native_1onStop(JNIEnv *env, jobject thiz) {
    // TODO: implement native_onStop()
}

extern "C"
JNIEXPORT void JNICALL
Java_com_android_simple_oboea_RhythmGameActivity_native_1setDefaultStreamValues(JNIEnv *env,
                                                                                jclass clazz,
                                                                                jint sampleRate,
                                                                                jint framesPerBurst) {
    oboe::DefaultStreamValues::SampleRate = (int32_t) sampleRate;
    oboe::DefaultStreamValues::FramesPerBurst = (int32_t) framesPerBurst;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_android_simple_oboea_RendererWrapper_native_1onSurfaceCreated(JNIEnv *env, jclass clazz) {
    // TODO: implement native_onSurfaceCreated()
}

extern "C"
JNIEXPORT void JNICALL
Java_com_android_simple_oboea_RendererWrapper_native_1onSurfaceChanged(JNIEnv *env, jclass clazz,
                                                                       jint width_in_pixels,
                                                                       jint height_in_pixels) {
    // TODO: implement native_onSurfaceChanged()
}

extern "C"
JNIEXPORT void JNICALL
Java_com_android_simple_oboea_RendererWrapper_native_1onDrawFrame(JNIEnv *env, jclass clazz) {
    // TODO: implement native_onDrawFrame()
}

extern "C"
JNIEXPORT void JNICALL
Java_com_android_simple_oboea_GameSurfaceView_native_1onTouchInput(JNIEnv *env, jclass clazz,
                                                                   jint event_type,
                                                                   jlong time_since_boot_ms, jint x,
                                                                   jint y) {
    // TODO: implement native_onTouchInput()
}

extern "C"
JNIEXPORT void JNICALL
Java_com_android_simple_oboea_GameSurfaceView_native_1surfaceDestroyed(JNIEnv *env, jclass clazz) {
    // TODO: implement native_surfaceDestroyed()
}