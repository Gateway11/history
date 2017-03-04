package com.example.bluetoothbleserver;

import android.app.Activity;
import android.net.wifi.WifiManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothGattServer;
import android.bluetooth.BluetoothManager;
import android.bluetooth.le.AdvertiseCallback;
import android.bluetooth.le.AdvertiseData;
import android.bluetooth.le.AdvertiseSettings;
import android.bluetooth.le.BluetoothLeAdvertiser;
import android.content.Context;
import android.content.Intent;

import com.example.bluetoothbleserver.MockServerCallBack;
import com.example.bluetoothbleserver.WifiAutoConnectManager.WifiCipherType;
public class BluetoothClientActivity extends Activity {


    private static final String TAG = "BluetoothClientActivity";

    private BluetoothManager mBluetoothManager;
    private Context mContext;
    private BluetoothAdapter mBluetoothAdapter;
    private static final int REQUEST_ENABLE_BT = 2;
    private Button broadcastButton, connectwifiButton;
    private BluetoothLeAdvertiser mBluetoothAdvertiser;
    private MockServerCallBack mMockServerCallBack;
    private BluetoothGattServer mGattServer;
    private MyHandler myHandler;
    public static boolean isConnected = false;
    public static String wifiName = null;
    public static String wifiPassword = null;
    private TextView wifiNameView, wifiPasswordView;
    private WifiAutoConnectManager mWifiAutoConnectManager;
    private WifiManager mWifiManager;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_bluetooth_client);

        mContext = this;
        myHandler = new MyHandler();
        mWifiManager = (WifiManager)getSystemService(Context.WIFI_SERVICE);
        mWifiAutoConnectManager = new WifiAutoConnectManager(mWifiManager);

        broadcastButton = (Button)findViewById(R.id.broadcastButton);
        broadcastButton.setOnClickListener(lisBroadcastBlue);

        connectwifiButton = (Button)findViewById(R.id.connectwifiButton);
        connectwifiButton.setOnClickListener(lisConnectWifiBlue);

        wifiNameView = (TextView)findViewById(R.id.wifiNameView);
        wifiPasswordView = (TextView)findViewById(R.id.wifiPasswordView);

    }

    private OnClickListener lisBroadcastBlue = new OnClickListener() {

        @Override
        public void onClick(View v) {
            if(mBluetoothManager == null)
                mBluetoothManager = (BluetoothManager) mContext.
            getSystemService(Context.BLUETOOTH_SERVICE);

            if (mBluetoothManager != null && mBluetoothAdapter == null) {
                mBluetoothAdapter = mBluetoothManager.getAdapter();
            }

            if ((mBluetoothAdapter == null) || (!mBluetoothAdapter.isEnabled())) {
                Toast.makeText(mContext, "unavailable", Toast.LENGTH_SHORT).show();
                Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
            }

            startBroadcast();
        }
    };

    private OnClickListener lisConnectWifiBlue = new OnClickListener() {

        @Override
        public void onClick(View v) {

            String name = wifiNameView.getText().toString();
            String pd = wifiPasswordView.getText().toString();

            mWifiAutoConnectManager.connect(name, pd, WifiAutoConnectManager.WifiCipherType.WIFICIPHER_WPA);

        }
    };

    private void startBroadcast() {
        if (mBluetoothAdvertiser == null) {
            mBluetoothAdvertiser = mBluetoothAdapter.getBluetoothLeAdvertiser();
        }
        if (mBluetoothAdvertiser != null) {
            mMockServerCallBack = new MockServerCallBack(mContext, myHandler);
            mMockServerCallBack.setActivity(this);
            mGattServer = mBluetoothManager.openGattServer(mContext, mMockServerCallBack);
            if(mGattServer == null){
                Log.d(TAG , "gatt is null");
            }
            try {
                mMockServerCallBack.setupServices(mGattServer);
                mBluetoothAdvertiser.startAdvertising(createAdvSettings(true, 0), createFMPAdvertiseData(),mAdvCallback);
            } catch(InterruptedException e) {
                Log.v(TAG, "Fail to setup BleService");
            }
        }

    }



    public static AdvertiseSettings createAdvSettings(boolean connectable, int timeoutMillis) {
        AdvertiseSettings.Builder builder = new AdvertiseSettings.Builder();
        builder.setAdvertiseMode(AdvertiseSettings.ADVERTISE_MODE_BALANCED);
        builder.setConnectable(connectable);
        builder.setTimeout(timeoutMillis);
        builder.setTxPowerLevel(AdvertiseSettings.ADVERTISE_TX_POWER_HIGH);
        return builder.build();
    }

    public static AdvertiseData createFMPAdvertiseData() {
        AdvertiseData.Builder builder = new AdvertiseData.Builder();
        builder.setIncludeDeviceName(true);
        AdvertiseData adv = builder.build();
        return adv;
    }

    private AdvertiseCallback mAdvCallback = new AdvertiseCallback() {
        public void onStartSuccess(android.bluetooth.le.AdvertiseSettings settingsInEffect) {
            if (settingsInEffect != null) {
                Log.d(TAG, "onStartSuccess TxPowerLv="+ settingsInEffect.getTxPowerLevel()+ " mode=" + settingsInEffect.getMode()+ " timeout=" + settingsInEffect.getTimeout());
            } else {
                Log.d(TAG, "onStartSuccess, settingInEffect is null");
            }
        }

        public void onStartFailure(int errorCode) {
            Log.d(TAG, "onStartFailure errorCode=" + errorCode);
        };
    };



    public class MyHandler extends Handler {

        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case 0:
                case 2:
                    String data = (String)msg.obj;
                    Log.d(TAG, "MyHandler handleMessage<> 0 data = <" + data + ">; isConnected = " + isConnected);
	                    if (data != null && !(data.equals("##wifi Con##") || data.equals("##wifi Recon##"))) {

                        if (isConnected) {
                            Log.d(TAG, "MyHandler handleMessage<> A");

                            if (wifiName == null) {
                                Log.d(TAG, "MyHandler handleMessage<> B");
                                Toast.makeText(mContext,"wifi_Name: data: " + data,
                                Toast.LENGTH_SHORT).show();
                                wifiName = data;
                                wifiNameView.setText(wifiName);
                            } else if (wifiPassword == null) {
                                Log.d(TAG, "MyHandler handleMessage<> C");
                                Toast.makeText(mContext,"wifi_Password: data: " + data,
                                Toast.LENGTH_SHORT).show();

                                wifiPassword = data;
                                wifiPasswordView.setText(wifiPassword);

                                wifiName = null;
                                wifiPassword = null;
                            }
                        }
                    } else {
                        Toast.makeText(mContext,data, Toast.LENGTH_SHORT).show();
                    }
                break;
            }
        }
    }
}
