package com.example.hotfix;

import android.app.Activity;
import android.os.Bundle;
import android.widget.Toast;

public class MainActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		try {
			new Utils().patch(getApplicationContext());
		} catch (Exception e) {
			e.printStackTrace();
		}
		Toast.makeText(getApplicationContext(), "lalala ...", Toast.LENGTH_SHORT).show();
	}
}
