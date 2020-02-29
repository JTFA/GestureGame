/*
 * Copyright 2017 Maxst, Inc. All Rights Reserved.
 */

package com.maxst.mkit;

import android.app.Activity;
import android.content.Context;
import android.content.res.AssetManager;

import com.kt.renderobject.ARComplexViewer;

public class MKitJni {
    static {
        System.loadLibrary("MKitRenderer");
    }

	public native static void initParam(Activity activity, AssetManager assetManager, MKitJniHelper jniHelperInstance, String externalPath);

	public native static void createGame();

    public native static void destroyGame();

    public native static void surfaceCreated();

    public native static void surfaceChanged(int width, int height);

    public native static void renderFrame();

	public native static boolean isInitialize();

	public native static void changeRendererSize(int lw, int lh, int rw, int rh);

    public native static void pause();

    public native static void resume();

    public native static void destroy();

    public native static void touchDown(int action, float x, float y, int pointerId);

    public native static void touchUp(int action, float x, float y, int pointerId);

    public native static void touchMove(int action, float x, float y, int pointerId);

    public native static void setProjectionMatrix(float [] matrix);

    public native static void setTrackingResult(boolean isSuccess, String id, String name, float [] poseMatrix);

	public native static void setCameraQuaternion(float[] quaternion);

	// FIXME : Maxst Boolean 뺴버림
	// region -- Form sample
	native static void addLayout1(String layoutName, String textImagePath, String checkControlId, String buttonControlId,
                                  float positionX, float positionY, float positionZ,
								  float scaleX, float scaleY, float scaleZ, float width, float height);
	// endregion -- Form sample

	native static void addChildLayout(String layoutName, String textImagePath, String checkControlId, String buttonControlId,
	                                  float positionX, float positionY, float positionZ,
	                                  float scaleX, float scaleY, float scaleZ, float width, float height, boolean isChild);

    native static void addLayout2(String layoutName, String textImagePath, String checkControlId, String buttonControlId,
                                  float positionX, float positionY, float positionZ,
                                  float scaleX, float scaleY, float scaleZ);

	native static void addLayout3(String layoutName, String checkControlId, String buttonControlId,
	                              float positionX, float positionY, float positionZ,
	                              float scaleX, float scaleY, float scaleZ);

	native static void addCheckLayout(String layoutName, String textImagePath, String buttonControlId,
	                                  float positionX, float positionY, float positionZ,
	                                  float scaleX, float scaleY, float scaleZ);

	native static void addIconLayout(final String layoutName, final String iconPath,
	                                 final float positionX, final float positionY, final float positionZ,
	                                 final float scaleX, final float scaleY, final float scaleZ,
									 float width, float height);

	native static void addTextLayout(final String layoutName, final String textImagePath, String controlId,
	                                 final float positionX, final float positionY, final float positionZ,
	                                 final float scaleX, final float scaleY, final float scaleZ,
									 float width, float height);

	native static void addTagLayout(String layoutName, String textImagePath, String buttonControlId,
	                                float positionX, float positionY, float positionZ,
	                                float scaleX, float scaleY, float scaleZ, float width, float height);

	native static void addImage(String imageName, String filePath,
	                            float positionX, float positionY, float positionZ,
	                            float rotationX, float rotationY, float rotationZ,
	                            float scaleX, float scaleY, float scaleZ,
								boolean modulateAlpha);

	native static void addImageAndAlpha(String controlId, String filePath,
										float positionX, float positionY, float positionZ,
										float rotationX, float rotationY, float rotationZ,
										float scaleX, float scaleY, float scaleZ,
										float alpha);

	// FIXME : Maxst
	// region -- Form sample
	native static void addImageLayout(String layoutName, String filePath, float imageWidth, float imageHeight,
									  String controlId, float positionX, float positionY, float positionZ,
									  float scaleX, float scaleY, float scaleZ);

	native static void addImagePagesLayoutAndAlpha(String layoutName, String controlId, String[] lst, float imageWidth, float imageHeight,
												   float positionX, float positionY, float positionZ,
												   float rotX, float rotY, float rotZ,
												   float scaleX, float scaleY, float scaleZ, float alpha);

	native static void changeImagePage(String controlId, int opt);

	// endregion -- Form sample

