package com.maxst.mkit;

public class Listener {
	public interface MKitCheckedChangeListener {
		void onCheckedChanged(String controlId, boolean isChecked);
	}

	public interface MKitClickListener {
		void onClick(String controlId, int resId);
	}

	public interface MKitEventListener {
		void onEvent(int eventId);
	}

	public static final int ENGINE_STARTED = 0;

}
