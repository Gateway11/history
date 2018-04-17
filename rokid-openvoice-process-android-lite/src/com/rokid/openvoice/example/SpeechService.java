package com.rokid.openvoice.example;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.app.Service;
import android.content.Intent;
import android.content.res.AssetManager;
import android.media.AudioRecord;
import android.media.MediaPlayer;
import android.media.MediaRecorder;
import android.os.Environment;
import android.os.Handler;
import android.os.IBinder;
import android.util.Log;

import com.rokid.openvoice.VoiceActivation;
import com.rokid.openvoice.VoiceActivation.State;
import com.rokid.openvoice.VoiceActivationBuilder;
import com.rokid.speech.OpusPlayer;
import com.rokid.speech.PrepareOptions;
import com.rokid.speech.Speech;
import com.rokid.speech.Speech.VoiceOptions;
import com.rokid.speech.SpeechCallback;
import com.rokid.speech.SpeechOptions;
import com.rokid.speech.Tts;
import com.rokid.speech.TtsCallback;

public class SpeechService extends Service implements Runnable,
		VoiceActivation.Callback, SpeechCallback, TtsCallback {

	@Override
	public IBinder onBind(Intent intent) {
		throw new UnsupportedOperationException("Not yet implemented");
	}

	@Override
	public void onCreate() {
		new Thread(this).start();
	}

	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
		return START_NOT_STICKY;
	}

	@Override
	public void run() {
		if (!prepareWorkDirAndFiles()) {
			Log.e(TAG, "create workdir_cn and files failed");
			return;
		}

		VoiceActivationBuilder builder = new VoiceActivationBuilder();
		// 设置输入pcm流的采样率，位宽
		// 麦克风数
		// 本地vad模式
		// 设备上workdir_cn所在目录(算法模块需要读取此目录下模型文件)
		// 单通道，通道高级参数不设置，全部忽略
		// 回调对象
		VoiceActivation va = builder
				.setSampleRate(16000)
				.setBitsPerSample(VoiceActivationBuilder.AudioFormat.ENCODING_PCM_16BIT)
				.setChannelNumber(1)
				.enableVad(false)
				.setBasePath(Environment.getExternalStorageDirectory().getPath())
				.addMic(0, 0, 0, 0, 0).setMicParamMask(0).setCallback(this)
				.build();
		
		AssetManager mAssetManager = getApplicationContext().getAssets();
		
		InputStream inputStream = null;
		try {
			inputStream = mAssetManager.open(OPENVOICE_CONFIG_FILE);
		} catch (IOException e1) {
			e1.printStackTrace();
		}
		mSpeech = new Speech();
		PrepareOptions opts = mSpeech.parseConfig(inputStream);
		SpeechOptions opt = new SpeechOptions();
		opt.set_codec("pcm");
		opt.set_lang("zh");
		opt.set_vad_mode("cloud");
		mSpeech.config(opt);
		mSpeech.prepare(opts);
		
		try {
			inputStream = mAssetManager.open(TTS_CONFIG_FILE);
		} catch (IOException e1) {
			e1.printStackTrace();
		}
		mTts = new Tts();
		opts = mTts.parseConfig(inputStream);
		mTts.prepare(opts);
		
		mOpusPlayer = new OpusPlayer();
		
		mMediaPlayer = new MediaPlayer();
		
		try {
			out = new FileOutputStream(Environment.getExternalStorageDirectory().getPath() + "/tt.pcm");
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}

		processVoiceData(va);
	}

	private boolean prepareWorkDirAndFiles() {
		AssetManager am = getApplicationContext().getAssets();
		try {
			copyAssetDir(am, "workdir_cn", Environment.getExternalStorageDirectory().getPath() + "/workdir_cn");
		} catch (Exception e) {
			e.printStackTrace();
			return false;
		}
		return true;
	}

	private boolean copyAssetDir(AssetManager am, String src, String dst)
			throws Exception {
		File f = new File(dst);
		if (!f.exists() && !f.mkdir()) {
			Log.e(TAG, "mkdir " + dst + " failed");
			return false;
		}
		String[] files = am.list(src);
		String[] subfiles;
		int i;
		int c;
		InputStream is;
		FileOutputStream os;
		byte[] buf = new byte[2048];
		for (i = 0; i < files.length; ++i) {
			subfiles = am.list(src + "/" + files[i]);
			if (subfiles.length > 0) {
				if (!copyAssetDir(am, src + "/" + files[i], dst + "/"
						+ files[i]))
					return false;
			} else {
				is = am.open(src + "/" + files[i]);
				os = new FileOutputStream(dst + "/" + files[i]);
				while (true) {
					c = is.read(buf);
					if (c <= 0)
						break;
					os.write(buf, 0, c);
				}
			}
		}
		return true;
	}

	private void processVoiceData(VoiceActivation va) {
		int min = AudioRecord.getMinBufferSize(16000, 16, 2);
		AudioRecord ar = new AudioRecord(MediaRecorder.AudioSource.VOICE_COMMUNICATION,
				16000, 16, 2, min * 5);

		this.va = va;
		byte[] buf = new byte[min];
		int c;

		ar.startRecording();
		while (true) {
			c = ar.read(buf, 0, buf.length);
			if (c > 0) {
				va.process(buf, 0, c);
			}
		}
		// ar.stop();
	}
	
	Runnable timerRunnable = new Runnable() {
		
		@Override
		public void run() {
			activationReset();
			speechReset(-1);
			finish = true;
		}
	};
	
	///////////////////////////////////////////////////////////activation/////////////////////////////////////////////
	@Override
	public void onAwake() {
		Log.d(TAG, "onAwake");
	}

	@Override
	public void onAwakeNoCmd() {
		Log.d(TAG, "onAwakeNoCmd");
	}

	@Override
	public void onSleep() {
		Log.d(TAG, "onSleep");
		speechReset(-1);
	}

	@Override
	public void onVadCancel() {
		Log.d(TAG, "onVadCancel");
		isAutoStart = false;
		speechReset(-1);
	}
	
	@Override
	public void onVadComing(float sl) {
		Log.d(TAG, "onVadComing: location = " + sl);
	}

	@Override
	public void onVadData(byte[] data) {
		Log.d(TAG, "onVadData: data is " + data.length + " bytes");
		if(session > 0)
			mSpeech.putVoice(session, data);
	}

	@Override
	public void onVadEnd() {
		Log.d(TAG, "onVadEnd");
		if(session > 0)
			mSpeech.endVoice(session);
		session = -1;
	}

	@Override
	public void onVadStart(float energy, float threshold) {
		Log.d(TAG, "onVadStart: energy = " + energy + ", energy threshold = " + threshold);
		if(session == -1){
			finish = false;
			VoiceOptions options = new VoiceOptions();
			if(isAutoStart){
				options.voice_trigger = word;
				options.trigger_start = offset;
				options.trigger_length = length;
			}
			JSONObject jsonObject = new JSONObject();
			try {
				jsonObject.put("threshold_energy", threshold);
			} catch (JSONException e) {
				e.printStackTrace();
			}
			options.stack = appId + ":";
			options.voice_extra = jsonObject.toString();
			session = mSpeech.startVoice(this, options);
			
			mHandler.postDelayed(timerRunnable, 3000);
			Log.d(TAG, "onVadStart: sses = " + session);
		}
	}

	@Override
	public void onVoiceTrigger(String word, int offset, int end, float energy) {
		Log.d(TAG, "onVoiceTrigger: word = " + word + ", start = " + offset + ", end = " + end + ", energy = " + energy);
		this.word = word;
		this.offset = offset;
		length = end - offset;
		isAutoStart = true;
	}
	//////////////////////////////////////////////END////////////////////////////////////////////////////
	
	/////////////////////////////////////////////speech//////////////////////////////////////////////////
	@Override
	public void onAsrComplete(int session, String asr) {
		Log.d(TAG, "onAsrComplete: asr \t" + asr);
		finish = true;
		activationReset();
		speechReset(session);
	}

	@Override
	public void onCancel(int session) {
		Log.d(TAG, "onCancel: id \t" + session);
	}

	@Override
	public void onComplete(int session, final String nlp, final String action) {
		Log.d(TAG, "onComplete: id \t" + session);
		Log.d(TAG, "onComplete: nlp \t" + nlp);
		Log.d(TAG, "onComplete: action \t" + action);
		if(!finish){
			finish = true;
			activationReset();
			return;
		}
		new Thread(new Runnable() {
			@Override
			public void run() {
				try {
					JSONObject jsonObject = new JSONObject(nlp).getJSONObject("slots");
					if(jsonObject.has("tts")){
						String string = jsonObject.getJSONObject("tts").getString("value");
						mTts.speak(string, SpeechService.this);
					}
					JSONObject actionJson = new JSONObject(action);
					appId = actionJson.getString("appId");
					if(MUSIC_APPID.equals(appId)){
						JSONArray directives = actionJson.getJSONObject("response").getJSONObject("action").getJSONArray("directives");
						String action = directives.getJSONObject(1).getString("action");
						switch (action) {
						case "PLAY":
							String url = directives.getJSONObject(1).getJSONObject("item").getString("url");
							Log.d(TAG, "url\t" + url);
							mMediaPlayer.reset();
							mMediaPlayer.setDataSource(url);
							mMediaPlayer.prepare();
							mMediaPlayer.start();
							break;
						case "PAUSE":
							if(mMediaPlayer.isPlaying()){
								mMediaPlayer.pause();
							}
							break;
						case "RESUME":
							if(!mMediaPlayer.isPlaying()){
								mMediaPlayer.start();
							}
							break;
						case "STOP":
							if(mMediaPlayer.isPlaying()){
								mMediaPlayer.stop();
							}
							break;
						}
					}
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		}).start();
	}

	@Override
	public void onError(int session, int errCode) {
		Log.d(TAG, "onError: id \t" + session + ", errCode \t" + errCode);
		activationReset();
		speechReset(session);
	}

	@Override
	public void onIntermediateResult(int session, String asr, String extra) {
		Log.d(TAG, "onIntermediateResult: id \t" + session + ", asr \t" + asr + ", extra \t" + extra);
		if(extra != null && (extra.contains("fake") || extra.contains("reject"))){
			activationReset();
			speechReset(session);
		}else{
			mHandler.removeCallbacks(timerRunnable);
			mHandler.postDelayed(timerRunnable, 6000);
		}
	}

	@Override
	public void onStart(int session) {
		Log.d(TAG, "onStart: id \t" + session);
	}
	////////////////////////////////////////////////END/////////////////////////////////////////////////////
	
	////////////////////////////////////////////////tts/////////////////////////////////////////////////////
	@Override
	public void onComplete(int arg0) {
		
	}

	@Override
	public void onText(int arg0, String arg1) {
		
	}

	@Override
	public void onVoice(int arg0, byte[] arg1) {
		mOpusPlayer.play(arg1);
	}
	
	private void speechReset(int session){
		if((session == -1 || session == this.session) && !finish)
			mSpeech.cancel(this.session);
		this.session = -1;
		mHandler.removeCallbacks(timerRunnable);
	}
	
	private void activationReset(){
		isAutoStart = false;
		va.control(State.SLEEP);
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

	private static final String TAG = "VoiceActivation.example";
	private static final String OPENVOICE_CONFIG_FILE = "openvoice_profile.json";
	private static final String TTS_CONFIG_FILE = "tts_profile.json";
	private static final String MUSIC_APPID = "R233A4F187F34C94B93EE3BAECFCE2E3";
	
	private VoiceActivation va = null;
	
	private Speech mSpeech = null;
	private int session = -1;
	
	private Tts mTts = null;
	private OpusPlayer mOpusPlayer = null;
	
	private Handler mHandler = new Handler();
	private MediaPlayer mMediaPlayer = null;
	private OutputStream out = null;
	
	private boolean isAutoStart = false;
	private boolean finish = false;
	private String appId = "";
	private String word = "";
	private int offset = 0;
	private int length = 0;
}