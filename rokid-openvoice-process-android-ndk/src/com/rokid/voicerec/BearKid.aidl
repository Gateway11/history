package com.rokid.voicerec;

import com.rokid.voicerec.BearKidResult;
import com.rokid.voicerec.CustomWord;

interface BearKid {
	void control(int action);

	int getState();

	BearKidResult poll();
	
	int addCustomWord(int type, String word, String pinyin);

	int removeCustomWord(String word);

	List<CustomWord> queryCustomWord(int type);
}
