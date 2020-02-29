package com.maxst.mkit;

import android.content.Context;
import android.util.Log;

import com.kt.renderobject.ARComplexViewer;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.Objects;

/**
 * Class for jni interfacing.
 */
public class MKitJniHelper {

	private static final String TAG = "MKitJniHelper";

	private static MKitJniHelper instance = new MKitJniHelper();

	public static MKitJniHelper getInstance() {
		return instance;
	}

	/**
	 * 증강된 컨텐츠에 해당하는 클릭리스너를 매핑하여 저장
	 */
	private HashMap<String, Listener.MKitClickListener> clickListenerHashMap = new HashMap<>();

	/**
	 * 라디오 버튼 체크 리스너를 매핑하여 저장
	 */
	private HashMap<String, Listener.MKitCheckedChangeListener> checkedChangeListenerHashMap = new HashMap<>();

	/**
	 * 트래킹 결과 콜백 리스너 저장
	 */
	private ArrayList<TrackingResultCallback> trackingResultCallbacks = new ArrayList<>();

	/**
	 * 게임 엔진 이벤트 리스너
	 */
	private Listener.MKitEventListener eventListener = null;

	private MKitJniHelper() {
	}

	public void setEventListener(Listener.MKitEventListener listener) {
		eventListener = listener;
	}

	/**
	 * 트래킹 콜백 리스너 추가.
	 *
	 * @param callback 콜백
	 */
	public void addTrackingResultCallback(TrackingResultCallback callback) {
		trackingResultCallbacks.add(callback);
	}

	/**
	 * 트래킹 콜백 리스너 삭제.
	 *
	 * @param callback 콜백
	 */
	public void removeTrackingResultCallback(TrackingResultCallback callback) {
		for (TrackingResultCallback l : trackingResultCallbacks) {
			if (callback.equals(l)) {
				trackingResultCallbacks.remove(l);
				break;
			}
		}
	}

	// FIXME : Maxst
	// region -- Form sample
	public void addLayout1(final String layoutName, final String textImagePath,
						   final String checkControlId, Listener.MKitCheckedChangeListener checkedChangeListener,
						   final String buttonControlId, Listener.MKitClickListener clickListener,
						   final float positionX, final float positionY, final float positionZ,
						   final float scaleX, final float scaleY, final float scaleZ,
						   final float width, final float height) {
		addClickListener(buttonControlId, clickListener);
		addCheckedChangeListener(checkControlId, checkedChangeListener);
		MKitJni.addLayout1(layoutName, textImagePath, checkControlId, buttonControlId,
				positionX, positionY, positionZ,
				scaleX, scaleY, scaleZ,
				width, height);
	}
	// endregion -- Form sample


	public void addChildLayout(final String layoutName, final String textImagePath,
							   final String checkControlId, Listener.MKitCheckedChangeListener checkedChangeListener,
							   final String buttonControlId, Listener.MKitClickListener clickListener,
							   final float positionX, final float positionY, final float positionZ,
							   final float scaleX, final float scaleY, final float scaleZ,
							   final float width, final float height, final boolean isChild) {
		addClickListener(buttonControlId, clickListener);
		addCheckedChangeListener(checkControlId, checkedChangeListener);
		MKitJni.addChildLayout(layoutName, textImagePath, checkControlId, buttonControlId,
				positionX, positionY, positionZ,
				scaleX, scaleY, scaleZ,
				width, height, isChild);
	}

	public void addTagLayout(final String layoutName, final String textImagePath,
							 final String buttonControlId, Listener.MKitClickListener clickListener,
							 final float positionX, final float positionY, final float positionZ,
							 final float scaleX, final float scaleY, final float scaleZ,
							 final float width, final float height) {
		addClickListener(buttonControlId, clickListener);
		MKitJni.addTagLayout(layoutName, textImagePath, buttonControlId,
				positionX, positionY, positionZ,
				scaleX, scaleY, scaleZ,
				width, height);
	}

