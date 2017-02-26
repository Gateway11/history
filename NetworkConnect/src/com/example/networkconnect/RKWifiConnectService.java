package com.example.networkconnect;

import java.util.concurrent.atomic.AtomicBoolean;

import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.net.ConnectivityManager;
import android.net.ConnectivityManager.NetworkCallback;
import android.net.Network;
import android.net.NetworkCapabilities;
import android.net.NetworkInfo;
import android.net.NetworkInfo.DetailedState;
import android.net.wifi.SupplicantState;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.net.wifi.WifiManager.WifiLock;
import android.os.Binder;
import android.os.Handler;
import android.os.IBinder;
import android.util.Log;

public class RKWifiConnectService extends Service {
	
	private final String TAG = "RKWifiConnectService";

	public static final int MESSAGE_WIFI_STATE_CHANGED = 0;
	public static final int MESSAGE_STOP_SCAN = 1;
	public static final int MESSAGE_SCAN_COMPLETE = 2;
	public static final int MESSAGE_UPDATE_NETWORK_INFO = 3;
	public static final int MESSAGE_UPDATE_ACCESS_POINTS = 4;
	public static final int MESSAGE_RSSI_CHANGED =5;
	public static final int MESSAGE_SUPPLICANT_STATE_CHANGED = 6;
	public static final int MESSAGE_STATE_SCAN = 7;
	public static final int MESSAGE_CONNECTED_NO_INTERNET = 8;
	Handler mHandler = new Handler() {
		public void handleMessage(android.os.Message msg) {
			switch (msg.what) {
			case MESSAGE_WIFI_STATE_CHANGED:
				updateWifiState(msg.arg1);
				break;
			case MESSAGE_STOP_SCAN:
				mWifiScanner.stopScan();
				break;
			case MESSAGE_SCAN_COMPLETE:
				mWifiConnectHelper.updateAccessPoints();
				break;
			case MESSAGE_UPDATE_NETWORK_INFO:
				updateNetworkInfo((NetworkInfo) msg.obj);
				break;
			case MESSAGE_UPDATE_ACCESS_POINTS:
				mWifiConnectHelper.updateAccessPoints();
			case MESSAGE_SUPPLICANT_STATE_CHANGED:
				supplicantStateChanaged(msg.arg1, (SupplicantState) msg.obj);
				break;
			case MESSAGE_RSSI_CHANGED:
				updateNetworkInfo();
				break;
			case MESSAGE_STATE_SCAN:
				mWifiScanner.run();
				break;
			case MESSAGE_CONNECTED_NO_INTERNET:
				//TODO
				break;
			}
		}
	};
	private final AtomicBoolean mConnected = new AtomicBoolean(false);
	private static boolean connecting = false;
	private WifiManager mWifiManager;
	private ConnectivityManager cm;
	private RKWifiScanner mWifiScanner;
	private RKWifiConnectHelper mWifiConnectHelper;
	private WifiLock wifiLock;
	
	public static final int DISABLED_UNKNOWN_REASON                         = 0;
	public static final int DISABLED_DNS_FAILURE                            = 1;
	public static final int DISABLED_DHCP_FAILURE                           = 2;
	public static final int DISABLED_AUTH_FAILURE                           = 3;

	@Override
	public void onCreate() {
		super.onCreate();
		cm = (ConnectivityManager)getApplicationContext().getSystemService(Context.CONNECTIVITY_SERVICE);
		mWifiManager = (WifiManager) getApplicationContext().getSystemService(Context.WIFI_SERVICE);
		new RKWifiReceiver().registReceiver(getApplicationContext(), mHandler);
		mWifiScanner = new RKWifiScanner(mWifiManager, mHandler);
		mWifiConnectHelper = new RKWifiConnectHelper(mHandler, mWifiManager);
		if (!mWifiManager.isWifiEnabled()) {
			mWifiManager.setWifiEnabled(true);
		} else {
			mWifiScanner.startScan();
		}
		wifiLock = mWifiManager.createWifiLock(TAG);
		android.net.NetworkRequest request = new android.net.NetworkRequest.Builder()
        .addTransportType(NetworkCapabilities.TRANSPORT_WIFI).build();
		cm.registerNetworkCallback(request, mNetworkCallback);
	}
	
	private void lockWifi(){
		if(wifiLock.isHeld()){
			wifiLock.acquire();
		}
	}
	
	NetworkCallback mNetworkCallback= new NetworkCallback() {
            @Override
            public void onAvailable(Network network) {
                NetworkInfo networkInfo = cm.getNetworkInfo(network);
                Log.e("dxx", "onAvailable     "+networkInfo.describeContents());
                cm.bindProcessToNetwork(network);
//                ConnectivityManager.setProcessDefaultNetwork(network);
            }
     };

