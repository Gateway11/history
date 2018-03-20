package com.rokid.openvoice;

import android.content.Intent;

public class VoiceReceiver extends android.content.BroadcastReceiver{

	@Override
	public void onReceive(android.content.Context context, Intent intent){

		String action = intent.getAction();
		android.util.Log.e("VoiceReceiver", "onReceive : " + action);
		Intent intent2 = new Intent(context, VoiceService.class);
		
		if(Intent.ACTION_BOOT_COMPLETED.equals(action)){
			context.startService(intent2);
		}else if(android.net.ConnectivityManager.CONNECTIVITY_ACTION.equals(action)){

			android.net.NetworkInfo mNetworkInfo = intent.getParcelableExtra(android.net.ConnectivityManager.EXTRA_NETWORK_INFO);
			if(mNetworkInfo != null){
				intent2.putExtra("cmd", "netchange");
				intent2.putExtra("isConnect", mNetworkInfo.isConnected());
				context.startService(intent2);
			}
		}
	}
}