	public void addIconLayout(final String layoutName, final String iconPath,
							  final float positionX, final float positionY, final float positionZ,
							  final float scaleX, final float scaleY, final float scaleZ,
							  final float width, final float height) {

		MKitJni.addIconLayout(layoutName, iconPath,
				positionX, positionY, positionZ,
				scaleX, scaleY, scaleZ,
				width, height);
	}

	public void addTextLayout(final String layoutName, final String textImagePath, String controlId,
							  Listener.MKitClickListener clickListener,
							  final float positionX, final float positionY, final float positionZ,
							  final float scaleX, final float scaleY, final float scaleZ,
							  final float width, final float height) {
		if (clickListener != null) {
			addClickListener(controlId, clickListener);
		}

		MKitJni.addTextLayout(layoutName, textImagePath, controlId,
				positionX, positionY, positionZ,
				scaleX, scaleY, scaleZ, width, height);
	}

	public void addLayout2(final String layoutName, final String textImagePath, final String checkControlId, Listener.MKitCheckedChangeListener checkedChangeListener,
						   final String buttonControlId, Listener.MKitClickListener clickListener,
						   final float positionX, final float positionY, final float positionZ,
						   final float scaleX, final float scaleY, final float scaleZ) {
		addClickListener(buttonControlId, clickListener);
		addCheckedChangeListener(checkControlId, checkedChangeListener);
		MKitJni.addLayout2(layoutName, textImagePath, checkControlId, buttonControlId,
				positionX, positionY, positionZ,
				scaleX, scaleY, scaleZ);
	}

	public void addLayout3(final String layoutName,
						   final String checkControlId, Listener.MKitCheckedChangeListener checkedChangeListener,
						   final String buttonControlId, Listener.MKitClickListener clickListener,
						   final float positionX, final float positionY, final float positionZ,
						   final float scaleX, final float scaleY, final float scaleZ) {
		addClickListener(buttonControlId, clickListener);
		addCheckedChangeListener(checkControlId, checkedChangeListener);
		MKitJni.addLayout3(layoutName, checkControlId, buttonControlId,
				positionX, positionY, positionZ,
				scaleX, scaleY, scaleZ);
	}

	/**
	 * 이미지 컨텐츠를 렌더링 하기 위해 게임엔진에 추가.
	 *
	 * @param imageName 식별을 위한 이미지 이름
	 * @param filePath  이미지 저장 경로
	 * @param positionX x위치
	 * @param positionY y위치
	 * @param positionZ z위치
	 * @param rotationX x축 회전
	 * @param rotationY y축 회전
	 * @param rotationZ z축 회전
	 * @param scaleX    x축 스케일 사이즈
	 * @param scaleY    y축 스케일 사이즈
	 * @param scaleZ    z축 스케일 사이즈
	 */
	public void addImage(final String imageName, final String filePath,
						 final float positionX, final float positionY, final float positionZ,
						 final float rotationX, final float rotationY, final float rotationZ,
						 final float scaleX, final float scaleY, final float scaleZ) {
		MKitJni.addImage(imageName, filePath,
				positionX, positionY, positionZ,
				rotationX, rotationY, rotationZ,
				scaleX, scaleY, scaleZ,
				false);
	}

	public void addImage(final String imageName, final String filePath,
						  final float positionX, final float positionY, final float positionZ,
						  final float rotationX, final float rotationY, final float rotationZ,
						  final float scaleX, final float scaleY, final float scaleZ,
						  final boolean modulateAlpha) {
		MKitJni.addImage(imageName, filePath,
				positionX, positionY, positionZ,
				rotationX, rotationY, rotationZ,
				scaleX, scaleY, scaleZ,
				modulateAlpha);
	}

	public void addImageAndAlpha(final String controlId, final String filePath,
						 		final float positionX, final float positionY, final float positionZ,
						 		final float rotationX, final float rotationY, final float rotationZ,
						 		final float scaleX, final float scaleY, final float scaleZ,
						 		final float alpha) {
		MKitJni.addImageAndAlpha(controlId, filePath,
							positionX, positionY, positionZ,
							rotationX, rotationY, rotationZ,
							scaleX, scaleY, scaleZ,
							alpha);
	}