	native static void addImageLayoutAndAlpha(String layoutName, String controlId, String filePath, float imageWidth, float imageHeight,
											  float positionX, float positionY, float positionZ,
											  float rotX, float rotY, float rotZ,
											  float scaleX, float scaleY, float scaleZ, float alpha);

	native static void addImageLayoutChange(String nodeId, String layoutName, String filePath, float imageWidth, float imageHeight,
										   String controlId, float positionX, float positionY, float positionZ,
										   float scaleX, float scaleY, float scaleZ);

    native static void changeImageLayout(String nodeId, String layoutName, String filePath);


	native static void addImageLayout1(String layoutName, String filePath, float imageWidth, float imageHeight,
									   String controlId, float positionX, float positionY, float positionZ,
									   float rotationX, float rotationY, float rotationZ,
									   float scaleX, float scaleY, float scaleZ);

	native static void addVideo(Context context, String filePath,
	                            float positionX, float positionY, float positionZ,
	                            float rotationX, float rotationY, float rotationZ,
	                            float scaleX, float scaleY, float scaleZ);

	native static void addVideoAndAlpha(Context context, String controlId, String filePath,
										float positionX, float positionY, float positionZ,
										float rotationX, float rotationY, float rotationZ,
										float scaleX, float scaleY, float scaleZ,
										float alpha);
	native static void addVideoLayout(Context context, String controlId, String filePath,
									  float positionX, float positionY, float positionZ,
									  float rotationX, float rotationY, float rotationZ,
									  float scaleX, float scaleY, float scaleZ);

	native static void addWebViewAndAlpha(Context context, String name, String url,
										float positionX, float positionY, float positionZ,
										float rotationX, float rotationY, float rotationZ,
										float scaleX, float scaleY, float scaleZ,
										float alpha);

	native static void addGlobalMenuLayout(Context context, String controlId,
										  float[] pos, float[] rotation, float[] scale,
										  float alpha);

	native static void showGlobalMenuLayout(Context context, String controlId,
										   float[] pos, float[] rotation, float[] scale,
										   float alpha);

	native static void hideGlobalMenuLayout();

	native static void addSystemLayoutAndAlpha(final Context context, final String name, final int layoutId,
											   float[] pos, float[] rotation, float[] scale, float alpha);
	native static boolean hasSystemLayout(final Context context, final String name);

	native static void removeSystemLayout(final Context context, final String name);

	native static void showSystemLayoutAndAlpha(final Context context, final String controlId, final int layoutId,
												float[] pos, float[] rotation, float[] scale, float alpha);
	native static void addCustomViewLayout(final Context context, final String controlId, final int layoutId,
										   int[] size, float[] pos, float[] rotation, float[] scale, float alpha);

	native static void removeCustomViewLayout(final String controlId);
	//template
	native static void addARComplexViewer(final Context context, final String name,
										  final String controlId, final ARComplexViewer viewer);

	native static void changeContextMenu(final String name, final int menuType);
	native static void setContextMenuBadge(final String name, final int count);

	native static void changeItemIndex(final String name, final int itemIndex);

	native static void changePage(final String name, final int op, final int pageNo); //0:prev 1:next 2:select

	native static void changeMediaPlayer(final String name, final int op);

	native static void setEnableContextMenu(final String name, final boolean enable);

	native static void show3DToast(final String msg, final int ms, final int type);

	public native static int getCurrentComplexMenu(final String name);

	public native static int getComplexCurrent(final String name);

	//template
	native static void addNode(String str);

	native static void createNode(String nodeName, boolean isChecked);

	native static void addNodeMap(String nodeName, boolean isChecked);

	native static void applyCheckState(String checkControlId, boolean isChecked);

	public native static void clearARContent();

	public native static void clearARContentWithId(String controlId);

	native static void captureScene(boolean toggleCapture);

	native static void setGlobalMenuBadgeText(String controlId, String text);

	native static void setGlobalMenuBadgeVisible(String controlId, boolean visible);

	native static void addMainControlNode(final Context context, final String name, final int layoutId,
										  float[] pos, float[] rotation, float[] scale, float alpha);

	native static void removeMainControlNode();

	native static void addFixedSystemNode(final Context context, String name, String systemFormName, int layoutId, float [] jpos, float jalpha);

	native static void addFixedSharedImage(String layoutName, String controlId,
										   String imagePath, int imageWidth, int imageHeight, boolean isSmall, float jalpha);

	native static void removeFixedNode(String name);

	native static void targetOnEnabled(boolean enable);

}
