package com.example.hotfix;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;

import android.content.Context;
import android.content.pm.PackageInfo;
import android.os.Environment;
import android.util.Log;

public class Utils {

	static{
		System.loadLibrary("patcher_tools");
	}
	
	public native void patch(String old_apk_path, String new_apk_path, String patch);
	
	public void patch(Context mContext) throws Exception{
		PackageInfo packageInfo = mContext.getPackageManager().getPackageInfo(mContext.getPackageName(), 0);
		String targe = Environment.getExternalStorageDirectory() + File.separator + mContext.getPackageName();
		String source = packageInfo.applicationInfo.sourceDir;
		FileOutputStream out = null;
		FileInputStream in = null;
		try {
			in = new FileInputStream(new File(source));
			out = new FileOutputStream(new File(targe, "base.apk"));
			int len = -1;
			byte []buffer = new byte[1024];
			while((len = in.read(buffer)) != -1){
				out.write(buffer, 0, len);
			}
		}finally{
			if(out != null)
				out.close();
			if(in != null)
				in.close();
		}
	}
}
