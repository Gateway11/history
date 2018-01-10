package com.rokid.openvoice;

import android.content.Intent;
import android.content.ComponentName;
import android.content.ServiceConnection;
import android.content.Context;

import android.os.Parcel;
import android.os.IBinder;
import android.os.RemoteException;

import android.util.Log;

public class SkillServiceConnection implements ServiceConnection, IBinder.DeathRecipient{

    private String TAG = getClass().getSimpleName();

    private Context mContext = null;
    private String mSkillName = null;
    private IBinder mSkillToken = null;
    private String mPackageName;
    private String mClassName;

    public SkillServiceConnection(String skillName, String packageName, String className){
        this.mSkillName = skillName;
        this.mContext = VoiceApplication.getContext();
        this.mPackageName = packageName;
        this.mClassName = className;
      
        bindSkillService();
    }

    private void bindSkillService(){
        ComponentName cn = new ComponentName(mPackageName, mClassName);
        Intent intent = new Intent().setComponent(cn);
        mContext.bindService(intent, this, Context.BIND_AUTO_CREATE);
    }

    public String getSkillOption(){
        if(mSkillToken != null){
    		Parcel data = Parcel.obtain();
    		Parcel reply = Parcel.obtain();
    		try{
    			data.writeInterfaceToken(mSkillToken.getInterfaceDescriptor());
    			mSkillToken.transact(IBinder.FIRST_CALL_TRANSACTION, data, reply, 0);
    			reply.readException();
                return reply.readString();
    		}catch(RemoteException e){
    			e.printStackTrace();
                bindSkillService();
    		}finally{
    			data.recycle();
    			reply.recycle();
    		}
        }else{
            bindSkillService();
        }
        return "";
    }

    @Override
    public void onServiceConnected(ComponentName name, IBinder service) {
        mSkillToken = service;
        try {
			mSkillToken.linkToDeath(this, 0);
		} catch (RemoteException e) {
			e.printStackTrace();
		}
        Log.e(TAG, mSkillName + "  service conneted");
    }

    @Override
    public void onServiceDisconnected(ComponentName name) {
        mSkillToken = null;
        Log.e(TAG, mSkillName + "  service disconnected");
        bindSkillService();
    }

	@Override
	public void binderDied() {
		bindSkillService();
	}
}
