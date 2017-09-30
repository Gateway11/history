package com.rokid.openvoice;

import com.rokid.openvoice.entity.VoiceCommand;
import com.rokid.openvoice.entity.VoiceEvent;

import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

public final class VoiceCallback{

    private String TAG = getClass().getSimpleName();
    
    private Handler mHandler = null;
    
    public VoiceCallback(Handler mHandler){
    	this.mHandler = mHandler;
    }

    private void onVoiceEvent(int id, int event, double sl, double energy){
        Log.e(TAG, "onVoiceEvent");
        mHandler.obtainMessage(VoiceService.MSG_VOICE_EVENT, new VoiceEvent(id, event, sl, energy)).sendToTarget();
    }

    private void onIntermediateResult(int id, int type, String asr){
        Log.e(TAG, "onIntermediateResult");
        Message obtain = Message.obtain();
        obtain.what = VoiceService.MSG_INTERMEDIATE_RESULT;
        Bundle mBundle = new Bundle();
        mBundle.putInt("id", id);
        mBundle.putInt("type", type);
        mBundle.putString("asr", asr);
        obtain.setData(mBundle);
        mHandler.sendMessage(obtain);
    }

    private void onVoiceCommand(int id, String asr, String nlp, String action){
        Log.e(TAG, "id \t " + id);
        Log.e(TAG, "asr \t " + asr);
        Log.e(TAG, "nlp \t" + nlp);
        Log.e(TAG, "action \t" + action);
        mHandler.obtainMessage(VoiceService.MSG_VOICE_COMMAND, new VoiceCommand(id, asr, nlp, action)).sendToTarget();
    }

    private void onSpeechError(int id, int errcode){
        Log.e(TAG, "onSpeechError");
        mHandler.obtainMessage(VoiceService.MSG_SPEECH_ERROR, id, errcode).sendToTarget();
    }
}
