package com.rokid.openvoice;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

public class FileUtils {

	//Don't modify
	private static final String ROOT = "/sdcard/rokid";
	
	public static final String WORKDIR_CN = "workdir_cn";
	public static final String ETC = "etc";
	
	static{
		File file = new File(ROOT);
		if(!file.exists()) file.mkdirs();
	}
	
	public static void delete(File file){
		if(file != null){
			if(file.isDirectory()){
				File[] files = file.listFiles();
				for (File file2 : files) {
					delete(file2);
				}
				file.delete();
			}else{
				file.delete();
			}
		}
	}
	
	public static String mkdirs(String path){
		File file = new File(ROOT, path);
		if(file.exists()){
			delete(file);
		}
		file.mkdirs();
		return file.getAbsolutePath();
	}
	
	public static void copyFrom(InputStream current, FileOutputStream target) {
		if(current != null && target != null){
			int len;
			byte []buff = new byte[4096];
			try {
				while ((len = current.read(buff)) != -1) {
					target.write(buff, 0, len);
				}
			} catch (IOException e) {
				e.printStackTrace();
			}finally{
				try {
					target.close();
					current.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
		}
	}
}
