package com.example.demo;

import android.app.Activity;
import android.os.Bundle;
import android.util.Log;

import com.rokid.voicerec.BearKidAdapter;
import com.rokid.voicerec.BearKidCallback;
import com.rokid.voicerec.CustomWord;

public class MainActivity extends Activity implements BearKidCallback {
	public static final String TAG = "MainActivity";
	BearKidAdapter bearKidAdapter = null;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		bearKidAdapter = new BearKidAdapter();
		bearKidAdapter.initialize(getApplicationContext(), "com.rokid.openvoice.VoiceService", this);
		Log.e(TAG, bearKidAdapter.addCustomWord(CustomWord.TYPE_VT_WORD, "大傻逼", "da4sha3bi1") + "           --------------------");
		Log.e(TAG, bearKidAdapter.queryCustomWord(-1).toString());
	}

	@Override
	public void onException(int arg0) {
		
	}

	@Override
	public void onIntermediateResult(String arg0) {
		
	}

	@Override
	public void onRecognizeResult(String arg0, String arg1, String arg2) {
		
	}

	@Override
	public void onVoiceEvent(int arg0, int arg1, double arg2) {
		
	}
}
