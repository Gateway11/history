package com.rokid.openvoice;

import android.content.Intent;
import android.content.Context;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.net.NetworkInfo;
import android.net.ConnectivityManager;

import org.json.JSONObject;
import org.json.JSONException;

//import rokid.tts.Tts;

public class VoiceService extends android.app.Service {

    String TAG = getClass().getSimpleName();

    public VoiceNative mVoiceNative = null;
    public static MainHandler mHandler = null;

    public static final int MSG_REINIT = 0;
    public static final int MSG_TIMEOUT = 1;

    public static boolean initialized = false;

    public static final int SPEECH_TIMEOUT = 3;
    public static final int SERVICE_UNAVAILABLE = 6;

    public static final int SIREN_STATE_AWAKE = 1;
    public static final int SIREN_STATE_SLEEP = 2;

    private static final int DELAY = 15 * 1000;

    private static final int EVENT_VAD_ATART = 100;
    private static final int EVENT_VAD_DATA = 101;
    private static final int EVENT_VAD_END = 102;
    private static final int EVENT_VAD_CANCEL = 103;
    private static final int EVENT_WAKE_NOCMD = 108;
    private static final int EVENT_WAKE_CMD = 109;
    private static final int EVENT_SLEEP = 111;

    class MainHandler extends Handler {

        public void handleMessage(Message msg) {
            switch(msg.what) {
            case MSG_REINIT:
                handleReinit();
                break;
            case MSG_TIMEOUT:
                handleTimeout();
                break;
            }
        }
    }

    public VoiceService() {
        Log.e(TAG, "VoiceService  created ");
        mVoiceNative = VoiceNative.asInstance();
        mVoiceNative.init();
        mVoiceNative.registCallback(callback);
        initialized = true;
    }

    @Override
    public void onCreate() {
        mHandler = new MainHandler();
        ConnectivityManager cm = (ConnectivityManager)getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo mNetworkInfo = cm.getActiveNetworkInfo();
        if(mNetworkInfo != null) {
            mVoiceNative.networkStateChange(true);
        }
        try{
            mUEventObserver.startObserving("/sound/card1/pcmC1D0c");
        }catch(Exception e){
            e.printStackTrace();
        }

//        Tts _tts = new Tts();
//        if(_tts.speak("测试", null) <= 0){
//            //tts speak faild
//        }
    }

    private void handleReinit() {
        Log.e(TAG, "+++++++++++++++++++++REINITT+++++++++++++++++++++");
        mVoiceNative = VoiceNative.asInstance();
        mVoiceNative.init();
        mVoiceNative.registCallback(callback);
        initialized = true;
        ConnectivityManager cm = (ConnectivityManager)getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo mNetworkInfo = cm.getActiveNetworkInfo();
        if(mNetworkInfo != null) {
            mVoiceNative.networkStateChange(true);
        }
    }

    private void handleTimeout(){
        mVoiceNative.setSirenState(SIREN_STATE_SLEEP);
    }

    private final IVoiceCallback.Stub callback = new IVoiceCallback.Stub() {

        @Override
        public void onVoiceCommand(String asr, String nlp, String action) {
            Log.e(TAG, "asr\t" + asr);
            Log.e(TAG, "nlp\t" + nlp);
            Log.e(TAG, "action " + action);
            String appId = "";
            try{
                appId = new JSONObject(nlp).getString("appId");
            }catch(JSONException e){
                e.printStackTrace();    
            }
            if(appId != null && appId.length() > 0 && !appId.equals("ROKID.EXCEPTION")){
    		    mVoiceNative.updateStack(appId + ":");
            } 
            mHandler.removeMessages(MSG_TIMEOUT);
            mHandler.sendEmptyMessageDelayed(MSG_TIMEOUT, DELAY);
            mVoiceNative.setSirenState(SIREN_STATE_AWAKE);
        }

        @Override
        public void onVoiceEvent(int event, boolean has_sl, double sl, double energy, double threshold) {
            Log.e(TAG, event + " ,has_sl : " + has_sl + " ,sl : " + sl);
            if(event == EVENT_VAD_ATART) {
    
            } else if(event == EVENT_VAD_END) {
                mVoiceNative.setSirenState(SIREN_STATE_SLEEP);
            }
        }

        @Override
        public void onArbitration(String extra) {
            if("accept".equals(extra)){
                mHandler.removeMessages(MSG_TIMEOUT);
                mHandler.sendEmptyMessageDelayed(MSG_TIMEOUT, DELAY);
                mVoiceNative.setSirenState(SIREN_STATE_AWAKE);
            }
        }

        @Override
        public void onSpeechError(int errcode) {
            if(errcode == SPEECH_TIMEOUT){
                mHandler.removeMessages(MSG_TIMEOUT);
                mHandler.sendEmptyMessageDelayed(MSG_TIMEOUT, DELAY);
                mVoiceNative.setSirenState(SIREN_STATE_AWAKE);
            }
        }
    };

    private final android.os.UEventObserver mUEventObserver = new android.os.UEventObserver() {

        @Override
        public void onUEvent(android.os.UEventObserver.UEvent event) {
            Log.e(TAG, event.toString());
            if(initialized) {
                String action = event.get("ACTION");
                if("add".equals(action)) {
                    mVoiceNative.startSiren(true);
                } else if("remove".equals(action)) {
                    mVoiceNative.startSiren(false);
                }
            }
        }
    };

    @Override
    public android.os.IBinder onBind(Intent intent) {
        return null;
    }
}