	protected void updateWifiState(int arg1) {
		if (WifiManager.WIFI_STATE_ENABLED == arg1) {
			if (mWifiScanner != null) {
				mWifiScanner.startScan();
			}
		} else if(WifiManager.WIFI_STATE_DISABLING == arg1
				||WifiManager.WIFI_STATE_DISABLED == arg1){
			connecting = false;
			mConnected.set(false);
			mWifiConnectHelper.release();
			if (mWifiScanner != null) {
				mWifiScanner.stopScan();
			}
		}
	}
	
	private void updateNetworkInfo() {
		Network network = cm.getBoundNetworkForProcess();
		NetworkCapabilities capabilities = cm.getNetworkCapabilities(network);
		int downKbps = capabilities.getLinkDownstreamBandwidthKbps();
		int upKbps = capabilities.getLinkUpstreamBandwidthKbps();
		Log.e("dx", downKbps+WifiInfo.LINK_SPEED_UNITS+"        "+upKbps+"kbps");
	}

	protected void supplicantStateChanaged(int arg1, SupplicantState obj) {
		if(arg1 == WifiManager.ERROR_AUTHENTICATING && obj == SupplicantState.DISCONNECTED){
			if(connecting){
				
			}else{
				callback.disconnected();
				mWifiScanner.startScan();
			}
		}
	}

	protected void updateNetworkInfo(NetworkInfo mInfo) {
		if (mInfo != null) {
			connecting = mInfo.isConnectedOrConnecting();
			DetailedState detailedState = mInfo.getDetailedState();
			if (detailedState == DetailedState.OBTAINING_IPADDR) {
				mWifiScanner.stopScan();
			}
			mConnected.set(mInfo.isConnected());
		}
		mWifiConnectHelper.updateNetworkInfo(mInfo);
		if(mConnected.get()){
			lockWifi();
			Network[] allNetworks = cm.getAllNetworks();
			for (Network network : allNetworks) {
				NetworkCapabilities networkCapabilities = cm.getNetworkCapabilities(network);
//				 nw = wifiManager.getCurrentNetwork();
				if(networkCapabilities!=null&& networkCapabilities.hasCapability(NetworkCapabilities.NET_CAPABILITY_VALIDATED)){
					if(mHandler.hasMessages(MESSAGE_CONNECTED_NO_INTERNET))
						mHandler.removeMessages(MESSAGE_CONNECTED_NO_INTERNET);
					callback.connected();
				}else{
					mHandler.sendEmptyMessageDelayed(MESSAGE_CONNECTED_NO_INTERNET, 5000);
				}
			}
			mWifiScanner.stopScan();
		}else{
//			WifiConfiguration mConfig = mWifiConnectHelper.getCurrentConfig();
//			if (mConfig != null && mConfig.status == WifiConfiguration.Status.DISABLED) {
//		        switch (mConfig.disableReason) {
//		            case DISABLED_AUTH_FAILURE:
//						password fail
//		                break;
//		            case DISABLED_DHCP_FAILURE:
//		            case DISABLED_DNS_FAILURE:
//						network fail
//		                break;
//		            case DISABLED_UNKNOWN_REASON:
//						wifi disable
//		        }   
//		    } else if (mRssi == Integer.MAX_VALUE) { 
//				 Wifi out of range
//		    } 
		}
	}
	
	@Override
	public IBinder onBind(Intent intent) {
		mWifiConnectHelper.registCallback(callback);
		return new RKWifiManagerStub();
	}
	
	IRKWifiConnectCallback callback = new IRKWifiConnectCallback() {
		
		@Override
		public void ssidNotFind() {
			Log.e("dx", "ssidNotFind");
		}
		
		@Override
		public void pwdError() {
			Log.e("dx", "pwdError");
		}
		
		@Override
		public void disconnected() {
			Log.e("dx", "disconnected");
		}
		
		@Override
		public void connected() {
			Log.e("dxx", "connected");
		}
	};

	class RKWifiManagerStub extends Binder implements IRKWifiManager {

		@Override
		public boolean connect(String ssid, String pwd) {
			boolean connect = mWifiConnectHelper.connect(ssid, pwd);
			if(connect){
				connecting = true;
			}
			return connect;
		}

		@Override
		public boolean disconnect() {
			mWifiConnectHelper.disconnect();
			return false;
		}

		@Override
		public boolean isConnected() {
			return mConnected.get();
		}
	};
}