	// FIXME : Maxst
	// region -- Form sample
	/**
	 * 이미지 컨텐츠를 렌더링 하기 위해 게임엔진에 추가.
	 *
	 * @param imageName 식별을 위한 이미지 이름
	 * @param filePath  이미지 저장 경로
	 * @param imageWidth 이미지 가로 사이즈
	 * @param imageHeight 이미지 세로 사이즈
	 * @param positionX x위치
	 * @param positionY y위치
	 * @param positionZ z위치
	 * @param scaleX    x축 스케일 사이즈
	 * @param scaleY    y축 스케일 사이즈
	 * @param scaleZ    z축 스케일 사이즈
	 */
	public void addImageLayout(final String imageName, final String filePath, final float imageWidth, final float imageHeight,
							   String controlId, Listener.MKitClickListener clickListener,
							   final float positionX, final float positionY, final float positionZ,
							   final float scaleX, final float scaleY, final float scaleZ) {
		if (clickListener != null) {
			addClickListener(controlId, clickListener);
		}
		MKitJni.addImageLayout(imageName, filePath, imageWidth, imageHeight,
				controlId, positionX, positionY, positionZ,
				scaleX, scaleY, scaleZ);
	}

	public void addImagePagesLayoutAndAlpha(final String controlId, String[] lst, final float imageWidth, final float imageHeight,
											Listener.MKitClickListener clickListener,
											final float positionX, final float positionY, final float positionZ,
											final float rotX, final float rotY, final float rotZ,
											final float scaleX, final float scaleY, final float scaleZ,
											final float alpha) {
		if (clickListener != null) {
			addClickListener(controlId, clickListener);
		}
        MKitJni.addImagePagesLayoutAndAlpha("imagePages", controlId, lst, imageWidth, imageHeight,
                positionX, positionY, positionZ,
                rotX, rotY, rotZ,
                scaleX, scaleY, scaleZ,
                alpha);
	}

	public void changeImagePage(final String controlId, final int opt) {

		MKitJni.changeImagePage(controlId, opt);
	}

	// endregion -- Form sample

	public void addImageLayoutAndAlpha(final String controlId, final String filePath, final float imageWidth, final float imageHeight,
							   Listener.MKitClickListener clickListener,
							   final float positionX, final float positionY, final float positionZ,
							   final float rotX, final float rotY, final float rotZ,
							   final float scaleX, final float scaleY, final float scaleZ,
							   final float alpha) {
		if (clickListener != null) {
			addClickListener(controlId, clickListener);
		}
		MKitJni.addImageLayoutAndAlpha("imageLayout", controlId, filePath, imageWidth, imageHeight,
				positionX, positionY, positionZ,
				rotX, rotY, rotZ,
				scaleX, scaleY, scaleZ,
				alpha);
	}

	public void addImageLayoutChange(final String nodeId, final String imageName, final String filePath, final float imageWidth, final float imageHeight,
							   String controlId, Listener.MKitClickListener clickListener,
							   final float positionX, final float positionY, final float positionZ,
							   final float scaleX, final float scaleY, final float scaleZ) {
		if (clickListener != null) {
			addClickListener(controlId, clickListener);
		}
		MKitJni.addImageLayoutChange(nodeId, imageName, filePath, imageWidth, imageHeight,
				controlId, positionX, positionY, positionZ,
				scaleX, scaleY, scaleZ);
	}

	public void changeImageLayout(final String nodeId, final String imageName, final String filePath) {
		MKitJni.changeImageLayout(nodeId, imageName, filePath);
	}


	// muiscen : jckim  added : 2019/10/7

