package com.example.networkconnect;

public interface IRKWifiConnectCallback {
	
	void ssidNotFind();
	void pwdError();
	void disconnected();
	void connected();
}
