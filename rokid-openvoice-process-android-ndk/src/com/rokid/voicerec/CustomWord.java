package com.rokid.voicerec;

import android.os.Parcel;
import android.os.Parcelable;

public class CustomWord implements Parcelable {

	public int type;
	public String word;
	public String pinyin;
	
	public static final int TYPE_VT_WORD = 1;
	public static final int TYPE_EXIT_WORD = 2;
	public static final int TYPE_HOT_WORD = 3;

	public CustomWord(int type, String word, String pinyin) {
		this.type = type;
		this.word = word;
		this.pinyin = pinyin;
	}

	public CustomWord(Parcel reply) {
		type = reply.readInt();
		word = reply.readString();
		pinyin = reply.readString();
	}

	@Override
	public int describeContents() {
		return 0;
	}

	@Override
	public void writeToParcel(Parcel data, int flag) {
		data.writeInt(type);
		data.writeString(word);
		data.writeString(pinyin);
	}

	public static final Parcelable.Creator<CustomWord> CREATOR = new Creator<CustomWord>() {
		@Override
		public CustomWord[] newArray(int size) {
			return new CustomWord[size];
		}

		@Override
		public CustomWord createFromParcel(Parcel source) {
			return new CustomWord(source);
		}
	};

	@Override
	public String toString() {
		return "CustomWord [type=" + type + ", word=" + word + ", pinyin=" + pinyin + "]";
	}
}
