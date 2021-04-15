package com.android.simple.oboea;

import android.app.Activity;
import android.media.AudioManager;
import android.os.Build;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.SimpleAdapter;
import android.widget.Spinner;
import android.widget.TextView;

import androidx.core.view.MotionEventCompat;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Locale;
import java.util.Timer;
import java.util.TimerTask;

public class HelloOboeActivity extends Activity {

    private static final String TAG = HelloOboeActivity.class.getName();
    private static final long UPDATE_LATENCY_EVERY_MILLIS = 1000;

    private static final String[] AUDIO_API_OPTIONS = {"Unspecified", "OpenSL ES", "AAudio"};
    private static final Integer[] CHANNEL_COUNT_OPTIONS = {1, 2, 3, 4, 5, 5, 7, 8};
    // Default to Stereo (OPTIONS is zero-based array so index 1 = 2 channels)
    private static final int CHANNEL_COUNT_DEFAULT_OPTION_INDEX = 1;
    private static final int[] BUFFER_SIZE_OPTIONS = {0, 1, 2, 4, 8};
    // Default all other spinners to the first option on the list.
    private static final int SPINNER_DEFAULT_OPTION_INDEX = 0;

    private Spinner mAudioApiSpinner;
    private AudioDeviceSpinner mPlaybackDeviceSpinner;
    private Spinner mChannelCountSpinner;
    private Spinner mBufferSizeSpinner;
    private TextView mLatencyText;
    private Timer mLatencyUpdater;

    /**
     * Hook to user control to start / stop audio playback:
     * touch-down: start, and keeps on playing
     * touch-up: stop
     * simply pass the event to native side.
     */
    @Override
    public boolean onTouchEvent(MotionEvent event) {
        int action = MotionEventCompat.getActionMasked(event);
        switch (action) {
            case (MotionEvent.ACTION_DOWN):
                PlaybackEngine.setToneOn(true);
                break;
            case (MotionEvent.ACTION_UP):
                PlaybackEngine.setToneOn(false);
                break;
        }
        return super.onTouchEvent(event);
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mLatencyText = findViewById(R.id.latencyText);
        setupAudioApiSpinner();
        setupPlaybackDeviceSpinner();
        setupChannelCountSpinner();
        setupBufferSizeSpinner();
    }

    /**
     * Creating engine in onResume() and destroying in onPause() so the stream retains exclusive
     * mode only while in focus. This allows other apps to reclaim exclusive stream mode.
     */
    @Override
    protected void onResume() {
        super.onResume();
        PlaybackEngine.create(this);
        setupLatencyUpdater();
        // Return the spinner states to their default value.
        mChannelCountSpinner.setSelection(CHANNEL_COUNT_DEFAULT_OPTION_INDEX);
        mPlaybackDeviceSpinner.setSelection(SPINNER_DEFAULT_OPTION_INDEX);
        mBufferSizeSpinner.setSelection(SPINNER_DEFAULT_OPTION_INDEX);
        mAudioApiSpinner.setSelection(SPINNER_DEFAULT_OPTION_INDEX);
    }

    @Override
    protected void onPause() {
        if (mLatencyUpdater != null)
            mLatencyUpdater.cancel();
        PlaybackEngine.delete();
        super.onPause();
    }

