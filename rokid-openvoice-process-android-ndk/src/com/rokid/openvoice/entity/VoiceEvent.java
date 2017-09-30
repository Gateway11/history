package com.rokid.openvoice.entity;

public class VoiceEvent {
	private int id;
	private int event;
	private double sl;
	private double energy;

	public VoiceEvent(int id, int event, double sl, double energy) {
		this.id = id;
		this.event = event;
		this.sl = sl;
		this.energy = energy;
	}

	public int getId() {
		return id;
	}

	public int getEvent() {
		return event;
	}

	public double getSl() {
		return sl;
	}

	public double getEnergy() {
		return energy;
	}

	@Override
	public String toString() {
		return "VoiceEvent [id=" + id + ", event=" + event + ", sl=" + sl
				+ ", energy=" + energy + "]";
	}
}
