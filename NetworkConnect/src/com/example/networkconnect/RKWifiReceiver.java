package com.example.networkconnect;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.net.NetworkInfo;
import android.net.wifi.SupplicantState;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

public class RKWifiReceiver extends BroadcastReceiver {

	private static final String HIDE_LINK_CONFIGURATION_CHANGED_ACTION = "android.net.wifi.LINK_CONFIGURATION_CHANGED";
	private static final String HIDE_CONFIGURED_CHANGED_ACTION = "android.net.wifi.CONFIGURED_NETWORKS_CHANGE";

	private Handler mHandler;

	public void registReceiver(Context mContext, Handler mHandler) {
		this.mHandler = mHandler;

		IntentFilter mFilter = new IntentFilter();
		mFilter.addAction(WifiManager.WIFI_STATE_CHANGED_ACTION);
		mFilter.addAction(WifiManager.SCAN_RESULTS_AVAILABLE_ACTION);
		mFilter.addAction(WifiManager.NETWORK_IDS_CHANGED_ACTION);
		mFilter.addAction(WifiManager.SUPPLICANT_STATE_CHANGED_ACTION);
		mFilter.addAction(WifiManager.NETWORK_STATE_CHANGED_ACTION);
		mFilter.addAction(WifiManager.RSSI_CHANGED_ACTION);

		mFilter.addAction(HIDE_CONFIGURED_CHANGED_ACTION);
		mFilter.addAction(HIDE_LINK_CONFIGURATION_CHANGED_ACTION);

		if (mContext != null) {
			mContext.registerReceiver(this, mFilter);
		}
	}

	@Override
	public void onReceive(Context context, Intent intent) {
		String action = intent.getAction();
		if (action.equals(WifiManager.WIFI_STATE_CHANGED_ACTION)) {
			int state = intent.getIntExtra(WifiManager.EXTRA_WIFI_STATE,WifiManager.WIFI_STATE_UNKNOWN);
			mHandler.obtainMessage(
					RKWifiConnectService.MESSAGE_WIFI_STATE_CHANGED, state, 0).sendToTarget();
		} else if (action.equals(WifiManager.SCAN_RESULTS_AVAILABLE_ACTION)) {
			mHandler.sendEmptyMessage(RKWifiConnectService.MESSAGE_SCAN_COMPLETE);
		} else if (action.equals(WifiManager.NETWORK_STATE_CHANGED_ACTION)) {
			NetworkInfo info = (NetworkInfo) intent.getParcelableExtra(WifiManager.EXTRA_NETWORK_INFO);
			mHandler.obtainMessage(
					RKWifiConnectService.MESSAGE_UPDATE_NETWORK_INFO, info).sendToTarget();
		} else if (action.equals(WifiManager.RSSI_CHANGED_ACTION)) {
			mHandler.sendEmptyMessage(RKWifiConnectService.MESSAGE_RSSI_CHANGED);
		} else if (action.equals(WifiManager.NETWORK_IDS_CHANGED_ACTION)
				|| action.equals(HIDE_CONFIGURED_CHANGED_ACTION)
				|| action.equals(HIDE_LINK_CONFIGURATION_CHANGED_ACTION)) {
			mHandler.sendEmptyMessage(RKWifiConnectService.MESSAGE_UPDATE_NETWORK_INFO);
		} else if (action.equals(WifiManager.SUPPLICANT_STATE_CHANGED_ACTION)) {
			SupplicantState state = (SupplicantState) intent.getParcelableExtra(WifiManager.EXTRA_NEW_STATE);
			int error = intent.getIntExtra(WifiManager.EXTRA_SUPPLICANT_ERROR,0);
			mHandler.obtainMessage(RKWifiConnectService.MESSAGE_SUPPLICANT_STATE_CHANGED,
					error, 0, state).sendToTarget();
		}
	}
}
