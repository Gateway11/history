package com.example.bleclient;

import java.util.UUID;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCallback;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattService;
import android.bluetooth.BluetoothManager;
import android.bluetooth.BluetoothProfile;
import android.content.Context;
import android.os.Handler;
import android.util.Log;

public class ClientGattCallback extends BluetoothGattCallback{
	
	private String TAG = "GattClient";
	private String address;
	private Context mContext;
	private Handler mHandler;
	private BluetoothGatt mBluetoothGatt;
	
	String data = "{\"U\":\"18771359283\",\"S\":\"ROKID.TC\",\"P\":\"rokidguya\",\"TYPE\":\"WIFI\",\"FILD\":\"SYS\"}";
	
	private final static String SERVICE_IMMEDIATE_ALERT = "0000ffe1-0000-1000-8000-00805f9b34fb";
    private final static String CHAR_ALERT_LEVEL = "00002a06-0000-1000-8000-00805f9b34fb";
    
    public ClientGattCallback(Context mContext, Handler mHandler){
    	this.mContext = mContext;
    	this.mHandler = mHandler;
    }
	
	@Override
	public void onConnectionStateChange(BluetoothGatt gatt, int status, int newState) {
		if (newState == BluetoothProfile.STATE_CONNECTED) {
			Log.i(TAG, "Connected to GATT server.");
			mBluetoothGatt.discoverServices();
		}else {
			mBluetoothGatt = null;
		}
	}

	@Override
	public void onServicesDiscovered(BluetoothGatt gatt, int status) {
		if (status == BluetoothGatt.GATT_SUCCESS) {
			Log.w(TAG, "onServicesDiscovered received: " + status);
			mBluetoothGatt.requestMtu(data.length()+15);
		}
	}
	
	@Override
	public void onMtuChanged(BluetoothGatt gatt, int mtu, int status) {
		Log.e(TAG, "onMtuChanged  sttatus = " + (status == BluetoothGatt.GATT_SUCCESS));
		if(status == BluetoothGatt.GATT_SUCCESS){
			mBluetoothGatt = gatt;
			displayGattServices(gatt, data);
		}
	}
	
	@Override
	public void onCharacteristicWrite(BluetoothGatt gatt,
            BluetoothGattCharacteristic characteristic, int status) {
		String data = new String(characteristic.getValue());
        Log.w(TAG, "onCharacteristicWrite status = " + status + ", value = " + data);
	}
	
	public void connect(String address) {
		if(mBluetoothGatt != null && this.address.equals(address)){
			mBluetoothGatt.connect();
			return;
		}
		this.address = address;
		BluetoothAdapter mBluetoothAdapter = ((BluetoothManager) mContext.getSystemService(Context.BLUETOOTH_SERVICE)).getAdapter();
		if(mBluetoothAdapter != null){
			BluetoothDevice device = mBluetoothAdapter.getRemoteDevice(address);
			mBluetoothGatt = device.connectGatt(mContext, false, this);
		}
	}
	
	public void displayGattServices(BluetoothGatt gatt, String value) {
        BluetoothGattService service = new BluetoothGattService( 
        		UUID.fromString(SERVICE_IMMEDIATE_ALERT),
                BluetoothGattService.SERVICE_TYPE_PRIMARY);

        BluetoothGattCharacteristic characteristic;
        if (mBluetoothGatt != null) {
            service = mBluetoothGatt.getService(UUID.fromString(SERVICE_IMMEDIATE_ALERT));
        }
        if (service != null) {
        	characteristic = service.getCharacteristic(UUID.fromString(CHAR_ALERT_LEVEL));
        } else {
        	characteristic = new BluetoothGattCharacteristic(
                   UUID.fromString(CHAR_ALERT_LEVEL),
                   BluetoothGattCharacteristic.PROPERTY_READ 
                   | BluetoothGattCharacteristic.PROPERTY_WRITE 
                   | BluetoothGattCharacteristic.PROPERTY_NOTIFY ,
                   BluetoothGattCharacteristic.PERMISSION_READ 
                   | BluetoothGattCharacteristic.PERMISSION_WRITE);
        }
        if (characteristic != null) {
            characteristic.setValue(value);
            if(gatt == null) {
            	mBluetoothGatt.writeCharacteristic(characteristic);
            } else {
            	gatt.writeCharacteristic(characteristic);
            }
        }
    }
}