	/**
	 * 이미지 컨텐츠를 렌더링 하기 위해 게임엔진에 추가.
	 *
	 * @param imageName 식별을 위한 이미지 이름
	 * @param filePath  이미지 저장 경로
	 * @param positionX x위치
	 * @param positionY y위치
	 * @param positionZ z위치
	 * @param rotationX x축 회전
	 * @param rotationY y축 회전
	 * @param rotationZ z축 회전
	 * @param scaleX    x축 스케일 사이즈
	 * @param scaleY    y축 스케일 사이즈
	 * @param scaleZ    z축 스케일 사이즈
	 */
	public void addImageLayout1(final String imageName, final String filePath, final float imageWidth, final float imageHeight,
								String controlId, Listener.MKitClickListener clickListener,
								final float positionX, final float positionY, final float positionZ,
								final float rotationX, final float rotationY, final float rotationZ,

								final float scaleX, final float scaleY, final float scaleZ) {
		if (clickListener != null) {
			addClickListener(controlId, clickListener);
		}
		//Log.e("imagel", "2");

		MKitJni.addImageLayout1(imageName, filePath, imageWidth, imageHeight,
				controlId, positionX, positionY, positionZ,
				rotationX, rotationY, rotationZ,
				scaleX, scaleY, scaleZ);

		//Log.e("imagel", "3");

	}

	/**
	 * 비디오 컨텐츠를 렌더링 하기 위해 게임엔진에 추가.
	 * @param context	activity current context.
	 * @param filePath	비디오 저장 경로
	 * @param positionX x위치
	 * @param positionY y위치
	 * @param positionZ z위치
	 * @param rotationX x축 회전
	 * @param rotationY y축 회전
	 * @param rotationZ z축 회전
	 * @param scaleX    x축 스케일 사이즈
	 * @param scaleY    y축 스케일 사이즈
	 * @param scaleZ    z축 스케일 사이즈
	 */
	public void addVideo(final Context context,
						 final String filePath,
						 final float positionX, final float positionY, final float positionZ,
						 final float rotationX, final float rotationY, final float rotationZ,
						 final float scaleX, final float scaleY, final float scaleZ) {
		MKitJni.addVideo(context, filePath,
				positionX, positionY, positionZ,
				rotationX, rotationY, rotationZ,
				scaleX, scaleY, scaleZ);
	}

	/**
	 * 비디오 컨텐츠를 Form에 렌더링 하기 위해 게임엔진에 추가.
	 * @param context	activity current context.
	 * @param filePath	비디오 저장 경로
	 * @param positionX x위치
	 * @param positionY y위치
	 * @param positionZ z위치
	 * @param rotationX x축 회전
	 * @param rotationY y축 회전
	 * @param rotationZ z축 회전
	 * @param scaleX    x축 스케일 사이즈
	 * @param scaleY    y축 스케일 사이즈
	 * @param scaleZ    z축 스케일 사이즈
	 */
	public void addVideoLayout(final Context context,
							   final String controlId,
							   Listener.MKitClickListener clickListener,
						 final String filePath,
						 final float positionX, final float positionY, final float positionZ,
						 final float rotationX, final float rotationY, final float rotationZ,
						 final float scaleX, final float scaleY, final float scaleZ) {
		addClickListener(controlId, clickListener);
		MKitJni.addVideoLayout(context, controlId, filePath,
				positionX, positionY, positionZ,
				rotationX, rotationY, rotationZ,
				scaleX, scaleY, scaleZ);
	}

	public void addVideoAndAlpha(final Context context,
						 		final String controlId,
						 		final String filePath,
						 		final float positionX, final float positionY, final float positionZ,
						 		final float rotationX, final float rotationY, final float rotationZ,
						 		final float scaleX, final float scaleY, final float scaleZ,
								final float alpha) {
		MKitJni.addVideoAndAlpha(context, controlId, filePath,
								positionX, positionY, positionZ,
								rotationX, rotationY, rotationZ,
								scaleX, scaleY, scaleZ,
								alpha);
	}

	public void addWebViewAndAlpha(final Context context,
								 final String name, final String url,
								 final float positionX, final float positionY, final float positionZ,
								 final float rotationX, final float rotationY, final float rotationZ,
								 final float scaleX, final float scaleY, final float scaleZ,
								 final float alpha) {
		MKitJni.addWebViewAndAlpha(context, name, url,
				positionX, positionY, positionZ,
				rotationX, rotationY, rotationZ,
				scaleX, scaleY, scaleZ,
				alpha);
	}

