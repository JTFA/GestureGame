//
// Created by Jack on 2018-04-13.
//

#ifdef __ANDROID__

#include <memory>

#include <jni.h>
#include "gameplay.h"
#include <LogUtils.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <AndroidVideoPlayer.h>
#include <AndroidHelper.h>
#include <MKitRendererManager.h>
#include <control/AndroidWebView.h>
#include <control/AndroidSystemLayout.h>
#include <model/ARComplexViewer.h>

#ifdef MAXST_CUSTOMIZE

extern void
initParam(JavaVM *jvm, jobject activity, AAssetManager *assetManager, char *externalPath);

extern void setViewPort(int width, int height);

extern void initGameTime();

#endif

extern "C"
{
static JavaVM *javaVM = 0;
static gameplay::MKitRendererManager *mKitRenderer = nullptr;
static bool __initialized;
static bool __suspended;

jobject stoJstring(JNIEnv *env, const char *pat) {
	jclass strClass = env->FindClass("java/lang/String");
	jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
	jbyteArray bytes = env->NewByteArray(strlen(pat));
	env->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte *) pat);
	jstring encoding = env->NewStringUTF("utf-8");
	jobject jobj = env->NewObject(strClass, ctorID, bytes, encoding);
	env->DeleteLocalRef(bytes);
	return jobj;
}

// jstring to char*
char *jstringTostring(JNIEnv *env, jstring jstr) {
	char *rtn = NULL;
	jclass clsstring = env->FindClass("java/lang/String");
	jstring strencode = env->NewStringUTF("utf-8");
	jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
	jbyteArray barr = (jbyteArray) env->CallObjectMethod(jstr, mid, strencode);
	jsize alen = env->GetArrayLength(barr);
	jbyte *ba = env->GetByteArrayElements(barr, JNI_FALSE);
	if (alen > 0) {
		rtn = (char *) malloc(alen + 1);
		memcpy(rtn, ba, alen);
		rtn[alen] = 0;
	}
	env->ReleaseByteArrayElements(barr, ba, 0);
	return rtn;
}

std::string jstring2String(JNIEnv *env, jstring jstr) {
	if (!jstr) return "";
	const char *pStr = env->GetStringUTFChars(jstr, NULL);//Java String to C Style string
	return pStr ? pStr : "";
}

// bind function
void setTransform(JNIEnv *env, jclass jcls_ARComplexViewer, jobject jobj_ARComplexViewer,
        std::shared_ptr<armakers::ARComplexViewer> viewer) {
	jfieldID fid_facilityName = env->GetFieldID(jcls_ARComplexViewer, "facilityName", "Ljava/lang/String;");
	jstring js_facilityName = (jstring)env->GetObjectField(jobj_ARComplexViewer, fid_facilityName);
	std::string facilityName = jstring2String(env, js_facilityName);

    jfieldID fid_pos = env->GetFieldID(jcls_ARComplexViewer, "pos", "[F");
    jfloatArray jfarr_pos = reinterpret_cast<jfloatArray>(env->GetObjectField(jobj_ARComplexViewer, fid_pos));

    jfieldID fid_scale = env->GetFieldID(jcls_ARComplexViewer, "scale", "[F");
    jfloatArray jfarr_scale = reinterpret_cast<jfloatArray>(env->GetObjectField(jobj_ARComplexViewer, fid_scale));

    jfieldID fid_rotation = env->GetFieldID(jcls_ARComplexViewer, "rotation", "[F");
    jfloatArray jfarr_rotation = reinterpret_cast<jfloatArray>(env->GetObjectField(jobj_ARComplexViewer, fid_rotation));

    float pos[3] = { 0.f, };
    float scale[3] = { 0.f, };
    float rotation[3] = { 0.f, };
    env->GetFloatArrayRegion(jfarr_pos, 0, 3, (jfloat *)pos);
    env->GetFloatArrayRegion(jfarr_scale, 0, 3, (jfloat *)scale);
    env->GetFloatArrayRegion(jfarr_rotation, 0, 3, (jfloat *)rotation);

    viewer->facilityName(facilityName);
    viewer->setPos(gameplay::Vector3(pos));
    viewer->setScale(gameplay::Vector3(scale));
    viewer->setRotation(gameplay::Vector3(rotation));
}

void setInformation(JNIEnv *env, jclass jcls_ARComplexViewer, jobject jobj_ARComplexViewer,
                  std::shared_ptr<armakers::ARComplexViewer> viewer) {
    jfieldID fid_alpha = env->GetFieldID(jcls_ARComplexViewer, "alpha", "F");
    jfloat jf_alpha = env->GetFloatField(jobj_ARComplexViewer, fid_alpha);

    jfieldID fid_currentMenuType = env->GetFieldID(jcls_ARComplexViewer, "currentMenuType", "I");
    jint ji_currentMenuType = env->GetIntField(jobj_ARComplexViewer, fid_currentMenuType);

    jfieldID fid_menuLayoutEnable = env->GetFieldID(jcls_ARComplexViewer, "menuLayoutEnable", "Z");
    jboolean jb_menuLayoutEnable = env->GetBooleanField(jobj_ARComplexViewer, fid_menuLayoutEnable);

    viewer->setAlpha(jf_alpha);
    viewer->setCurrentMenuType(ji_currentMenuType);
    viewer->setMenuLayoutEnable(jb_menuLayoutEnable);
}

void setARContextMenu(JNIEnv *env, jclass jcls_ARContextMenu, jobject jobj_ARContextMenu,
                      std::shared_ptr<armakers::ARContextMenu> menu) {
    jfieldID fid_type = env->GetFieldID(jcls_ARContextMenu, "type", "I");
    jint ji_type = env->GetIntField(jobj_ARContextMenu, fid_type);

    jfieldID fid_bgImagePath = env->GetFieldID(jcls_ARContextMenu, "bgImagePath", "Ljava/lang/String;");
    jstring js_bgImagePath = (jstring)env->GetObjectField(jobj_ARContextMenu, fid_bgImagePath);
    std::string bgImagepPath = jstring2String(env, js_bgImagePath);

    jfieldID fid_text = env->GetFieldID(jcls_ARContextMenu, "text", "Ljava/lang/String;");
    jstring js_text = (jstring)env->GetObjectField(jobj_ARContextMenu, fid_text);
    std::string text = jstring2String(env, js_text);

    menu->setType(ji_type);
    menu->setBgImagePath(bgImagepPath);
    menu->setText(text);
}

void setMenuList(JNIEnv *env, jclass jcls_ARComplexViewer, jobject jobj_ARComplexViewer,
                 std::shared_ptr<armakers::ARComplexViewer> viewer) {
    jclass jcls_ARContextMenu = env->FindClass("com/kt/renderobject/ARContextMenu");
    jfieldID fid_menus = env->GetFieldID(jcls_ARComplexViewer, "menus", "[Lcom/kt/renderobject/ARContextMenu;");
    jobjectArray joarr_menus = reinterpret_cast<jobjectArray>(env->GetObjectField(jobj_ARComplexViewer, fid_menus));
    jint size = env->GetArrayLength(joarr_menus);
    for (int i = 0; i < size; ++i) {
        std::shared_ptr<armakers::ARContextMenu> menu(new armakers::ARContextMenu());
        jobject jobj_ARContextMenu = env->GetObjectArrayElement(joarr_menus, i);
        setARContextMenu(env, jcls_ARContextMenu, jobj_ARContextMenu, menu);
        viewer->getMenus().push_back(menu);
        //LOGI("ARContextMenu (%d)(%s)(%s)", menu->getType(), menu->getBgImagePath().c_str(), menu->getText().c_str());
    }
}

