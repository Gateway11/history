package com.rokid.openvoice;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

import org.json.JSONException;
import org.json.JSONObject;

import android.content.Context;
import android.content.Intent;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Bundle;
import android.os.Handler;
import android.os.RemoteException;
import android.util.Log;

import com.rokid.openvoice.VoiceManager;
import com.rokid.voicerec.BearKid;
import com.rokid.voicerec.BearKidResult;
import com.rokid.voicerec.CustomWord;

public class VoiceService extends android.app.Service{

    private String TAG = getClass().getSimpleName();
    
    public static final int MSG_VOICE_EVENT             	= 0;
    public static final int MSG_INTERMEDIATE_RESULT			= 1;
    public static final int MSG_VOICE_COMMAND           	= 2;
    public static final int MSG_SPEECH_ERROR            	= 3;
    
    private static final int VOICE_COMING               	= 0;
    private static final int VOICE_LOCAL_WAKE           	= 1;
    private static final int VOICE_START                	= 2;
    private static final int VOICE_DATA           	        = 3;
    private static final int VOICE_ACCEPT               	= 4;
    private static final int VOICE_REJECT               	= 5;
    private static final int VOICE_CANCEL               	= 6;
    private static final int VOICE_LOCAL_SLEEP       		= 7;
    
    private static final int TYPE_INTERMEDIATE_ASR       	= 0;
    private static final int TYPE_FINISHED_ASR       		= 2;

    private static final int SPEECH_SERVER_INTERNAL     	= 6;
    private static final int SPEECH_VAD_TIMEOUT         	= 7;
    private static final int SPEECH_SERVICE_UNAVAILABLE 	= 101;
    private static final int SPEECH_TIMEOUT             	= 103;

    private Lock lock = new ReentrantLock();
    private Condition condition = lock.newCondition();
    private static SkillOptionHelper mSkillOption = new SkillOptionHelper();
    private LinkedList<BearKidResult> results = new LinkedList<BearKidResult>();
    
    public Handler mHandler = new Handler(){
    	public void handleMessage(android.os.Message msg) {
    		switch (msg.what) {
			case MSG_VOICE_EVENT:{
				Bundle data = msg.getData();
				handleVoiceEvent(data.getInt("id"), data.getInt("event"), data.getDouble("sl"), data.getDouble("energy"));
			}
				break;
			case MSG_INTERMEDIATE_RESULT:{
				Bundle data = msg.getData();
				handleIntermediateResult(data.getInt("id"), data.getInt("type"), data.getString("asr"));
			}
				break;
			case MSG_VOICE_COMMAND:{
				Bundle data = msg.getData();
				handleVoiceCommand(data.getInt("id"), data.getString("asr"), data.getString("nlp"), data.getString("action"));
			}
				break;
			case MSG_SPEECH_ERROR:{
				handleSpeechError(msg.arg1, msg.arg2);
			}
				break;
			}
    	}
    };

	public VoiceService(){
		Log.e(TAG, "VoiceService  created ");
        VoiceManager.registCallback(new VoiceCallback(mHandler));
	}

	@Override
	public void onCreate(){
		mSkillOption.bindSkills();
		
		ConnectivityManager cm = (ConnectivityManager)getSystemService(Context.CONNECTIVITY_SERVICE);
		NetworkInfo mNetworkInfo = cm.getActiveNetworkInfo();
		if(mNetworkInfo != null){
		    VoiceManager.networkStateChange(true);
		}
//		mUEventObserver.startObserving("/sound/card1/pcmC1D0c");
	}

	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
		String cmd = intent.getStringExtra("cmd");
		if (cmd == null)
			return START_NOT_STICKY;
		if (cmd.equals("awake")) {
			VoiceManager.setSirenState(VoiceManager.SIREN_STATE_AWAKE);
		} else if (cmd.equals("sleep")) {
			VoiceManager.setSirenState(VoiceManager.SIREN_STATE_SLEEP);
		} else if (cmd.equals("updateStack")) {
			String cdomain = intent.getStringExtra("cdomain");
			if (cdomain == null)
				cdomain = "";
			String sdomain = intent.getStringExtra("sdomain");
			if (sdomain == null)
				sdomain = "";

			if (cdomain == null && sdomain == null)
				VoiceManager.updateStack("");
			else if (cdomain == null && sdomain != null)
				Log.i(TAG, "update stack: invalid arguments, cdomain = " + cdomain + ", sdomain = " + sdomain);
			else
				VoiceManager.updateStack(cdomain + ":" + (sdomain == null ? "" : sdomain));
		}

