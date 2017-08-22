package com.rokid.openvoice;

import android.app.Activity;
import android.os.Bundle;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        VoiceNative.setup();
        VoiceNative.networkStateChange(true);
    }
}
//08-21 11:42:26.506: E/libOpenSLES(9216): pAudioSnk: channelMask=0x3 numChannels=4
//08-21 11:42:26.506: W/libOpenSLES(9216): Leaving Engine::CreateAudioRecorder (SL_RESULT_PARAMETER_INVALID)