	public void addGlobalMenuLayout(final Context context,
							   final String controlId,
							   Listener.MKitClickListener clickListener,
							   float[] pos, float[] rotation, float[] scale, float alpha) {
		addClickListener(controlId, clickListener);
		MKitJni.addGlobalMenuLayout(context, controlId,
				pos, rotation, scale, alpha);
	}

	public void showGlobalMenuLayout(final Context context,
									  final String controlId,
									  Listener.MKitClickListener clickListener,
									  float[] pos, float[] rotation, float[] scale, float alpha) {
		addClickListener(controlId, clickListener);
		MKitJni.showGlobalMenuLayout(context, controlId,
				pos, rotation, scale, alpha);
	}

	public void hideGlobalMenuLayout() {
		MKitJni.hideGlobalMenuLayout();
	}

	public void addSystemLayoutAndAlpha(final Context context, final String name, final int layoutId,
										float[] pos, float[] rotation, float[] scale, float alpha) {
		MKitJni.addSystemLayoutAndAlpha(context, name, layoutId,
				pos, rotation, scale, alpha);
	}

	public boolean hasSystemLayout(final Context context, final String name) {
		return MKitJni.hasSystemLayout(context, name);
	}

	public void removeSystemLayout(final Context context, final String name) {
		MKitJni.removeSystemLayout(context, name);
	}

	public void showSystemLayoutAndAlpha(final Context context, final String controlId, final int layoutId,
										 float[] pos, float[] rotation, float[] scale, float alpha) {
		MKitJni.showSystemLayoutAndAlpha(context, controlId, layoutId,
				pos, rotation, scale, alpha);
	}

	public void addCustomViewLayout(final Context context, final String controlId, final int layoutId,
									int[] size, float[] pos, float[] rotation, float[] scale, float alpha) {
		MKitJni.addCustomViewLayout(context, controlId, layoutId, size, pos, rotation, scale, alpha);
	}

	public void removeCustomViewLayout(final String controlId) {
		MKitJni.removeCustomViewLayout(controlId);
	}

	public void addARComplexViewer(final Context context, final String name, String controlId,
								   final Map<String, Listener.MKitClickListener> listenerMap,
								   ARComplexViewer viewer) {
		for( Map.Entry<String, Listener.MKitClickListener> elem : listenerMap.entrySet() ){
			addClickListener(elem.getKey(), elem.getValue());
		}
		MKitJni.addARComplexViewer(context, name, controlId, viewer);
	}

	public void changeContextMenu(String name, int menuType) {
		MKitJni.changeContextMenu(name, menuType);
	}

	public void setContextMenuBadge(String name, int count) {
		MKitJni.setContextMenuBadge(name, count);
	}

	public void changeItemIndex(String name, int itemIndex) {
		MKitJni.changeItemIndex(name, itemIndex);
	}

	public void changePage(String name, int op, int pageNo) {
		MKitJni.changePage(name, op, pageNo);
	}

	public void changeMediaPlayer(String name, int op) {
		MKitJni.changeMediaPlayer(name, op);
	}

	public void setEnableContextMenu(String name, boolean enable) {
		MKitJni.setEnableContextMenu(name, enable);
	}

	//type 0 : WAITING_DRAW_TYPE
	//type 1 : IMMEDIATELY_DRAW_TYPE
	public void show3DToast(final String msg, final int ms, final int type) {
		MKitJni.show3DToast(msg, ms, type);
	}


	public void createNode(final String nodeName, final boolean isChecked) {
		MKitJni.createNode(nodeName, isChecked);
	}

	public void addNode(final String str) {
		MKitJni.addNode(str);
	}

	public void addNodeMap(final String nodeName, final boolean isChecked) {
		MKitJni.addNodeMap(nodeName, isChecked);
	}

