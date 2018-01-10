package com.rokid.test;

import java.util.ArrayList;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;

import com.rokid.openvoice.R;
import com.rokid.openvoice.VoiceManager;

public class MainActivity extends Activity implements OnClickListener {
	
	String TAG = getClass().getSimpleName();

	private static boolean isMute = false;
	private static boolean network_connect = true;
	private static int mCurrentState = 2;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);

		setContentView(R.layout.activity_main);

		findViewById(R.id.init).setOnClickListener(this);
		findViewById(R.id.start_siren).setOnClickListener(this);
		findViewById(R.id.set_siren_state).setOnClickListener(this);
		findViewById(R.id.update_config).setOnClickListener(this);
		findViewById(R.id.network_state_change).setOnClickListener(this);
		findViewById(R.id.update_stack).setOnClickListener(this);
		findViewById(R.id.add_vt_word).setOnClickListener(this);
		findViewById(R.id.remove_vt_word).setOnClickListener(this);
		findViewById(R.id.get_vt_words).setOnClickListener(this);
	}
	/**
	 * am start --activity-single-top -n com.rokid.openvoice/com.rokid.test.MainActivity --es word "大傻逼" --es pinyin "da4sha3bi1"
	 */
	@Override
	protected void onNewIntent(Intent intent) {
		super.onNewIntent(intent);
		if(intent != null){
			String vt_word = intent.getStringExtra("word");
			String vt_pinyin = intent.getStringExtra("pinyin");
			Log.e(TAG, "--------------------------------  word : " + vt_word +", pinyin : " + vt_pinyin);
			if(vt_pinyin != null && vt_word != null){
				Log.e(TAG, "add vt word result : " + VoiceManager.addVtWord(new VoiceManager.VtWord(1, vt_word, vt_pinyin)));
			}
		}
	}

	@Override
	public void onClick(View arg0) {
		switch (arg0.getId()) {
		case R.id.init:
			VoiceManager.init();
			break;
		case R.id.start_siren:
			isMute = !isMute;
			VoiceManager.startSiren(isMute);
			break;
		case R.id.set_siren_state:
			VoiceManager.setSirenState(mCurrentState);
			break;
		case R.id.update_config:
			VoiceManager.updateConfig("device_id", "device_type_id", "key", "secret");
			VoiceManager.updateConfig(null, null, null, null);
			break;
		case R.id.network_state_change:
			VoiceManager.networkStateChange(network_connect);
			network_connect = !network_connect;
			break;
		case R.id.update_stack:
			VoiceManager.updateStack("curr_appid:prev_appid");
			VoiceManager.updateStack(null);
			break;
		case R.id.add_vt_word:
			Log.e(TAG, "add vt word result : " + VoiceManager.addVtWord(new VoiceManager.VtWord(1, "大傻逼", "da4sha3bi1")));
			Log.e(TAG, "add vt word result : " + VoiceManager.addVtWord(new VoiceManager.VtWord(1, "小傻逼", "xiao3sha3bi1")));
			break;
		case R.id.remove_vt_word:
			Log.e(TAG, "remove vt word result : " + VoiceManager.removeVtWord("大傻逼"));
			break;
		case R.id.get_vt_words:
			ArrayList<VoiceManager.VtWord> vtWords = VoiceManager.getVtWords();
			for (VoiceManager.VtWord vtWord : vtWords) {
				Log.e(TAG, vtWord.toString());
			}
			break;
		}
	}
}
