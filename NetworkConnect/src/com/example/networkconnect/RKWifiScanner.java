package com.example.networkconnect;

import android.net.wifi.WifiManager;
import android.os.Handler;

public class RKWifiScanner {

	private WifiManager mWifiManager;
	private Handler mHandler;
	private boolean flag;

	public RKWifiScanner(WifiManager mWifiManager, Handler mHandler) {
		this.mWifiManager = mWifiManager;
		this.mHandler = mHandler;
	}

	public void startScan() {
		flag = false;
		if (!mHandler.hasMessages(RKWifiConnectService.MESSAGE_STATE_SCAN)) {
			mHandler.sendEmptyMessage(RKWifiConnectService.MESSAGE_STATE_SCAN);
		}
	}

	public void stopScan() {
		flag = true;
		mHandler.removeMessages(RKWifiConnectService.MESSAGE_STATE_SCAN);
	}

	public void run() {
		if (flag) {
			return;
		}
		mWifiManager.startScan();
		mHandler.sendEmptyMessageDelayed(
				RKWifiConnectService.MESSAGE_STATE_SCAN, 5000);
	}
}
