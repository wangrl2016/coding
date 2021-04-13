package com.android.simple.oboea;

import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class RendererWrapper implements GLSurfaceView.Renderer {
    public static native void native_onSurfaceCreated();
    public static native void native_onSurfaceChanged(int widthInPixels, int heightInPixels);
    public static native void native_onDrawFrame();

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        native_onSurfaceCreated();
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        native_onSurfaceChanged(width, height);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        native_onDrawFrame();
    }
}