void createARContentItem(JNIEnv *env, jobject context, jclass jcls_ARContentItem, jobject jobj_ARContentItem, armakers::ARContentItem **pItem) {
    jfieldID fid_type = env->GetFieldID(jcls_ARContentItem, "type", "I");
    jint ji_type = env->GetIntField(jobj_ARContentItem, fid_type);

    jfieldID fid_contentID = env->GetFieldID(jcls_ARContentItem, "contentID", "Ljava/lang/String;");
    jstring js_contentID = (jstring)env->GetObjectField(jobj_ARContentItem, fid_contentID);
    std::string contentID = jstring2String(env, js_contentID);

    jfieldID fid_menuTitle = env->GetFieldID(jcls_ARContentItem, "menuTitle", "Ljava/lang/String;");
    jstring js_menuTitle = (jstring)env->GetObjectField(jobj_ARContentItem, fid_menuTitle);
    std::string menuTitle = jstring2String(env, js_menuTitle);

    jfieldID fid_realFileName = env->GetFieldID(jcls_ARContentItem, "realFileName", "Ljava/lang/String;");
    jstring js_realFileName = (jstring)env->GetObjectField(jobj_ARContentItem, fid_realFileName);
    std::string realFileName = jstring2String(env, js_realFileName);

    jfieldID fid_currentIndex = env->GetFieldID(jcls_ARContentItem, "currentIndex", "I");
    jint ji_currentIndex = env->GetIntField(jobj_ARContentItem, fid_currentIndex);

	jfieldID fid_localPathList = env->GetFieldID(jcls_ARContentItem, "localPathList", "[Ljava/lang/String;");
	jobjectArray joarr_localPathList = reinterpret_cast<jobjectArray>(env->GetObjectField(jobj_ARContentItem, fid_localPathList));

	jint size = env->GetArrayLength(joarr_localPathList);
	std::vector< std::string > localPathList;
	for (int i = 0; i < size; ++i) {
		jstring js_localPath = (jstring)env->GetObjectArrayElement(joarr_localPathList, i);
		std::string localPath = jstring2String(env, js_localPath);
		localPathList.push_back(localPath);
		//LOGI("ARMenuItem ARContentItem localPath (%s)", localPath.c_str());
	}

    armakers::ARContentItem *pARContentItem;
    if (ji_type == armakers::ARContentItemType::VIDEO_DATA) {
        pARContentItem = new armakers::ARVideoItem();
        std::string filePath = localPathList.empty() ? "" : localPathList[0];
        std::shared_ptr<gameplay::AndroidVideoPlayer> androidVideoPlayer =
                std::static_pointer_cast<gameplay::AndroidVideoPlayer>(gameplay::VideoPlayer::create(filePath));
        androidVideoPlayer->initJavaVariable(javaVM, env, context);
        ((armakers::ARVideoItem*)pARContentItem)->setVideoPlayer(androidVideoPlayer);
    } else {
        pARContentItem = new armakers::ARContentItem();
    }
    *pItem = pARContentItem;
    pARContentItem->setType(ji_type);
    pARContentItem->setContentID(contentID);
    pARContentItem->setMenuTitle(menuTitle);
    pARContentItem->setRealFileName(realFileName);
    pARContentItem->setCurrentIndex(ji_currentIndex);
    pARContentItem->setLocalPathList(localPathList);
	//LOGI("ARMenuItem ARContentItem (%d)(%s)(%s)(%s)(%d)", pARContentItem->getType(),
	//     pARContentItem->getContentID().c_str(), pARContentItem->getMenuTitle().c_str(),
	//     pARContentItem->getRealFileName().c_str(), pARContentItem->getCurrentIndex());
}

void createARMenuItem(JNIEnv *env, jobject context, jclass jcls_ARMenuItem, jobject jobj_ARMenuItem, armakers::ARMenuItem **pItem) {
    jfieldID fid_type = env->GetFieldID(jcls_ARMenuItem, "type", "I");
    jint ji_type = env->GetIntField(jobj_ARMenuItem, fid_type);

    jfieldID fid_resID = env->GetFieldID(jcls_ARMenuItem, "resID", "I");
    jint ji_resID = env->GetIntField(jobj_ARMenuItem, fid_resID);

    jfieldID fid_form = env->GetFieldID(jcls_ARMenuItem, "form", "Ljava/lang/String;");
    jstring js_form = (jstring)env->GetObjectField(jobj_ARMenuItem, fid_form);
    std::string form = jstring2String(env, js_form);

    switch (ji_type) {
    	case armakers::ARContextMenuType::AR_CALL_MENU:
        case armakers::ARContextMenuType::FACILITY_WORK_MENU:
        case armakers::ARContextMenuType::FACILITY_MEMO_MENU:
		{
			std::shared_ptr<armakers::AndroidSystemLayout> androidSystemLayout =
					std::static_pointer_cast<armakers::AndroidSystemLayout>(armakers::SystemLayout::create(
									ARContextMenuTypeToString((armakers::ARContextMenuType)ji_type),
									-1, -1, ji_resID));
			androidSystemLayout->initJavaVariable(javaVM, env, context);
            armakers::ARSystemLayoutItem* pMenuItem = new armakers::ARSystemLayoutItem();
            *pItem = pMenuItem;
            pMenuItem->setType(ji_type);
            pMenuItem->setResID(ji_resID);
            pMenuItem->setForm(form);
            pMenuItem->setSystemLayout(androidSystemLayout);
		}
			break;
    	case armakers::ARContextMenuType::DATA_VIEWER_MENU:
		{
			armakers::ARContentList* itemList = new armakers::ARContentList();
			*pItem = itemList;
			itemList->setType(ji_type);
			itemList->setResID(ji_resID);
			itemList->setForm(form);

			jclass jcls_ARContentList = env->FindClass("com/kt/renderobject/ARContentList");
			jfieldID fid_items = env->GetFieldID(jcls_ARContentList, "items", "[Lcom/kt/renderobject/ARContentItem;");
			jobjectArray joarr_items = reinterpret_cast<jobjectArray>(env->GetObjectField(jobj_ARMenuItem, fid_items));

			jfieldID fid_currentItemIndex = env->GetFieldID(jcls_ARContentList, "currentItemIndex", "I");
			jint currentItemIndex = env->GetIntField(jobj_ARMenuItem, fid_currentItemIndex);
			itemList->setCurrentItemIndex(currentItemIndex);

			jint size = env->GetArrayLength(joarr_items);
			jclass jcls_ARContentItem = env->FindClass("com/kt/renderobject/ARContentItem");
			std::vector< std::shared_ptr<armakers::ARContentItem> > items;
			for (int i = 0; i < size; ++i) {
				jobject jobj_ARContentItem = env->GetObjectArrayElement(joarr_items, i);
				armakers::ARContentItem *pContentItem = nullptr;
				createARContentItem(env, context, jcls_ARContentItem, jobj_ARContentItem, &pContentItem);
				std::shared_ptr<armakers::ARContentItem> item(pContentItem);
				items.push_back(item);
			}
			itemList->setItems(items);
		}
			break;
        default:
        {
			armakers::ARMenuItem* pMenuItem = new armakers::ARMenuItem();
            *pItem = pMenuItem;
            pMenuItem->setType(ji_type);
            pMenuItem->setResID(ji_resID);
            pMenuItem->setForm(form);
        }
            break;

    }
}

