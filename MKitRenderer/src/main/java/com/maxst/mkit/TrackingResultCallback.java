package com.maxst.mkit;

public interface TrackingResultCallback {
	void onTrackingSuccess(String trackableId, String trackableName);
	default void onTrackingFail() {}
}
