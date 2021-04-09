package com.android.simple.oboea;

import android.media.AudioDeviceInfo;
import android.media.AudioManager;

import java.util.List;
import java.util.Objects;
import java.util.Vector;

/**
 * POJO which represents basic information for an audio device.
 * <p>
 * Example: id:8, deviceName: "build-in speaker"
 */
public class AudioDeviceListEntry {
    private int mId;
    private String mName;

    public AudioDeviceListEntry(int deviceId, String deviceName) {
        mId = deviceId;
        mName = deviceName;
    }

    public int getId() {
        return mId;
    }

    public String getName() {
        return mName;
    }

    public String toString() {
        return getName();
    }

    @Override
    public boolean equals(Object other) {
        if (this == other) return true;
        if (other == null || getClass() != other.getClass()) return false;
        AudioDeviceListEntry that = (AudioDeviceListEntry) other;
        if (mId != that.mId)
            return false;
        return Objects.equals(mName, that.mName);
    }

    @Override
    public int hashCode() {
        int result = mId;
        result = 31 * result + (mName != null ? mName.hashCode() : 0);
        return result;
    }

    /**
     * Create a list of AudioDeviceListEntry objects from a list of AudioDeviceInfo objects.
     *
     * @param devices       A list of {@link AudioDeviceInfo} objects
     * @param directionType Only audio devices with the direction will be inclued in the list.
     *                      Valid value are GET_DEVICES_ALL, GET_DEVICES_OUTPUTS and
     *                      GET_DEVICES_INPUTS.
     * @return A list of AudioDeviceListEntry objects
     */
    public static List<AudioDeviceListEntry> createListFrom(AudioDeviceInfo[] devices, int directionType) {
        List<AudioDeviceListEntry> listEntries = new Vector<>();
        for (AudioDeviceInfo info : devices) {
            if (directionType == AudioManager.GET_DEVICES_ALL ||
                    (directionType == AudioManager.GET_DEVICES_OUTPUTS && info.isSink()) ||
                    (directionType == AudioManager.GET_DEVICES_INPUTS && info.isSource())) {
                listEntries.add(new AudioDeviceListEntry(info.getId(), info.getProductName() + " " +
                        AudioDeviceInfoConverter.typeToString(info.getType())));
            }
        }
        return listEntries;
    }
}
