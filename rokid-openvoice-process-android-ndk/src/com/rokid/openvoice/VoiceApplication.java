package com.rokid.openvoice;

import android.content.Context;
import android.os.Process;

public class VoiceApplication extends android.app.Application{

    private static Context mContext = null;

    static{
//        VoiceManager.init();
    	android.util.Log.e("VoiceService", "-------------------------	pid	: " + Process.myPid() + ", tid : " + Process.myTid() + ", uid : " + Process.myUid());
    }

    @Override
    public void onCreate() {
    	mContext = this;
    }

    public static Context getContext(){
        return mContext;
    }
}