void setItemList(JNIEnv *env, jobject context, jclass jcls_ARComplexViewer, jobject jobj_ARComplexViewer,
                 std::shared_ptr<armakers::ARComplexViewer> viewer) {
    jclass jcls_ARMenuItem = env->FindClass("com/kt/renderobject/ARMenuItem");
    jfieldID fid_items = env->GetFieldID(jcls_ARComplexViewer, "items", "[Lcom/kt/renderobject/ARMenuItem;");
    jobjectArray joarr_items = reinterpret_cast<jobjectArray>(env->GetObjectField(jobj_ARComplexViewer, fid_items));
    jint size = env->GetArrayLength(joarr_items);
    for (int i = 0; i < size; ++i) {
        jobject jobj_ARMenuItem = env->GetObjectArrayElement(joarr_items, i);
		armakers::ARMenuItem *pItem = nullptr;
        createARMenuItem(env, context, jcls_ARMenuItem, jobj_ARMenuItem, &pItem);
        std::shared_ptr<armakers::ARMenuItem> item(pItem);
        viewer->getItems().push_back(item);
        //LOGI("ARMenuItem (%d)(%d)(%s)", item->getType(), item->getResID(), item->getForm().c_str());
    }
}

std::shared_ptr<armakers::ARComplexViewer> bindARComplexViewer(JNIEnv *env, jobject context, jobject jobj) {
    jclass jcls_ARComplexViewer = env->FindClass("com/kt/renderobject/ARComplexViewer");
    jobject jobj_ARComplexViewer = jobj;
    std::shared_ptr<armakers::ARComplexViewer> viewer(new armakers::ARComplexViewer());
    setTransform(env, jcls_ARComplexViewer, jobj_ARComplexViewer, viewer);
    setInformation(env, jcls_ARComplexViewer, jobj_ARComplexViewer, viewer);
    setMenuList(env, jcls_ARComplexViewer, jobj_ARComplexViewer, viewer);
    setItemList(env, context, jcls_ARComplexViewer, jobj_ARComplexViewer, viewer);
    return viewer;
}
// bind function

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
	// Store Java VM handler:
	javaVM = vm;
	return JNI_VERSION_1_4;
}

JNIEXPORT void JNICALL Java_com_maxst_mkit_MKitJni_createGame(JNIEnv *env, jclass clazz) {
	if (mKitRenderer != nullptr) {
		delete mKitRenderer;
		mKitRenderer = nullptr;
	}

	mKitRenderer = new gameplay::MKitRendererManager();
}

