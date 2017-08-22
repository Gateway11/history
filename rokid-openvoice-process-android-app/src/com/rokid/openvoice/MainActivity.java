package com.rokid.openvoice;

import android.app.Activity;
import android.os.Bundle;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        VoiceNative.setup();
        VoiceNative.startSiren(true);
//        VoiceNative.networkStateChange(true);
    }
}

//08-22 10:43:51.804: E/speech.Connection(5757): initialize ssl failed: Exception: /system/etc/roots.pem
//08-22 10:43:51.804: E/speech.Connection(5757): connect: init ssl failed
//08-22 10:43:51.804: I/speech.Connection(5757): connect to server failed, wait a while and retry



