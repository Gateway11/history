package com.rokid.openvoice;

import com.rokid.openvoice.entity.VoiceCommand;
import com.rokid.openvoice.entity.VoiceEvent;

import android.content.Intent;
import android.content.Context;
import android.util.Log;
import android.net.NetworkInfo;
import android.net.ConnectivityManager;
import android.os.Bundle;
import android.os.Handler;

public class VoiceService extends android.app.Service{

    String TAG = getClass().getSimpleName();
    
    public static final int MSG_VOICE_EVENT             = 0;
    public static final int MSG_INTERMEDIATE_RESULT		= 1;
    public static final int MSG_VOICE_COMMAND           = 2;
    public static final int MSG_SPEECH_ERROR            = 3;
    
    private static final int VOICE_COMING               = 0;
    private static final int VOICE_LOCAL_WAKE           = 1;
    private static final int VOICE_START                = 2;
    private static final int VOICE_ACCEPT               = 3;
    private static final int VOICE_REJECT               = 4;
    private static final int VOICE_CANCEL               = 5;
    private static final int VOICE_SLEEP                = 6;

    private static final int SPEECH_SERVER_INTERNAL     = 6;
    private static final int SPEECH_VAD_TIMEOUT         = 7;
    private static final int SPEECH_SERVICE_UNAVAILABLE = 101;
    private static final int SPEECH_TIMEOUT             = 103;
    
    public Handler mHandler = new Handler(){
    	public void handleMessage(android.os.Message msg) {
    		switch (msg.what) {
			case MSG_VOICE_EVENT:
				handleVoiceEvent((VoiceEvent)msg.obj);
				break;
			case MSG_INTERMEDIATE_RESULT:
				Bundle data = msg.getData();
				handleIntermediateResult(data.getInt("id"), data.getInt("type"), data.getString("asr"));
				break;
			case MSG_VOICE_COMMAND:
				handleVoiceCommand((VoiceCommand)msg.obj);
				break;
			case MSG_SPEECH_ERROR:
				handleSpeechError(msg.arg1, msg.arg2);
				break;
			}
    	}
    };

	public VoiceService(){
		Log.e(TAG, "VoiceService  created ");
        VoiceManager.registCallback(new VoiceCallback(mHandler));
	}

	@Override
	public void onCreate(){
		ConnectivityManager cm = (ConnectivityManager)getSystemService(Context.CONNECTIVITY_SERVICE);
		NetworkInfo mNetworkInfo = cm.getActiveNetworkInfo();
//		if(mNetworkInfo != null){
		    VoiceManager.networkStateChange(true);
//		}
//		mUEventObserver.startObserving("/sound/card1/pcmC1D0c");
	}
	
	public void handleVoiceEvent(VoiceEvent mVoiceEvent){
		
	}
	
	public void handleIntermediateResult(int id, int type, String asr){
		
	}
	
	public void handleVoiceCommand(VoiceCommand mVoiceCommand){
		
	}

	public void handleSpeechError(int id, int errcode){
	
	}

	private final android.os.UEventObserver mUEventObserver = new android.os.UEventObserver() {
		 
		@Override
		public void onUEvent(android.os.UEventObserver.UEvent event){
			Log.e(TAG, event.toString());
    		String action = event.get("ACTION");
    		if("add".equals(action)){
    			VoiceManager.startSiren(true);	
    		}else if("remove".equals(action)){
    			VoiceManager.startSiren(false);
    		}
		}
	};

	@Override
	public android.os.IBinder onBind(Intent intent) {
		return null;
	}
}
