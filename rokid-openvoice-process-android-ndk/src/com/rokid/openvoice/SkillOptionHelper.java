package com.rokid.openvoice;

import java.util.Vector;

public class SkillOptionHelper {

	public static final String SKILL_CUT = "cut";
	public static final String SKILL_SCENE = "scene";
	Vector<SkillServiceConnection> skills = new Vector<SkillServiceConnection>();

	public void bindSkills() {
		skills.add(new SkillServiceConnection(SKILL_CUT,
				"com.rokid.cloudappclient",
				"com.rokid.cloudappclient.aidl.CloudCutService"));
		skills.add(new SkillServiceConnection(SKILL_SCENE,
				"com.rokid.cloudappclient",
				"com.rokid.cloudappclient.aidl.CloudSceneService"));
	}

	String getSkillOptions() {
		StringBuffer skillOptions = new StringBuffer("{");
		int count = 0;
		for (SkillServiceConnection skillObj : skills) {
			String skill = skillObj.getSkillOption();
			if (skill != null && skill.length() > 0) {
				if (count++ != 0) {
					skillOptions.append(",");
				}
				skillOptions.append(skill);
			}
		}
		skillOptions.append("}");
		return skillOptions.toString();
	}
}
