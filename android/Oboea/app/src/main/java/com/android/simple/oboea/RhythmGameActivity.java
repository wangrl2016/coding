package com.android.simple.oboea;

import androidx.appcompat.app.AppCompatActivity;

import android.content.res.AssetManager;
import android.os.Bundle;

public class RhythmGameActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_rhythm_game);
    }

    @Override
    protected void onResume() {
        super.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
    }

    private native void native_onStart(AssetManager assetManager);

    private native void native_onStop();

    private static native void native_setDefaultStreamValues(int defaultSampleRate,
                                                             int defaultFramesPerBurst);
}