	/**
	 * Change check state of gameplay3d's CheckBox
	 *
	 * @param checkControlId : CheckBox control id
	 * @param isChecked      : true - checked, false - unchecked
	 */
	public void applyCheckState(final String checkControlId, final boolean isChecked) {
		MKitJni.applyCheckState(checkControlId, isChecked);
	}

	/**
	 * Add click listener for click event from gameplay3d's Button
	 *
	 * @param controlId
	 * @param clickListener
	 */
	private void addClickListener(String controlId, Listener.MKitClickListener clickListener) {
		clickListenerHashMap.put(controlId, clickListener);
	}

	/**
	 * Add check state change listener for check change event from gameplay3d's CheckBox
	 *
	 * @param controlId
	 * @param checkedChangeListener
	 */
	private void addCheckedChangeListener(String controlId, Listener.MKitCheckedChangeListener checkedChangeListener) {
		checkedChangeListenerHashMap.put(controlId, checkedChangeListener);
	}

	public void captureScene(boolean toggleCapture) {
		MKitJni.captureScene(toggleCapture);
	}

	/**
	 * Called from jni
	 *
	 * @param controlId Augmented object id
	 */
	public void onClick(String controlId, int resId) {
		Log.d(TAG, "onClick : " + controlId + ", resId : " + resId);
		if (clickListenerHashMap.containsKey(controlId)) {
			Objects.requireNonNull(clickListenerHashMap.get(controlId)).onClick(controlId, resId);
		}
	}

	/**
	 * Called from jni
	 *
	 * @param controlId Augmented object id
	 * @param isChecked checked state
	 */
	public void onCheckedChanged(String controlId, boolean isChecked) {
		Log.d(TAG, "onCheckedChanged : " + controlId + ", checked: " + isChecked);
		if (checkedChangeListenerHashMap.containsKey(controlId)) {
			Objects.requireNonNull(checkedChangeListenerHashMap.get(controlId)).onCheckedChanged(controlId, isChecked);
		}
	}

	/**
	 * Called from jni
	 */
	public void onTrackingFail() {
		for (TrackingResultCallback callback : trackingResultCallbacks) {
			callback.onTrackingFail();
		}
	}

	/**
	 * Called from jni
	 *
	 * @param id   : Trackable id
	 * @param name : Trackable name
	 */
	public void onTrackingSuccess(String id, String name) {
		for (TrackingResultCallback callback : trackingResultCallbacks) {
			callback.onTrackingSuccess(id, name);
		}
	}

	/**
	 * Called from jni
	 *
	 * @param eventId Event ID
	 */
	public void onEvent(int eventId) {
		Log.d(TAG, "onEvent : " + eventId);
		if (eventListener != null) {
			eventListener.onEvent(eventId);
		}
	}


	public void setGlobalMenuBadgeCount(String controlId, int count) {
		if (count <= 0) {
			MKitJni.setGlobalMenuBadgeVisible(controlId, false);
		} else {
			MKitJni.setGlobalMenuBadgeText(controlId, ""+count);
		}
	}

	public void addMainControlNode(final Context context, final String controlId, final int layoutId,
										 float[] pos, float[] rotation, float[] scale, float alpha) {
		MKitJni.addMainControlNode(context, controlId, layoutId,
				pos, rotation, scale, alpha);
	}

	public void removeMainControlNode() {
		MKitJni.removeMainControlNode();
	}

	public void addFixedSystemNode(final Context context, String name, String systemFormName,
								   int layoutId, float [] pos, float alpha) {
		MKitJni.addFixedSystemNode(context, name, systemFormName, layoutId, pos, alpha);
	}

	public void addFixedSharedImage(String layoutName, String controlId, String imagePath,
									int imageWidth, int imageHeight, boolean isSmall, float alpha) {
		MKitJni.addFixedSharedImage(layoutName, controlId, imagePath, imageWidth, imageHeight, isSmall, alpha);
	}

	public void removeFixedNode(String name) {
		MKitJni.removeFixedNode(name);
	}

	public void targetOnEnabled(boolean enable) {
		MKitJni.targetOnEnabled(enable);
	}
}