		return START_NOT_STICKY;
	}
	
	private void handleVoiceEvent(int id, int event, double sl, double energy){
		BearKidResult newResult = new BearKidResult();
		switch (event) {
		case VOICE_COMING:
			newResult.type = BearKidResult.TYPE_LOCATION;
			newResult.location = (float)sl;
			break;
//		case VOICE_LOCAL_WAKE:
//			newResult.type = BearKidResult.TYPE_EXCEPTION;
//			newResult.extype = 0;
//			break;
		case VOICE_DATA:
			newResult.type = BearKidResult.TYPE_VOICE_INFO;
			newResult.energy = energy;
			break;
		case VOICE_ACCEPT:
			newResult.type = BearKidResult.TYPE_ACTIVATION;
			newResult.activation = 0;
			break;
		case VOICE_REJECT:
			newResult.type = BearKidResult.TYPE_ACTIVATION;
			newResult.activation = 1;
			break;
		case VOICE_CANCEL:
			newResult.type = BearKidResult.TYPE_EXCEPTION;
			newResult.extype = 2;
			break;
		case VOICE_LOCAL_SLEEP:
			newResult.type = BearKidResult.TYPE_DEACTIVE;
			break;
		default:
			return;
		}
		addToTarget(newResult);
	}
	
	private void handleIntermediateResult(int id, int type, String asr){
		BearKidResult newResult = new BearKidResult();
		switch (type) {
		case TYPE_INTERMEDIATE_ASR:
			newResult.type = BearKidResult.TYPE_INTERMEDIATE;
			break;
		case TYPE_FINISHED_ASR:
			newResult.type = BearKidResult.TYPE_ASR;
			break;
		default:
			return;
		}
		newResult.asr = asr;
		addToTarget(newResult);
	}
	
	private void handleVoiceCommand(int id, String asr, String nlp, String action){
		BearKidResult newResult = new BearKidResult();
		newResult.type = BearKidResult.TYPE_NLP;
		newResult.nlp = nlp;
		newResult.action = action;
		addToTarget(newResult);
	}

	private void handleSpeechError(int id, int errcode){
		BearKidResult newResult = new BearKidResult();
		newResult.type = BearKidResult.TYPE_EXCEPTION;
		switch (errcode) {
		case SPEECH_TIMEOUT:
			newResult.extype = 3;
			break;
		default:
			newResult.extype = 4;
		}
		addToTarget(newResult);
	}
	/**
	 * jni call
	 */
	private String getSkillOptions(){
		String skillOptions = mSkillOption.getSkillOptions();
		JSONObject skillJSONObject = new JSONObject();
		try{
			JSONObject jsonObject = new JSONObject(skillOptions);
			skillJSONObject.put("application", jsonObject);
		}catch(JSONException e){ 
			e.printStackTrace();
		}   
		return skillJSONObject.toString();
	}
	
	private void addToTarget(BearKidResult newResult){
		lock.lock();
		try {
			Iterator<BearKidResult> iterator = results.iterator();
			while(iterator.hasNext()){
				BearKidResult result = iterator.next();
				if(result.type == newResult.type){
					iterator.remove();
					break;
				}
			}
			results.addLast(newResult);
//			Iterator<BearKidResult> iterator2 = results.iterator();
//			while(iterator2.hasNext()){
//				BearKidResult result = iterator2.next();
//				Log.e(TAG, result.toString());
//			}
//			Log.e(TAG, "\n");
			condition.signal();
		} finally{
			lock.unlock();
		}
	}

	private final BearKid.Stub bearKid = new BearKid.Stub(){

		@Override
		public void control(int action) throws RemoteException {
			switch (action) {
			case VoiceManager.SIREN_STATE_AWAKE:
				VoiceManager.setSirenState(VoiceManager.SIREN_STATE_AWAKE);
				break;
			case VoiceManager.SIREN_STATE_SLEEP:
				VoiceManager.setSirenState(VoiceManager.SIREN_STATE_SLEEP);
				break;
			case VoiceManager.SIREN_STATE_STOP:
				VoiceManager.startSiren(false);
				break;
			case VoiceManager.SIREN_STATE_START:
				VoiceManager.startSiren(true);
				break;
			}
		}

		@Override
		public int getState() throws RemoteException {
			return 0;
		}

		@Override
		public BearKidResult poll() throws RemoteException {
			BearKidResult result = null;
			lock.lock();
			try {
				while (results.isEmpty())
					condition.await();
				result = results.getFirst();
				results.removeFirst();
			}catch (InterruptedException e) {
				e.printStackTrace();
			}finally{
				lock.unlock();
			}
			return result;
		}

		@Override
		public int addCustomWord(int type, String word, String pinyin) throws RemoteException {
			int result = -1;
			if(VoiceManager.isValidVtType(type) 
					&&word != null && pinyin != null && word.length() > 0 && pinyin.length() > 0){
				result =  VoiceManager.addVtWord(new VoiceManager.VtWord(type, word, pinyin));
				if(result != VoiceManager.VT_WORD_OK){
					result = -1;
				}
			}
			return result;
		}

		@Override
		public int removeCustomWord(String word) throws RemoteException {
			int result = -1;
			if(word != null && word.length() > 0){
				result = VoiceManager.removeVtWord(word);
				if(result != VoiceManager.VT_WORD_OK){
					result = -1;
				}
			}
			return result;
		}

		@Override
		public List<CustomWord> queryCustomWord(int type) throws RemoteException {
			List<CustomWord> vtWords = new ArrayList<CustomWord>();
			
			if(VoiceManager.isValidVtType(type) || type == -1){
				ArrayList<VoiceManager.VtWord> result = VoiceManager.getVtWords();
				if(result != null) {
					for (VoiceManager.VtWord vtWord : result) {
						if(type == -1 || type == vtWord.vt_type){
							vtWords.add(new CustomWord(vtWord.vt_type, vtWord.vt_word, vtWord.vt_pinyin));
						}
					}
				}
			}
			return vtWords;
		}
	};
	
//	private final android.os.UEventObserver mUEventObserver = new android.os.UEventObserver() {
//	 
//	@Override
//	public void onUEvent(android.os.UEventObserver.UEvent event){
//		Log.e(TAG, event.toString());
//		String action = event.get("ACTION");
//		if("add".equals(action)){
//			VoiceManager.startSiren(true);	
//		}else if("remove".equals(action)){
//			VoiceManager.startSiren(false);
//		}
//	}
//};
	
	@Override
	public android.os.IBinder onBind(Intent intent) {
		return bearKid;
	}
}
