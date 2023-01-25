package com.example.myapplication;

import com.google.firebase.database.IgnoreExtraProperties;

@IgnoreExtraProperties
public class Data {
    public String timestamp;
    public String temperature;
    public String humidity;
    public String lum;

    public Data() {
        // Default constructor required for calls to DataSnapshot.getValue(User.class)
    }

    public Data(String timestamp, String temperature, String humidity, String lum) {
        this.timestamp = timestamp;
        this.temperature = temperature;
        this.humidity = humidity;
        this.lum = lum;
    }
}

