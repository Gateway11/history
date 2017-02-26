package com.example.networkconnect;

import android.Manifest;
import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.IBinder;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;

public class MainActivity extends Activity implements OnClickListener {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		((Button)findViewById(R.id.disconnect)).setOnClickListener(this);
		((Button)findViewById(R.id.connect)).setOnClickListener(this);
		bindService(new Intent(this,RKWifiConnectService.class), conn, Context.BIND_AUTO_CREATE);
	}
	
	ServiceConnection conn=new ServiceConnection() {
		@Override
		public void onServiceDisconnected(ComponentName name) {
			
		}
		@Override
		public void onServiceConnected(ComponentName name, IBinder service) {
			mWifiManager = (IRKWifiManager) service;
		}
	};
	
	private IRKWifiManager mWifiManager;
	
	@Override
	public void onClick(View v) {
		switch (v.getId()) {
		case R.id.disconnect:
			mWifiManager.disconnect();
			break;

		case R.id.connect:
			if (checkSelfPermission(Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
				requestPermissions(
						new String[] { Manifest.permission.ACCESS_COARSE_LOCATION },
						0);
			}
			mWifiManager.connect("ROKID.TC", "rokidguys");
			break;
		}
	}
}
