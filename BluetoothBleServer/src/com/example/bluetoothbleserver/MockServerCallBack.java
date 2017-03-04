package com.example.bluetoothbleserver;

import java.util.UUID;

import android.bluetooth.BluetoothGattServerCallback;
import android.app.Activity;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattDescriptor;
import android.bluetooth.BluetoothGattServer;
import android.bluetooth.BluetoothGattServerCallback;
import android.bluetooth.BluetoothGattService;
import android.content.Context;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.widget.Toast;

import com.example.bluetoothbleserver.BluetoothClientActivity.MyHandler;

public class MockServerCallBack extends BluetoothGattServerCallback {
    private static final String TAG = "MockServerCallBack";
    private byte[] mAlertLevel = new byte[] {(byte) 0x00};
    private BluetoothClientActivity mActivity;
    private boolean mIsPushStatic = false;
    private BluetoothGattServer mGattServer;
    private BluetoothGattCharacteristic mDateChar;
    private BluetoothDevice btClient;
    private BluetoothGattCharacteristic mHeartRateChar;
    private BluetoothGattCharacteristic mTemperatureChar;
    private BluetoothGattCharacteristic mBatteryChar;
    private BluetoothGattCharacteristic mManufacturerNameChar;
    private BluetoothGattCharacteristic mModuleNumberChar;
    private BluetoothGattCharacteristic mSerialNumberChar;
    private final static String SERVICE_IMMEDIATE_ALERT = "0000ffe1-0000-1000-8000-00805f9b34fb";

    private final static String CHAR_ALERT_LEVEL = "00002a06-0000-1000-8000-00805f9b34fb";

    private Context mContext;
    private MyHandler mHanldler;

    public MockServerCallBack(Context context, MyHandler handler) {
        mContext = context;
        mHanldler = handler;
    }

    public void setupServices(BluetoothGattServer gattServer) throws InterruptedException {
        if (gattServer == null) {
            throw new IllegalArgumentException("gattServer is null");
        }
        mGattServer = gattServer;
        {
            //immediate alert
            BluetoothGattService ias = new BluetoothGattService( UUID.fromString(SERVICE_IMMEDIATE_ALERT),
            BluetoothGattService.SERVICE_TYPE_PRIMARY);
            //alert level char.
            BluetoothGattCharacteristic alc = new BluetoothGattCharacteristic(
            UUID.fromString(CHAR_ALERT_LEVEL),
            BluetoothGattCharacteristic.PROPERTY_READ |BluetoothGattCharacteristic.PROPERTY_WRITE | BluetoothGattCharacteristic.PROPERTY_NOTIFY ,
                    BluetoothGattCharacteristic.PERMISSION_READ |BluetoothGattCharacteristic.PERMISSION_WRITE);
            alc.setValue("");
            ias.addCharacteristic(alc);
            if(mGattServer!=null && ias!=null)
                mGattServer.addService(ias);
        }
    }

    public void onServiceAdded(int status, BluetoothGattService service) {
        if (status == BluetoothGatt.GATT_SUCCESS) {
            Log.d(TAG, "onServiceAdded status=GATT_SUCCESS service=" + service.getUuid().toString());
        } else {
            Log.d(TAG, "onServiceAdded status!=GATT_SUCCESS");
        }
    }

    public void onConnectionStateChange(android.bluetooth.BluetoothDevice device, int status,
            int newState) {
        Log.d(TAG, "onConnectionStateChange status=" + status + "->" + newState);
        if (newState == 2) {
            mActivity.isConnected = true;
        } else {
            mActivity.isConnected = false;
            mActivity.wifiName = null;
            mActivity.wifiPassword = null;
        }
    }

    public void onCharacteristicReadRequest(android.bluetooth.BluetoothDevice device,
        int requestId, int offset, BluetoothGattCharacteristic characteristic) {
        Log.d(TAG, "onCharacteristicReadRequest requestId="
                + requestId + " offset=" + offset);

        byte[] bytes = characteristic.getValue();
        String data = new String(bytes);
        Log.w(TAG, "onCharacteristicReadRequest<> data = " + data);
        Message msg = new Message();
        msg.what = 0;
        msg.obj = data;
        mHanldler.sendMessage(msg);
        mGattServer.sendResponse(device, requestId, BluetoothGatt.GATT_SUCCESS, offset,characteristic.getValue());
    }

    @Override
    public void onCharacteristicWriteRequest(android.bluetooth.BluetoothDevice device,
            int requestId, BluetoothGattCharacteristic characteristic, boolean preparedWrite,
            boolean responseNeeded, int offset, byte[] value) {

        Log.d(TAG, "onCharacteristicWriteRequest");

        String data = new String(value);
        Log.w(TAG, "onCharacteristicWriteRequest<> data = " + data);

        Message msg = new Message();
        msg.what = 2;
        msg.obj = data;
        mHanldler.sendMessage(msg);
        mGattServer.sendResponse(device, requestId, BluetoothGatt.GATT_SUCCESS, offset, null);
    }

    @Override
    public void onDescriptorWriteRequest (BluetoothDevice device, int requestId, BluetoothGattDescriptor descriptor, boolean preparedWrite, boolean responseNeeded, int offset, byte[] value) {

        btClient = device;
        String data = new String(value);
        Log.d(TAG, "onDescriptorWriteRequest data = " + data);
        // now tell the connected device that this was all successfull
        mGattServer.sendResponse(device, requestId, BluetoothGatt.GATT_SUCCESS, offset, value);
    }

    @Override
    public void onDescriptorReadRequest(BluetoothDevice device, int requestId,
            int offset, BluetoothGattDescriptor descriptor) {

        Log.d(TAG, "onDescriptorReadRequest");
    }

    public void setActivity(BluetoothClientActivity activity){
        mActivity = activity;
    }

}