JNIEXPORT void JNICALL Java_com_maxst_mkit_MKitJni_destroyGame(JNIEnv *env, jclass clazz) {
	if (mKitRenderer != nullptr) {
		mKitRenderer->shutdown();
		delete mKitRenderer;
		mKitRenderer = nullptr;
	}
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_initParam(JNIEnv *env, jobject obj, jobject activity,
									  jobject assetManager, jobject jniHelper,
									  jstring externalPath) {
	LOGI("nativeSetAsset");

	AAssetManager *_assetManager = AAssetManager_fromJava(env, assetManager);
	GP_ASSERT(_assetManager);

	char *path = jstringTostring(env, externalPath);
	GP_ASSERT(path);
	jobject _activity = env->NewGlobalRef(activity);
	initParam(javaVM, _activity, _assetManager, path);

	std::static_pointer_cast<AndroidHelper>(MKitHelper::getInstance())->initJavaVariables(javaVM, activity, jniHelper);
}

JNIEXPORT void JNICALL Java_com_maxst_mkit_MKitJni_clearARContent(JNIEnv *env, jobject obj) {
	mKitRenderer->clearContent();
}

JNIEXPORT void JNICALL Java_com_maxst_mkit_MKitJni_clearARContentWithId(JNIEnv *env, jobject obj, jstring controlId) {
	char *strControlId = jstringTostring(env, controlId);
	mKitRenderer->clearContent(strControlId);
}

JNIEXPORT void JNICALL Java_com_maxst_mkit_MKitJni_surfaceCreated(JNIEnv *env, jobject obj) {
	LOGI("nativeCreated");

	gameplay::Platform *platform = gameplay::Platform::create(mKitRenderer);
	GP_ASSERT(platform);

	initGameTime();
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_surfaceChanged(JNIEnv *env, jobject obj, jint width, jint height) {
	setViewPort(width, height);
    gameplay::Platform::resizeEventInternal(width, height);
	LOGI("nativeChanged. Viewport width : %d, height : %d", width, height);
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_renderFrame(JNIEnv *env, jobject obj) {
	if (gameplay::Game::getInstance()->getState() == gameplay::Game::UNINITIALIZED) {
		gameplay::Game::getInstance()->run();
		if (mKitRenderer) mKitRenderer->onStarted();
	}

	if (!__suspended) {
		gameplay::Game::getInstance()->frame();
	}
}

JNIEXPORT jboolean JNICALL
Java_com_maxst_mkit_MKitJni_isInitialize(JNIEnv *env, jobject obj) {
    return static_cast<jboolean>(
            gameplay::Game::getInstance()->getState() != gameplay::Game::UNINITIALIZED);
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_changeRendererSize(JNIEnv *env, jobject obj, jint lw, jint lh, jint rw, jint rh) {
	if (mKitRenderer) mKitRenderer->changeRendererSize(lw, lh, rw, rh);
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_addCustomViewLayout(JNIEnv *env, jobject obj, jobject context,
													 jstring controlId, jint layoutId,
													 jintArray jsize, jfloatArray jpos,
													 jfloatArray jrotation, jfloatArray jscale, jfloat jalpha) {
	auto strControlId = jstring2String(env, controlId);
	LOGD("addCustomViewLayout name : %s", strControlId.c_str());
	LOGD("addCustomViewLayout layoutId : %d", layoutId);

	int size[2] = {0, };
	float pos[3] = { 0.f, };
	float rotation[3] = { 0.f, };
	float scale[3] = { 0.f, };

    env->GetIntArrayRegion(jsize, 0, 2, (jint *)size);
	env->GetFloatArrayRegion(jpos, 0, 3, (jfloat *)pos);
	env->GetFloatArrayRegion(jrotation, 0, 3, (jfloat *)rotation);
	env->GetFloatArrayRegion(jscale, 0, 3, (jfloat *)scale);

    LOGD("addCustomViewLayout pos : %f/%f/%f", pos[0], pos[1], pos[2]);

	std::shared_ptr<armakers::AndroidSystemLayout> androidSystemLayout =
			std::static_pointer_cast<armakers::AndroidSystemLayout>(
					armakers::SystemLayout::create(strControlId, 100, 100, layoutId));
	androidSystemLayout->initJavaVariable(javaVM, env, context);

	mKitRenderer->addCustomViewLayout(androidSystemLayout, strControlId, size,
		gameplay::Vector3(pos), gameplay::Vector3(rotation), gameplay::Vector3(scale), jalpha);
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_removeCustomViewLayout(JNIEnv *env, jobject obj, jstring controlId) {
	auto strControlId = jstring2String(env, controlId);
    mKitRenderer->removeCustomViewLayout(strControlId);
}

JNIEXPORT void JNICALL Java_com_maxst_mkit_MKitJni_pause(JNIEnv *env, jobject obj) {
	LOGI("nativePause");
	LOGD("Game state = %d", gameplay::Game::getInstance()->getState());

	__suspended = true;

	if (__initialized) {
		LOGD("game pause");
	}
}

JNIEXPORT void JNICALL Java_com_maxst_mkit_MKitJni_resume(JNIEnv *env, jobject obj) {
	LOGI("nativeResume");
	LOGD("Game state = %d", gameplay::Game::getInstance()->getState());

	__suspended = false;

	if (__initialized) {
		LOGD("game resume");
		gameplay::Game::getInstance()->resume();
	}
}

JNIEXPORT void JNICALL Java_com_maxst_mkit_MKitJni_destroy(JNIEnv *env, jobject obj) {
	LOGI("nativeDestroy");
	LOGD("Game state = %d", gameplay::Game::getInstance()->getState());

	__suspended = true;
	if (__initialized) {
		LOGD("game shutdown");
		gameplay::Game::getInstance()->shutdown();
		__initialized = false;
	}
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_touchDown(JNIEnv *env, jobject obj, jint action, jfloat x, jfloat y,
									  jint pointerId) {
	gameplay::Touch::TouchEvent event = gameplay::Touch::TOUCH_PRESS;
	gameplay::Platform::touchEventInternal(event, x, y, pointerId);
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_touchUp(JNIEnv *env, jobject obj, jint action, jfloat x, jfloat y,
									jint pointerId) {
	gameplay::Touch::TouchEvent event = gameplay::Touch::TOUCH_RELEASE;
	gameplay::Platform::touchEventInternal(event, x, y, pointerId);
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_touchMove(JNIEnv *env, jobject obj, jint action, jfloat x, jfloat y,
									  jint pointerId) {
	gameplay::Touch::TouchEvent event = gameplay::Touch::TOUCH_MOVE;
	gameplay::Platform::touchEventInternal(event, x, y, pointerId);
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_setProjectionMatrix(JNIEnv *env, jobject obj, jfloatArray poseMatrix) {
	jfloat * nativeFloats = env->GetFloatArrayElements(poseMatrix, 0);
	mKitRenderer->setProjectionMatrix((float *)nativeFloats);
	env->ReleaseFloatArrayElements(poseMatrix, nativeFloats, JNI_ABORT);
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_setTrackingResult(JNIEnv *env, jobject obj, jboolean isSuccess,
											  jstring id, jstring name, jfloatArray poseMatrix) {
	if (isSuccess) {
		char * strId = jstringTostring(env, id);
		char * strName = jstringTostring(env, name);
		jfloat * nativeFloats = env->GetFloatArrayElements(poseMatrix, 0);
		mKitRenderer->setTrackingResult(isSuccess, strId, strName, (float *)nativeFloats);
		env->ReleaseFloatArrayElements(poseMatrix, nativeFloats, JNI_ABORT);
	} else {
		mKitRenderer->setTrackingResult(isSuccess);
	}
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_setCameraQuaternion(JNIEnv *env, jobject obj, jfloatArray jQuaternion) {
	float quaternion[4] = {0.f,};
	env->GetFloatArrayRegion(jQuaternion, 0, 4, (jfloat *) quaternion);
	if (mKitRenderer) {
		mKitRenderer->setCameraQuaternion(quaternion);
	}
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_addLayout1(JNIEnv *env, jobject obj,
									   jstring layoutName,
									   jstring textImagePath,
									   jstring checkControlId,
									   jstring buttonControlId,
									   jfloat positionX, jfloat positionY, jfloat positionZ,
									   jfloat scaleX, jfloat scaleY, jfloat scaleZ,
									   jfloat width, jfloat height) {
	char *strLayoutName = jstringTostring(env, layoutName);
	char *strTextImagePath = jstringTostring(env, textImagePath);
	char *strCheckControlId = jstringTostring(env, checkControlId);
	char *strButtonControlId = jstringTostring(env, buttonControlId);

	mKitRenderer->addLayout1(strLayoutName, strTextImagePath, strCheckControlId, strButtonControlId,
							 gameplay::Vector3(positionX, positionY, positionZ),
							 gameplay::Vector3(scaleX, scaleY, scaleZ),
							 width, height);
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_addChildLayout(JNIEnv *env, jobject obj,
									   jstring layoutName,
									   jstring textImagePath,
									   jstring checkControlId,
									   jstring buttonControlId,
									   jfloat positionX, jfloat positionY, jfloat positionZ,
									   jfloat scaleX, jfloat scaleY, jfloat scaleZ,
									   jfloat width, jfloat height, jboolean isChild) {
	char *strLayoutName = jstringTostring(env, layoutName);
	char *strTextImagePath = jstringTostring(env, textImagePath);
	char *strCheckControlId = jstringTostring(env, checkControlId);
	char *strButtonControlId = jstringTostring(env, buttonControlId);

	mKitRenderer->addChildLayout(strLayoutName, strTextImagePath, strCheckControlId, strButtonControlId,
							 gameplay::Vector3(positionX, positionY, positionZ),
							 gameplay::Vector3(scaleX, scaleY, scaleZ),
							 width, height, (bool)isChild);
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_addCheckLayout(JNIEnv *env, jclass type, jstring layoutName_,
										   jstring textImagePath_, jstring buttonControlId_,
										   jfloat positionX, jfloat positionY, jfloat positionZ,
										   jfloat scaleX, jfloat scaleY, jfloat scaleZ) {
	const char *layoutName = env->GetStringUTFChars(layoutName_, 0);
	const char *textImagePath = env->GetStringUTFChars(textImagePath_, 0);
	const char *buttonControlId = env->GetStringUTFChars(buttonControlId_, 0);


}

JNIEXPORT void JNICALL Java_com_maxst_mkit_MKitJni_addLayout2(JNIEnv *env, jobject obj,
															  jstring layoutName,
															  jstring textImagePath,
															  jstring checkControlId,
															  jstring buttonControlId,
															  jfloat positionX, jfloat positionY,
															  jfloat positionZ,
															  jfloat scaleX, jfloat scaleY,
															  jfloat scaleZ) {
	char *strLayoutName = jstringTostring(env, layoutName);
	char *strTextImagePath = jstringTostring(env, textImagePath);
	char *strCheckControlId = jstringTostring(env, checkControlId);
	char *strButtonControlId = jstringTostring(env, buttonControlId);

	mKitRenderer->addLayout2(strLayoutName, strTextImagePath, strCheckControlId, strButtonControlId,
							 gameplay::Vector3(positionX, positionY, positionZ),
							 gameplay::Vector3(scaleX, scaleY, scaleZ));
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_addLayout3(JNIEnv *env, jobject obj,
									   jstring layoutName,
									   jstring checkControlId,
									   jstring buttonControlId,
									   jfloat positionX, jfloat positionY, jfloat positionZ,
									   jfloat scaleX, jfloat scaleY, jfloat scaleZ) {
	char *strLayoutName = jstringTostring(env, layoutName);
	char *strCheckControlId = jstringTostring(env, checkControlId);
	char *strButtonControlId = jstringTostring(env, buttonControlId);

	mKitRenderer->addLayout3(strLayoutName, strCheckControlId, strButtonControlId,
							 gameplay::Vector3(positionX, positionY, positionZ),
							 gameplay::Vector3(scaleX, scaleY, scaleZ));
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_addIconLayout(JNIEnv *env, jobject obj,
									   jstring layoutName,
									   jstring iconPath,
									   jfloat positionX, jfloat positionY, jfloat positionZ,
									   jfloat scaleX, jfloat scaleY, jfloat scaleZ,
									   jfloat width, jfloat height) {
	char *strLayoutName = jstringTostring(env, layoutName);
	char *strIconPath = jstringTostring(env, iconPath);

	mKitRenderer->addIconLayout(strLayoutName, strIconPath,
							 gameplay::Vector3(positionX, positionY, positionZ),
							 gameplay::Vector3(scaleX, scaleY, scaleZ),
							 width, height);
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_addTextLayout(JNIEnv *env, jobject obj,
										  jstring layoutName,
										  jstring textImagePath,
										  jstring controlId,
										  jfloat positionX, jfloat positionY, jfloat positionZ,
										  jfloat scaleX, jfloat scaleY, jfloat scaleZ,
										  jfloat width, jfloat height) {
	char *strLayoutName = jstringTostring(env, layoutName);
	char *strIconPath = jstringTostring(env, textImagePath);
	char *strControlId = jstringTostring(env, controlId);

	mKitRenderer->addTextLayout(strLayoutName, strIconPath, strControlId,
								gameplay::Vector3(positionX, positionY, positionZ),
								gameplay::Vector3(scaleX, scaleY, scaleZ),
								width, height);
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_addTagLayout(JNIEnv *env, jobject obj,
										 jstring layoutName,
										 jstring textImagePath,
										 jstring buttonControlId,
										 jfloat positionX, jfloat positionY, jfloat positionZ,
										 jfloat scaleX, jfloat scaleY, jfloat scaleZ,
										 jfloat width, jfloat height) {
	char *strLayoutName = jstringTostring(env, layoutName);
	char *strTextImagePath = jstringTostring(env, textImagePath);
	char *strButtonControlId = jstringTostring(env, buttonControlId);

	mKitRenderer->addTagLayout(strLayoutName, strTextImagePath, strButtonControlId,
							 gameplay::Vector3(positionX, positionY, positionZ),
							 gameplay::Vector3(scaleX, scaleY, scaleZ),
							 width, height);
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_addImage(JNIEnv *env, jobject obj, jstring imageName, jstring filePath,
									 jfloat posX, jfloat posY, jfloat posZ,
									 jfloat rotationX, jfloat rotationY, jfloat rotationZ,
									 jfloat scaleX, jfloat scaleY, jfloat scaleZ,
									 jboolean modulateAlpha) {
	const char *strImageName = jstringTostring(env, imageName);
	const char *strFilePath = jstringTostring(env, filePath);

	LOGD("strImageName : %s, filePath:%s", strImageName, strFilePath);

	if (mKitRenderer == nullptr) {
		LOGD("mKitRenderer is null !!");
	}

	mKitRenderer->addImage(strImageName,
						   strFilePath,
						   gameplay::Vector3(posX, posY, posZ),
                           gameplay::Vector3(rotationX, rotationY, rotationZ),
                           gameplay::Vector3(scaleX, scaleY, scaleZ),
                           modulateAlpha);
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_addImageAndAlpha(JNIEnv *env, jobject obj, jstring controlId, jstring filePath,
											jfloat posX, jfloat posY, jfloat posZ,
									 		jfloat rotationX, jfloat rotationY, jfloat rotationZ,
									 		jfloat scaleX, jfloat scaleY, jfloat scaleZ,
									 		jfloat alpha) {
	const char *strControlId = jstringTostring(env, controlId);
	const char *strFilePath = jstringTostring(env, filePath);

//	LOGD("addImageAndAlpha strImageName : %s, filePath : %s", strImageName, strFilePath);

	if (mKitRenderer == nullptr) {
		LOGD("addImageAndAlpha mKitRenderer is null !!");
	}

	mKitRenderer->addImageAndAlpha(strControlId,
			                       strFilePath,
                                   gameplay::Vector3(posX, posY, posZ),
                                   gameplay::Vector3(rotationX, rotationY, rotationZ),
                                   gameplay::Vector3(scaleX, scaleY, scaleZ),
								   alpha);
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_addImageLayout(JNIEnv *env, jobject obj, jstring layoutName,
										   jstring filePath, jfloat imageWidth, jfloat imageHeight,
										   jstring controlId, jfloat posX, jfloat posY, jfloat posZ,
										   jfloat scaleX, jfloat scaleY, jfloat scaleZ) {
	char *strLayoutName = jstringTostring(env, layoutName);
	char *strFilePath = jstringTostring(env, filePath);
	char *strControlId = jstringTostring(env, controlId);

	LOGD("layoutName : %s, filePath:%s, controlID:%s", strLayoutName, strFilePath, strControlId);

	if (mKitRenderer == nullptr) {
		LOGD("mKitRenderer is null !!");
	}

	mKitRenderer->addImageLayout(strLayoutName,
								 strFilePath, imageWidth, imageHeight,
								 strControlId,
                                 gameplay::Vector3(posX, posY, posZ),
                                 gameplay::Vector3(scaleX, scaleY, scaleZ));
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_addImagePagesLayoutAndAlpha(JNIEnv *env, jobject obj, jstring layoutName,
                                                        jstring controlId, jobjectArray lst, jfloat imageWidth, jfloat imageHeight,
                                                        jfloat posX, jfloat posY, jfloat posZ,
                                                        jfloat rotX, jfloat rotY, jfloat rotZ,
                                                        jfloat scaleX, jfloat scaleY, jfloat scaleZ,
                                                        jfloat alpha) {
    char *strLayoutName = jstringTostring(env, layoutName);
    char *strControlId = jstringTostring(env, controlId);

    LOGD("layoutName : %s, controlID:%s", strLayoutName, strControlId);

    if (mKitRenderer == nullptr) {
        LOGD("mKitRenderer is null !!");
    }
	jint size = env->GetArrayLength(lst);
	std::vector< std::string > localPathList;
	for (int i = 0; i < size; ++i) {
		jstring js_localPath = (jstring)env->GetObjectArrayElement(lst, i);
		std::string localPath = jstring2String(env, js_localPath);
		localPathList.push_back(localPath);
	}

    mKitRenderer->addImagePagesLayoutAndAlpha(strLayoutName, strControlId,
											  localPathList, imageWidth, imageHeight,
                                         gameplay::Vector3(posX, posY, posZ),
                                         gameplay::Vector3(rotX, rotY, rotZ),
                                         gameplay::Vector3(scaleX, scaleY, scaleZ),
                                         alpha);
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_changeImagePage(JNIEnv *env, jobject obj, jstring controlId, jint opt) {

	char *strControlId = jstringTostring(env, controlId);

	if (mKitRenderer == nullptr) {
		LOGD("mKitRenderer is null !!");
	}
	mKitRenderer->changeImagePage(strControlId, opt);
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_addImageLayoutAndAlpha(JNIEnv *env, jobject obj, jstring layoutName,
												   jstring controlId, jstring filePath, jfloat imageWidth, jfloat imageHeight,
												   jfloat posX, jfloat posY, jfloat posZ,
												   jfloat rotX, jfloat rotY, jfloat rotZ,
												   jfloat scaleX, jfloat scaleY, jfloat scaleZ,
												   jfloat alpha) {
	char *strLayoutName = jstringTostring(env, layoutName);
	char *strFilePath = jstringTostring(env, filePath);
	char *strControlId = jstringTostring(env, controlId);

	LOGD("layoutName : %s, filePath:%s, controlID:%s", strLayoutName, strFilePath, strControlId);

	if (mKitRenderer == nullptr) {
		LOGD("mKitRenderer is null !!");
	}

	mKitRenderer->addImageLayoutAndAlpha(strLayoutName, strControlId,
								 strFilePath, imageWidth, imageHeight,
                                 gameplay::Vector3(posX, posY, posZ),
                                 gameplay::Vector3(rotX, rotY, rotZ),
                                 gameplay::Vector3(scaleX, scaleY, scaleZ),
								 alpha);
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_addImageLayoutChange(JNIEnv *env, jobject obj, jstring nodeId, jstring layoutName,
										   jstring filePath, jfloat imageWidth, jfloat imageHeight,
										   jstring controlId, jfloat posX, jfloat posY, jfloat posZ,
										   jfloat scaleX, jfloat scaleY, jfloat scaleZ) {
	char *strNodeId = jstringTostring(env, nodeId);
	char *strLayoutName = jstringTostring(env, layoutName);
	char *strFilePath = jstringTostring(env, filePath);
	char *strControlId = jstringTostring(env, controlId);

	LOGD("addImageLayoutCheck strNodeId : %s layoutName : %s, filePath:%s, controlID:%s", strNodeId,
		 strLayoutName, strFilePath, strControlId);

	if (mKitRenderer == nullptr) {
		LOGD("mKitRenderer is null !!");
	}

	mKitRenderer->addImageLayoutChange(strNodeId, strLayoutName,
									  strFilePath, imageWidth, imageHeight,
									  strControlId,
                                      gameplay::Vector3(posX, posY, posZ),
                                      gameplay::Vector3(scaleX, scaleY, scaleZ));

}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_changeImageLayout(JNIEnv *env, jobject obj, jstring nodeId, jstring layoutName, jstring filePath) {

	char *strNodeId = jstringTostring(env, nodeId);
	char *strLayoutName = jstringTostring(env, layoutName);
	char *strFilePath = jstringTostring(env, filePath);

	LOGD("MKitJni_changeImageLayout strNodeId : %s, layoutName : %s, filePath:%s", strNodeId, strLayoutName, strFilePath);

	if (mKitRenderer == nullptr) {
		LOGD("mKitRenderer is null !!");
	}

	mKitRenderer->changeImageLayout(strNodeId, strLayoutName, strFilePath);

}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_addImageLayout1(JNIEnv *env, jobject obj, jstring layoutName,
											jstring filePath, jfloat imageWidth, jfloat imageHeight,
											jstring controlId, jfloat posX, jfloat posY, jfloat posZ,
											jfloat rotationX, jfloat rotationY, jfloat rotationZ,
											jfloat scaleX, jfloat scaleY, jfloat scaleZ) {
	char *strLayoutName = jstringTostring(env, layoutName);
	char *strFilePath = jstringTostring(env, filePath);
	char *strControlId = jstringTostring(env, controlId);

	LOGD("layoutName : %s, filePath:%s, controlID:%s", strLayoutName, strFilePath, strControlId);

	if (mKitRenderer == nullptr) {
		LOGD("mKitRenderer is null !!");
	}

	mKitRenderer->addImageLayout1(strLayoutName,
								  strFilePath, imageWidth, imageHeight,
								  strControlId,
                                  gameplay::Vector3(posX, posY, posZ),
                                  gameplay::Vector3(rotationX, rotationY, rotationZ),
                                  gameplay::Vector3(scaleX, scaleY, scaleZ));
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_addVideo(JNIEnv *env, jobject obj, jobject context,
									 jstring filePath,
									 jfloat posX, jfloat posY, jfloat posZ,
									 jfloat rotationX, jfloat rotationY, jfloat rotationZ,
									 jfloat scaleX, jfloat scaleY, jfloat scaleZ) {
	char *strFilePath = jstringTostring(env, filePath);
	std::shared_ptr<gameplay::AndroidVideoPlayer> androidVideoPlayer =
			std::static_pointer_cast<gameplay::AndroidVideoPlayer>(gameplay::VideoPlayer::create(strFilePath));
	androidVideoPlayer->initJavaVariable(javaVM, env, context);

	mKitRenderer->addVideo(androidVideoPlayer,
                           gameplay::Vector3(posX, posY, posZ),
                           gameplay::Vector3(rotationX, rotationY, rotationZ),
                           gameplay::Vector3(scaleX, scaleY, scaleZ));
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_addVideoAndAlpha(JNIEnv *env, jobject obj, jobject context,
                                            jstring controlId, jstring filePath,
                                            jfloat posX, jfloat posY, jfloat posZ,
                                            jfloat rotationX, jfloat rotationY, jfloat rotationZ,
                                            jfloat scaleX, jfloat scaleY, jfloat scaleZ,
                                            jfloat alpha) {
    char *strControlId = jstringTostring(env, controlId);
    char *strFilePath = jstringTostring(env, filePath);
    std::shared_ptr<gameplay::AndroidVideoPlayer> androidVideoPlayer =
            std::static_pointer_cast<gameplay::AndroidVideoPlayer>(gameplay::VideoPlayer::create(strFilePath));
    androidVideoPlayer->initJavaVariable(javaVM, env, context);

    mKitRenderer->addVideoAndAlpha(androidVideoPlayer,
                                   strControlId,
                                   gameplay::Vector3(posX, posY, posZ),
                                   gameplay::Vector3(rotationX, rotationY, rotationZ),
                                   gameplay::Vector3(scaleX, scaleY, scaleZ),
                                   alpha);
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_addVideoLayout(JNIEnv *env, jobject obj, jobject context,
									 jstring controlId,
									 jstring filePath,
									 jfloat posX, jfloat posY, jfloat posZ,
									 jfloat rotationX, jfloat rotationY, jfloat rotationZ,
									 jfloat scaleX, jfloat scaleY, jfloat scaleZ) {
	char *strControlId = jstringTostring(env, controlId);
	char *strFilePath = jstringTostring(env, filePath);

	LOGD("Video controlId : %s", strControlId);
	LOGD("Video filePath : %s", strFilePath);
	std::shared_ptr<gameplay::AndroidVideoPlayer> androidVideoPlayer =
			std::static_pointer_cast<gameplay::AndroidVideoPlayer>(gameplay::VideoPlayer::create(strFilePath));
	androidVideoPlayer->initJavaVariable(javaVM, env, context);

	mKitRenderer->addVideoLayout(strControlId, androidVideoPlayer,
                                 gameplay::Vector3(posX, posY, posZ),
                                 gameplay::Vector3(rotationX, rotationY, rotationZ),
                                 gameplay::Vector3(scaleX, scaleY, scaleZ));
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_addWebViewAndAlpha(JNIEnv *env, jobject obj, jobject context,
											   jstring name, jstring url,
											   jfloat posX, jfloat posY, jfloat posZ,
											   jfloat rotationX, jfloat rotationY, jfloat rotationZ,
											   jfloat scaleX, jfloat scaleY, jfloat scaleZ,
											   jfloat alpha) {
	char *strUrl = jstringTostring(env, url);
	char *strName = jstringTostring(env, name);
	std::shared_ptr<armakers::AndroidWebView> androidWebView =
			std::static_pointer_cast<armakers::AndroidWebView>(
                    armakers::WebView::create(strName, strUrl, (int) (scaleX * 100), (int) (scaleY * 100)));
	androidWebView->initJavaVariable(javaVM, env, context);

	mKitRenderer->addWebViewAndAlpha(androidWebView,
                                     gameplay::Vector3(posX, posY, posZ),
                                     gameplay::Vector3(rotationX, rotationY, rotationZ),
                                     gameplay::Vector3(scaleX, scaleY, scaleZ),
									 alpha);
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_addGlobalMenuLayout(JNIEnv *env, jobject obj, jobject context,
										   		jstring controlId, jfloatArray jpos,
										   		jfloatArray jrotation, jfloatArray jscale, jfloat jalpha) {
	char *strControlId = jstringTostring(env, controlId);
	LOGD("GlobalMenu controlId : %s", strControlId);
	float pos[3] = { 0.f, };
    float rotation[3] = { 0.f, };
    float scale[3] = { 0.f, };

	env->GetFloatArrayRegion(jpos, 0, 3, (jfloat *)pos);
	env->GetFloatArrayRegion(jrotation, 0, 3, (jfloat *)rotation);
	env->GetFloatArrayRegion(jscale, 0, 3, (jfloat *)scale);

	mKitRenderer->addGlobalMenuLayout(strControlId, gameplay::Vector3(pos), gameplay::Vector3(rotation),
                                      gameplay::Vector3(scale), jalpha);
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_showGlobalMenuLayout(JNIEnv *env, jobject obj, jobject context,
												 jstring controlId, jfloatArray jpos,
												 jfloatArray jrotation, jfloatArray jscale, jfloat jalpha) {
	char *strControlId = jstringTostring(env, controlId);
	LOGD("GlobalMenu controlId : %s", strControlId);
	float pos[3] = { 0.f, };
	float rotation[3] = { 0.f, };
	float scale[3] = { 0.f, };

	env->GetFloatArrayRegion(jpos, 0, 3, (jfloat *)pos);
	env->GetFloatArrayRegion(jrotation, 0, 3, (jfloat *)rotation);
	env->GetFloatArrayRegion(jscale, 0, 3, (jfloat *)scale);

	mKitRenderer->showGlobalMenuLayout(strControlId, gameplay::Vector3(pos), gameplay::Vector3(rotation),
                                       gameplay::Vector3(scale), jalpha);
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_hideGlobalMenuLayout(JNIEnv *env, jobject obj) {
	mKitRenderer->hideGlobalMenuLayout();
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_addSystemLayoutAndAlpha(JNIEnv *env, jobject obj, jobject context,
													jstring name, jint layoutId, jfloatArray jpos,
													jfloatArray jrotation, jfloatArray jscale, jfloat jalpha) {
	char *strName = jstringTostring(env, name);
	LOGD("addSystemLayoutAndAlpha name : %s", strName);
	LOGD("addSystemLayoutAndAlpha layoutId : %d", layoutId);

	float pos[3] = { 0.f, };
	float rotation[3] = { 0.f, };
	float scale[3] = { 0.f, };

	env->GetFloatArrayRegion(jpos, 0, 3, (jfloat *)pos);
	env->GetFloatArrayRegion(jrotation, 0, 3, (jfloat *)rotation);
	env->GetFloatArrayRegion(jscale, 0, 3, (jfloat *)scale);

	std::shared_ptr<armakers::AndroidSystemLayout> androidSystemLayout =
			std::static_pointer_cast<armakers::AndroidSystemLayout>(
                    armakers::SystemLayout::create(strName, (int) (scale[0] * 100), (int) (scale[1] * 100), layoutId));
	androidSystemLayout->initJavaVariable(javaVM, env, context);

	mKitRenderer->addSystemLayoutAndAlpha(androidSystemLayout, gameplay::Vector3(pos),
	        gameplay::Vector3(rotation), gameplay::Vector3(scale), jalpha);
}

JNIEXPORT jboolean JNICALL
Java_com_maxst_mkit_MKitJni_hasSystemLayout(JNIEnv *env, jobject obj, jobject context,
                                                    jstring name) {
    char *strName = jstringTostring(env, name);
    LOGD("Java_com_maxst_mkit_MKitJni_hasSystemLayout name : %s", strName);
    return mKitRenderer->hasSystemLayout(strName);
}

//template
JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_addARComplexViewer(JNIEnv *env, jobject jobj, jobject context,
                                               jstring name, jstring controlId, jobject jARComplexViewer) {
	LOGD("Java_com_maxst_mkit_MKitJni_addARComplexViewer");
    std::string strName = jstring2String(env, name);
    std::string strControlId = jstring2String(env, controlId);
	std::shared_ptr<armakers::ARComplexViewer> viewer = bindARComplexViewer(env, context, jARComplexViewer);
    //LOGD("ARComplexViewer getAlpha(%f)", viewer->getAlpha());
    //LOGD("ARComplexViewer getCurrentMenuType(%d)", viewer->getCurrentMenuType());
    //LOGD("ARComplexViewer getMenuLayoutEnable(%d)", viewer->getMenuLayoutEnable());

    mKitRenderer->addARComplexViewer(strName, strControlId, viewer);
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_changeContextMenu(JNIEnv *env, jobject jobj, jstring name, jint jmenuType) {
	LOGD("Java_com_maxst_mkit_MKitJni_changeContextMenu");
	std::string strName = jstring2String(env, name);
	mKitRenderer->changeContextMenu(strName, jmenuType);
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_setContextMenuBadge(JNIEnv *env, jobject jobj, jstring name, jint jcount) {
    LOGD("Java_com_maxst_mkit_MKitJni_setContextMenuBadge");
    std::string strName = jstring2String(env, name);
    mKitRenderer->setContextMenuBadge(strName, jcount);
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_changeItemIndex(JNIEnv *env, jobject jobj, jstring name, jint jItemIndex) {
	LOGD("Java_com_maxst_mkit_MKitJni_changeItemIndex");
	std::string strName = jstring2String(env, name);
	mKitRenderer->changeItemIndex(strName, jItemIndex);
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_changePage(JNIEnv *env, jobject jobj, jstring name, jint jOp, jint jPageNo) {
	LOGD("Java_com_maxst_mkit_MKitJni_changePage");
	std::string strName = jstring2String(env, name);
	mKitRenderer->changePage(strName, jOp, jPageNo);
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_changeMediaPlayer(JNIEnv *env, jobject jobj, jstring name, jint jOp) {
	LOGD("Java_com_maxst_mkit_MKitJni_changeMediaPlayer");
	std::string strName = jstring2String(env, name);
	mKitRenderer->changeMediaPlayer(strName, jOp);
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_setEnableContextMenu(JNIEnv *env, jobject jobj, jstring name, jboolean jEnable) {
	LOGD("Java_com_maxst_mkit_MKitJni_changeMediaPlayer");
	std::string strName = jstring2String(env, name);
	mKitRenderer->setEnableContextMenu(strName, jEnable);
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_show3DToast(JNIEnv *env, jobject jobj, jstring jmsg, jint ms, jint type) {
	LOGD("Java_com_maxst_mkit_MKitJni_show3DToast");
	std::string msg = jstring2String(env, jmsg);
	mKitRenderer->show3DToast(msg, ms, type);
}

JNIEXPORT jint JNICALL
Java_com_maxst_mkit_MKitJni_getCurrentComplexMenu(JNIEnv *env, jobject jobj, jstring name) {
	LOGD("Java_com_maxst_mkit_MKitJni_getCurrentComplexMenu");
	std::string strName = jstring2String(env, name);

	return mKitRenderer->getCurrentComplexMenu(strName);
}

JNIEXPORT jint JNICALL
Java_com_maxst_mkit_MKitJni_getComplexCurrent(JNIEnv *env, jobject jobj, jstring name) {
	LOGD("Java_com_maxst_mkit_MKitJni_getComplexCurrent");
	std::string strName = jstring2String(env, name);

	return mKitRenderer->getComplexContentPage(strName);
}
//template

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_removeSystemLayout(JNIEnv *env, jobject obj, jobject context,
                                            jstring name) {
    char *strName = jstringTostring(env, name);
    LOGD("Java_com_maxst_mkit_MKitJni_removeSystemLayout name : %s", strName);
    mKitRenderer->removeSystemLayout(strName);
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_showSystemLayoutAndAlpha(JNIEnv *env, jobject obj, jobject context,
													jstring controlId, jint layoutId, jfloatArray jpos,
													jfloatArray jrotation, jfloatArray jscale, jfloat jalpha) {
	char *strControlId = jstringTostring(env, controlId);
	LOGD("addSystemLayoutAndAlpha name : %s", strControlId);
	LOGD("addSystemLayoutAndAlpha layoutId : %d", layoutId);

	float pos[3] = { 0.f, };
	float rotation[3] = { 0.f, };
	float scale[3] = { 0.f, };

	env->GetFloatArrayRegion(jpos, 0, 3, (jfloat *)pos);
	env->GetFloatArrayRegion(jrotation, 0, 3, (jfloat *)rotation);
	env->GetFloatArrayRegion(jscale, 0, 3, (jfloat *)scale);

	std::shared_ptr<armakers::AndroidSystemLayout> androidSystemLayout =
			std::static_pointer_cast<armakers::AndroidSystemLayout>(
                    armakers::SystemLayout::create(strControlId, 100, 100, layoutId));
	androidSystemLayout->initJavaVariable(javaVM, env, context);

	mKitRenderer->showSystemLayoutAndAlpha(androidSystemLayout,
										  gameplay::Vector3(pos), gameplay::Vector3(rotation), gameplay::Vector3(scale), jalpha);
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_addNode(JNIEnv *env, jobject obj, jstring str1) {
	const char *str = jstringTostring(env, str1);


	mKitRenderer->addNode(const_cast<char *>(str));
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_createNode(JNIEnv *env, jobject obj, jstring nodeName_, jboolean isChecked) {
	char *nodeName = jstringTostring(env, nodeName_);

	mKitRenderer->createNode(nodeName, (bool)isChecked);
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_addNodeMap(JNIEnv *env, jobject obj, jstring nodeName_, jboolean isChecked) {
	char *nodeName = jstringTostring(env, nodeName_);

	mKitRenderer->addNodeMap(nodeName, (bool)isChecked);
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_applyCheckState(JNIEnv *env, jobject obj,
									   jstring checkControlId,
									   jboolean isChecked) {
	char *strCheckControlId = jstringTostring(env, checkControlId);
	mKitRenderer->applyCheckState(strCheckControlId, (bool)isChecked);
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_setGlobalMenuBadgeText(JNIEnv *env, jobject obj,
											jstring controlId,
											jstring text) {
	char *strControlId = jstringTostring(env, controlId);
	char *strText = jstringTostring(env, text);

	mKitRenderer->setGlobalMenuBadgeText(strControlId, strText);
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_setGlobalMenuBadgeVisible(JNIEnv *env, jobject obj,
												   jstring controlId,
												   jboolean visible) {
	char *strControlId = jstringTostring(env, controlId);

	mKitRenderer->setGlobalMenuBadgeVisible(strControlId, (bool) visible);
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_addMainControlNode(JNIEnv *env, jobject obj, jobject context,
													jstring name, jint layoutId, jfloatArray jpos,
													jfloatArray jrotation, jfloatArray jscale, jfloat jalpha) {
	char *strName = jstringTostring(env, name);

	float pos[3] = { 0.f, };
	float rotation[3] = { 0.f, };
	float scale[3] = { 0.f, };

	env->GetFloatArrayRegion(jpos, 0, 3, (jfloat *)pos);
	env->GetFloatArrayRegion(jrotation, 0, 3, (jfloat *)rotation);
	env->GetFloatArrayRegion(jscale, 0, 3, (jfloat *)scale);

	std::shared_ptr<armakers::AndroidSystemLayout> androidSystemLayout =
			std::static_pointer_cast<armakers::AndroidSystemLayout>(
					armakers::SystemLayout::create(strName, 100, 100, layoutId));
	androidSystemLayout->initJavaVariable(javaVM, env, context);

	mKitRenderer->addMainControlMenu(androidSystemLayout, gameplay::Vector3(pos),
									 gameplay::Vector3(rotation), gameplay::Vector3(scale), jalpha);
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_removeMainControlNode(JNIEnv *env, jobject obj) {
	mKitRenderer->removeMainControlMenu();
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_addFixedSystemNode(JNIEnv *env, jobject obj, jobject context,
											   jstring name, jstring systemFormName, jint layoutId, jfloatArray jpos, jfloat jalpha) {
	char *strName = jstringTostring(env, name);
	char *strFormName = jstringTostring(env, systemFormName);

	float pos[3] = { 0.f, };

	env->GetFloatArrayRegion(jpos, 0, 3, (jfloat *)pos);

	std::shared_ptr<armakers::AndroidSystemLayout> androidSystemLayout =
			std::static_pointer_cast<armakers::AndroidSystemLayout>(
					armakers::SystemLayout::create(strName, 100, 100, layoutId));
	androidSystemLayout->initJavaVariable(javaVM, env, context);

	mKitRenderer->addFixedSystemLayout(androidSystemLayout, strFormName, gameplay::Vector3(pos), jalpha);
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_addFixedSharedImage(JNIEnv *env, jobject obj, jstring layoutName, jstring controlId,
			jstring imagePath, jint imageWidth, jint imageHeight, jboolean jisSmall, jfloat jalpha) {
	char *strName = jstringTostring(env, layoutName);
	char *strControlId = jstringTostring(env, controlId);
	char *strImagePath = jstringTostring(env, imagePath);

	mKitRenderer->addFixedSharedImage(strName, strControlId, strImagePath, imageWidth, imageHeight, jisSmall, jalpha);
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_removeFixedNode(JNIEnv *env, jobject obj, jstring name) {
	char *strName = jstringTostring(env, name);
	mKitRenderer->removeFixedNode(strName);
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_targetOnEnabled(JNIEnv *env, jobject obj, jboolean enable) {
    mKitRenderer->targetOnEnabled(enable);
}

JNIEXPORT void JNICALL
Java_com_maxst_mkit_MKitJni_captureScene(JNIEnv *env, jclass type, jboolean toggleCapture) {
	mKitRenderer->captureScene(toggleCapture);
}

}//extern "C"

#endif // __ANDROID__