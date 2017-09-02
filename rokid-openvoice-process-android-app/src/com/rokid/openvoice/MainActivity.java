package com.rokid.openvoice;



import android.app.Activity;
import android.os.Bundle;

public class MainActivity extends Activity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        VoiceManager.setup();
        VoiceManager.networkStateChange(true);
    }
}
//09-02 09:18:51.549: W/speech.Connection(7327): websocket connect failed: No address found: apigwws.open.rokid.com, N4Poco3Net23NoAddressFoundExceptionE




