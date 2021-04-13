package com.android.simple.oboea;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.SurfaceHolder;

public class GameSurfaceView extends GLSurfaceView {

    private final RendererWrapper mRenderer;

    public GameSurfaceView(Context context) {
        super(context);
        setEGLContextClientVersion(2);
        mRenderer = new RendererWrapper();
        // Set the Renderer for drawing on the GLSurfaceView.
        setRenderer(mRenderer);
    }

    public GameSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
        setEGLContextClientVersion(2);
        mRenderer = new RendererWrapper();
        // Set the Renderer for drawing on the GLSurfaceView.
        setRenderer(mRenderer);
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        native_surfaceDestroyed();
        super.surfaceDestroyed(holder);
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        // Motion Event reports input details from the touch screen.
        // and other input controls. In our case we care about DOWN events.
        switch (event.getAction()) {
            case MotionEvent.ACTION_DOWN:
                native_onTouchInput(0, event.getEventTime(),
                        (int) event.getX(), (int) event.getY());
                break;
        }
        return true;
    }

    public static native void native_onTouchInput(int eventType, long timeSinceBootMs,
                                                  int x, int y);

    public static native void native_surfaceDestroyed();
}
