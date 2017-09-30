package com.rokid.openvoice;

public class VoiceManager{
	
	static{
		System.loadLibrary("openvoice2");
	}

    public static final int SIREN_STATE_AWAKE           = 1;
    public static final int SIREN_STATE_SLEEP           = 2;
    public static final int SIREN_STATE_START           = 3;
    public static final int SIREN_STATE_STOP            = 4;
    
	public static native boolean init(); 
	
	public static native void startSiren(boolean isopen); 
	
	public static native void setSirenState(int state);
	
	public static native void networkStateChange(boolean isconnected);
	
	public static native void updateStack(String appid);

    public static native void updateConfig(String device_id, String device_type_id, String key, String secret);

    public static native void registCallback(VoiceCallback callback);
}
