package com.android.simple.oboea;

import android.app.Activity;
import android.media.AudioManager;
import android.os.Build;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;
import android.widget.AdapterView;
import android.widget.TextView;

import androidx.core.view.MotionEventCompat;

public class MainActivity extends Activity {

    private static final String TAG = MainActivity.class.getName();
    private static final long UPDATE_LATENCY_EVERY_MILLIS = 1000;

    private AudioDeviceSpinner mPlaybackDeviceSpinner;

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        int action = MotionEventCompat.getActionMasked(event);
        switch (action) {
            case (MotionEvent.ACTION_DOWN):
                break;
            case (MotionEvent.ACTION_UP):
                break;
        }
        return super.onTouchEvent(event);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Example of a call to a native method
        // TextView tv = findViewById(R.id.sample_text);
        // tv.setText(PlaybackEngine.stringFromJNI());
        setupPlaybackDeviceSpinner();
    }

    @Override
    protected void onResume() {
        super.onResume();
        // PlaybackEngine.create(this);
    }

    @Override
    protected void onPause() {
        super.onPause();
    }

    private void setupPlaybackDeviceSpinner() {
        mPlaybackDeviceSpinner = findViewById(R.id.playbackDevicesSpinner);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            mPlaybackDeviceSpinner.setDirectionType(AudioManager.GET_DEVICES_OUTPUTS);
            mPlaybackDeviceSpinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
                @Override
                public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {

                }

                @Override
                public void onNothingSelected(AdapterView<?> parent) {

                }
            });
        }
    }
}
