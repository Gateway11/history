package com.rokid.openvoice;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

public final class VoiceCallback {

	private String TAG = getClass().getSimpleName();

	private Handler mHandler = null;

	public VoiceCallback(Handler mHandler) {
		this.mHandler = mHandler;
	}

	private void onVoiceEvent(int id, int event, double sl, double energy) {
		Message obtain = Message.obtain();
		obtain.what = VoiceService.MSG_VOICE_EVENT;
		Bundle mBundle = new Bundle();
		mBundle.putInt("id", id);
		mBundle.putInt("event", event);
		mBundle.putDouble("sl", sl);
		mBundle.putDouble("energy", energy);
		obtain.setData(mBundle);
		mHandler.sendMessage(obtain);
	}

	private void onIntermediateResult(int id, int type, String asr) {
		Message obtain = Message.obtain();
		obtain.what = VoiceService.MSG_INTERMEDIATE_RESULT;
		Bundle mBundle = new Bundle();
		mBundle.putInt("id", id);
		mBundle.putInt("type", type);
		mBundle.putString("asr", asr);
		obtain.setData(mBundle);
		mHandler.sendMessage(obtain);
	}

	private void onVoiceCommand(int id, String asr, String nlp, String action) {
		Message obtain = Message.obtain();
		obtain.what = VoiceService.MSG_VOICE_COMMAND;
		Bundle mBundle = new Bundle();
		mBundle.putInt("id", id);
		mBundle.putString("asr", asr);
		mBundle.putString("nlp", nlp);
		mBundle.putString("action", action);
		obtain.setData(mBundle);
		mHandler.sendMessage(obtain);
	}

	private void onSpeechError(int id, int errcode) {
		mHandler.obtainMessage(VoiceService.MSG_SPEECH_ERROR, id, errcode).sendToTarget();
	}
}
