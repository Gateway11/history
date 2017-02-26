package com.example.bleclient;

import java.util.ArrayList;
import java.util.List;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothAdapter.LeScanCallback;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothManager;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

public class BLEClientActivity extends Activity {

	private String TAG = "GattCallback";

	private Context mContext;
	private BLEAdapter mBLEAdapter;
	private ClientGattCallback mGattCallback;
	private BTConnectReceiver mBTConnectReceiver;
	private BluetoothAdapter mBluetoothAdapter;

	private static final int ENABLE = 0;
	private static final int DISABLE = 1;
	private static int mCurrentBluetoothState = DISABLE;
	
	private List<BluetoothDevice> devices = new ArrayList<BluetoothDevice>();

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		mContext = getApplicationContext();
		BluetoothManager mBluetoothManager = (BluetoothManager) getSystemService(Context.BLUETOOTH_SERVICE);
		mBluetoothAdapter = mBluetoothManager.getAdapter();
		mGattCallback = new ClientGattCallback(mContext, mHandler);
		mBTConnectReceiver = new BTConnectReceiver();
		mBLEAdapter = new BLEAdapter();
		mBTConnectReceiver.registerReceiver();
		mBTConnectReceiver.enableBluetooth();
		if(mCurrentBluetoothState == ENABLE) {
			mBluetoothAdapter.startLeScan(mLeScanCallback);
		}
		ListView lv = (ListView) findViewById(R.id.lv);
		lv.setAdapter(mBLEAdapter);
	}

	public class BTConnectReceiver extends BroadcastReceiver {

		public void registerReceiver() {
			mContext.registerReceiver(this, new IntentFilter(BluetoothAdapter.ACTION_STATE_CHANGED));
		}
		
		private void enableBluetooth() {
			if (mBluetoothAdapter != null) {
				if (!mBluetoothAdapter.isEnabled()) {
					if (!mBluetoothAdapter.enable()) {
						Log.e(TAG, "bluetooth open fail !");
					}
				}else {
					mCurrentBluetoothState = ENABLE;
					Log.e(TAG, "bluetooth enabled !");
				}
			}
		}

		public void onReceive(Context context, Intent intent) {
			if (intent.getAction().equals(BluetoothAdapter.ACTION_STATE_CHANGED)) {
				int state = intent.getIntExtra(BluetoothAdapter.EXTRA_STATE,BluetoothAdapter.ERROR);
				int prevState = intent.getIntExtra(BluetoothAdapter.EXTRA_PREVIOUS_STATE,BluetoothAdapter.ERROR);
				Log.i(TAG, "change state from " + prevState + " to " + state);
				if (state == BluetoothAdapter.STATE_ON) {
					mCurrentBluetoothState = ENABLE;
					mBluetoothAdapter.startLeScan(mLeScanCallback);
				}
			}
		}
	}
	
	private LeScanCallback mLeScanCallback = new LeScanCallback() {

		@Override
		public void onLeScan(final BluetoothDevice device, int rssi,byte[] scanRecord) {
			if (device.getName() == null || !device.getName().contains("Rokid"))return;
			if(devices.contains(device)) return;
			devices.add(device);
			runOnUiThread(new Runnable() {
				public void run() {
					mBLEAdapter.notifyDataSetChanged();
				}
			});
		}
	};
	
	private class BLEAdapter extends BaseAdapter {

		@Override
		public int getCount() {
			return devices.size();
		}

		@Override
		public Object getItem(int arg0) {
			return devices.get(arg0);
		}

		@Override
		public long getItemId(int arg0) {
			return arg0;
		}

		@Override
		public View getView(int arg0, View arg1, ViewGroup arg2) {
			TextView tv = new TextView(getApplicationContext());
			tv.setText(devices.get(arg0).getName());
			tv.setTextColor(Color.BLACK);
			tv.setGravity(Gravity.CENTER_VERTICAL);
			tv.setHeight(100);
			tv.setTag(devices.get(arg0));
			tv.setClickable(true);
			tv.setOnClickListener(new OnClickListener() {

				@Override
				public void onClick(View arg0) {
					showToast("CONNECTING ");
					mGattCallback.connect(((BluetoothDevice) arg0.getTag()).getAddress());
				}
			});
			return tv;
		}
	}

	private void showToast(final String text) {
		runOnUiThread(new Runnable() {

			@Override
			public void run() {
				Toast.makeText(mContext, text, Toast.LENGTH_LONG).show();
			}
		});
	}
	Handler mHandler = new Handler();
}
