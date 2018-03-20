package com.example.demo;

import android.app.Activity;
import android.os.Bundle;

import com.rokid.voicerec.BearKidAdapter;
import com.rokid.voicerec.BearKidCallback;
import com.rokid.voicerec.CustomWord;

public class MainActivity extends Activity implements BearKidCallback {

    public static final String TAG = "BEAR_KID_ACTIVITY";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        new Thread(new Runnable() {
            @Override
            public void run() {
                BearKidAdapter bearKidAdapter = new BearKidAdapter();
                boolean initialize = bearKidAdapter.initialize(getApplicationContext(), "com.rokid.openvoice.VoiceService", MainActivity.this);
                int i = bearKidAdapter.addCustomWord(CustomWord.TYPE_VT_WORD, "小祥祥", "xiao3xiang2xiang2");
            }
        }).start();
    }

    @Override
    public void onVoiceEvent(int i, int i1, double v) {

    }

    @Override
    public void onIntermediateResult(String s) {

    }

    @Override
    public void onRecognizeResult(String s, String s1, String s2) {

    }

    @Override
    public void onException(int i) {

    }
}
