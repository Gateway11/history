package com.rokid.openvoice;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

import android.content.Context;
import android.content.res.AssetManager;

public class ConfigManager {

	private AssetManager mAssetManager = null;
	private final String[] paths = { FileUtils.ETC, FileUtils.WORKDIR_CN };

	public ConfigManager(Context mContext) {
		mAssetManager = mContext.getAssets();
	}

	public boolean install() {
		for (String core : paths) {
			String target = FileUtils.mkdirs(core);
			try {
				String[] list = mAssetManager.list(core);
				for (String path : list) {
					FileUtils.copyFrom(mAssetManager.open(core + "/" + path), new FileOutputStream(new File(target, path)));
				}
			} catch (IOException e) {
				e.printStackTrace();
				return false;
			}
		}
		return true;
	}
}