    private void setupLatencyUpdater() {
        // Update the latency every 1s.
        TimerTask latencyUpdateTask = new TimerTask() {
            @Override
            public void run() {
                final String latencyStr;
                if (PlaybackEngine.isLatencyDetectionSupported()) {
                    double latency = PlaybackEngine.getCurrentOutputLatencyMillis();
                    if (latency >= 0) {
                        latencyStr = String.format(Locale.getDefault(), "%.2fms", latency);
                    } else {
                        latencyStr = "Unknown";
                    }
                } else {
                    latencyStr = "Only supported in AAudio (API 26+)";
                }

                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        mLatencyText.setText(getString(R.string.latency, latencyStr));
                    }
                });
            }
        };
        mLatencyUpdater = new Timer();
        mLatencyUpdater.schedule(latencyUpdateTask, 0, UPDATE_LATENCY_EVERY_MILLIS);
    }

    private void setupAudioApiSpinner() {
        mAudioApiSpinner = findViewById(R.id.audioApiSpinner);
        mAudioApiSpinner.setAdapter(new SimpleAdapter(
                this,
                createAudioApisOptionsList(),
                R.layout.audio_apis_spinner,    // the xml layout
                new String[]{getString(R.string.audio_api_description_key)}, // field to display
                new int[]{R.id.audioApiOption}  // view to show field in
        ));

        mAudioApiSpinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                PlaybackEngine.setAudioApi(position);
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {

            }
        });
    }

    private int getPlaybackDeviceId() {
        return ((AudioDeviceListEntry) mPlaybackDeviceSpinner.getSelectedItem()).getId();
    }

    private void setupPlaybackDeviceSpinner() {
        mPlaybackDeviceSpinner = findViewById(R.id.playbackDevicesSpinner);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            mPlaybackDeviceSpinner.setDirectionType(AudioManager.GET_DEVICES_OUTPUTS);
            mPlaybackDeviceSpinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
                @Override
                public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                    PlaybackEngine.setAudioDeviceId(getPlaybackDeviceId());
                }

                @Override
                public void onNothingSelected(AdapterView<?> parent) {

                }
            });
        }
    }

    private void setupChannelCountSpinner() {
        mChannelCountSpinner = findViewById(R.id.channelCountSpinner);
        ArrayAdapter<Integer> channelCountAdapter = new ArrayAdapter<>(this,
                R.layout.channel_counts_spinner, CHANNEL_COUNT_OPTIONS);
        mChannelCountSpinner.setAdapter(channelCountAdapter);
        mChannelCountSpinner.setSelection(CHANNEL_COUNT_DEFAULT_OPTION_INDEX);
        mChannelCountSpinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                PlaybackEngine.setChannelCount(CHANNEL_COUNT_OPTIONS[mChannelCountSpinner.getSelectedItemPosition()]);
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {

            }
        });
    }

    private int getBufferSizeInBursts() {
        HashMap<String, String> selectionOption = (HashMap<String, String>)
                mBufferSizeSpinner.getSelectedItem();

        String valueKey = getString(R.string.buffer_size_value_key);

        // parseInt will throw a NumberFormatException if the string doesn't contain a valid integer
        // representation. We don't need to worry about this because the values are derived from
        // the BUFFER_SIZE_OPTIONS int array.
        return Integer.parseInt(selectionOption.get(valueKey));
    }

    private void setupBufferSizeSpinner() {
        mBufferSizeSpinner = findViewById(R.id.bufferSizeSpinner);
        mBufferSizeSpinner.setAdapter(new SimpleAdapter(
                this,
                createBufferSizeOptionsList(),   // list of buffer size option
                R.layout.buffer_sizes_spinner,
                new String[]{getString(R.string.buffer_size_description_key)}, // field to display
                new int[]{R.id.bufferSizeOption}    // view to show field in
        ));

        mBufferSizeSpinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                PlaybackEngine.setBufferSizeInBursts(getBufferSizeInBursts());
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {

            }
        });
    }

    private List<HashMap<String, String>> createAudioApisOptionsList() {
        ArrayList<HashMap<String, String>> audioApiOptions = new ArrayList<>();
        for (int i = 0; i < AUDIO_API_OPTIONS.length; i++) {
            HashMap<String, String> option = new HashMap<>();
            option.put(getString(R.string.buffer_size_description_key), AUDIO_API_OPTIONS[i]);
            option.put(getString(R.string.buffer_size_value_key), String.valueOf(i));
            audioApiOptions.add(option);
        }
        return audioApiOptions;
    }

    /**
     * Creates a list of buffer size options which can be used to populate a SimpleAdapter.
     * Each option has a description and a value. The description is always equal to the value,
     * except when the value is zero as this indicates that the buffer size should be set
     * automatically by the audio engine.
     *
     * @return list of buffer size options
     */
    private List<HashMap<String, String>> createBufferSizeOptionsList() {
        ArrayList<HashMap<String, String>> bufferSizeOptions = new ArrayList<>();

        for (int i : BUFFER_SIZE_OPTIONS) {
            HashMap<String, String> option = new HashMap<>();
            String strValue = String.valueOf(i);
            String description = (i == 0) ? getString(R.string.automatic) : strValue;
            option.put(getString(R.string.buffer_size_description_key), strValue);
            option.put(getString(R.string.buffer_size_value_key), strValue);
            bufferSizeOptions.add(option);
        }
        return bufferSizeOptions;
    }
}
