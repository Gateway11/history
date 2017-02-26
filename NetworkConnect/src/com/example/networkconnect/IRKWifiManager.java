package com.example.networkconnect;

public interface IRKWifiManager {
	boolean connect(String ssid, String pwd);
	boolean disconnect();
	boolean isConnected();
}
