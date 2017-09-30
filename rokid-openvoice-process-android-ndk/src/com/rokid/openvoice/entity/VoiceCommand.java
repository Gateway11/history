package com.rokid.openvoice.entity;

public class VoiceCommand {
	private int id;
	private String asr;
	private String nlp;
	private String action;

	public VoiceCommand(int id, String asr, String nlp, String action) {
		this.id = id;
		this.asr = asr;
		this.nlp = nlp;
		this.action = action;
	}

	public int getId() {
		return id;
	}

	public String getAsr() {
		return asr;
	}

	public String getNlp() {
		return nlp;
	}

	public String getAction() {
		return action;
	}

	@Override
	public String toString() {
		return "VoiceCommand [id=" + id + ", asr=" + asr + ", nlp=" + nlp
				+ ", action=" + action + "]";
	}
}
