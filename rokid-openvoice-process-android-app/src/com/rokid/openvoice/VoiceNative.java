package com.rokid.openvoice;

public class VoiceNative{
	
	public static native final boolean native_setup(Object callback); 
	
	public static native final void native_start_Siren(boolean isopen); 
	
	public static native final void native_set_siren_state(int state);
	
	public static native final void native_network_state_hange(boolean isconnected);
	
	public static native final void native_update_tack(String appid);
}