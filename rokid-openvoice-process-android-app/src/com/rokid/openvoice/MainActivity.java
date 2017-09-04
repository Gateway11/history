package com.rokid.openvoice;

import android.app.Activity;
import android.os.Bundle;

public class MainActivity extends Activity {
	
	private ConfigManager mConfigManager = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        mConfigManager = new ConfigManager(this);
        mConfigManager.install();
        
        VoiceManager.init();
        VoiceManager.networkStateChange(true);
    }
}




