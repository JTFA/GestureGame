//
// Created by kt-hyunjun on 2019-10-27.
//

#ifdef __ANDROID__

#include <LogUtils.h>
#include <control/AndroidSystemLayout.h>

#define ACTION_DOWN 		0
#define ACTION_UP 			1
#define ACTION_MOVE 		2
#define ACTION_CANCEL 		3

using namespace armakers;

AndroidSystemLayout::AndroidSystemLayout(std::string name, int w, int h, int layoutId)
 : SystemLayout(name, w, h, layoutId) {
	LOGD("AndroidSystemLayout create name : %s", name.c_str());
}

AndroidSystemLayout::~AndroidSystemLayout() {
    //LOGD("AndroidSystemLayout destroy name : %s", name.c_str());
	destroyObject();

	JNIEnv* env;
	if (javaVM->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK) {
		LOGD("Failed to get JNIEnv");
		return;
	}

	if (ViewGroupManagerObject) env->DeleteGlobalRef(ViewGroupManagerObject);
	if (IGLLayoutObject) env->DeleteGlobalRef(IGLLayoutObject);
}

void AndroidSystemLayout::initJavaVariable(JavaVM * javaVM, JNIEnv * env, jobject context) {
	this->javaVM = javaVM;

	jmethodID constructorMId;
	jclass ViewGroupManagerClass = env->FindClass("com/kt/library/opengl/view/ViewGroupManager");

	if (ViewGroupManagerClass == 0) {
		LOGE("Find ViewGroupManager Class Failed.\n");
	} else {
		LOGD("Found ViewGroupManager class.\n");
	}

	jmethodID instanceMId = env->GetStaticMethodID(ViewGroupManagerClass, "getInstance", "()Lcom/kt/library/opengl/view/ViewGroupManager;");
	if (instanceMId == 0) {
        LOGE("Find instanceMId method Failed.\n");
	} else{
		LOGD("Found instanceMId method.\n");
	}

    ViewGroupManagerObject = reinterpret_cast<jobject>(env->NewGlobalRef(env->CallStaticObjectMethod(ViewGroupManagerClass, instanceMId)));

	createAndAddMethodId = env->GetMethodID(ViewGroupManagerClass, "processCreateAndAdd", "(Ljava/lang/String;IIII)V");
	if (createAndAddMethodId == 0) {
        LOGE("Java createAndAddMethodId method is not found");
		return;
	}

	destroyAndRemoveMethodId = env->GetMethodID(ViewGroupManagerClass, "processDestroyAndRemove", "(Ljava/lang/String;)V");
	if (destroyAndRemoveMethodId == 0) {
        LOGE("Java destroyAndRemoveMethodId method is not found");
		return;
	}

	updateMethodId = env->GetMethodID(ViewGroupManagerClass, "processUpdate", "(Lcom/kt/library/opengl/view/IGLLayout;)V");
	if (updateMethodId == 0) {
        LOGE("Java updateMethodId method is not found");
		return;
	}

	touchEventMethodId = env->GetMethodID(ViewGroupManagerClass, "processTouchEvent", "(Lcom/kt/library/opengl/view/IGLLayout;III)V");
	if (touchEventMethodId == 0) {
        LOGE("Java touchEventMethodId method is not found");
		return;
	}

	getIGLLayoutMethodId = env->GetMethodID(ViewGroupManagerClass, "getGLInterface", "(Ljava/lang/String;)Lcom/kt/library/opengl/view/IGLLayout;");
	if (getIGLLayoutMethodId == 0) {
		LOGD("Java getIGLLayoutMethodId method is not found");
		return;
	}

    layoutSizeMethodId = env->GetMethodID(ViewGroupManagerClass, "layoutSize", "()[I");
    if (layoutSizeMethodId == 0) {
        LOGE("Java layoutSizeMethodId method is not found");
        return;
    }
}

bool AndroidSystemLayout::isDrawable(gameplay::ImageControl * imageControl, gameplay::Form * form) {
	JNIEnv* env;
	if (javaVM->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK) {
		return false;
	}

	if (!createAcquired) {
		if (width == 0 || height == 0) {
            LOGE("AndroidSystemLayout size error!!! w(%d), h(%d)", width, height);
			return false;
		}

		createAcquired = true;
        //updateSize();
        gameplay::Rectangle rect = form->getBounds();
        width = (int)rect.width;
        height = (int)rect.height;
        gameplay::Texture * texture = gameplay::Texture::create(gameplay::Texture::RGBA, width, height,
                0, false, gameplay::Texture::TEXTURE_2D);
        imageControl->setTexture(texture);
        createObject(width, height, texture->getHandle(), layoutId);
        LOGD("AndroidSystemLayout w(%d), h(%d)", width, height);
		keepRendering = true;
		return false;
	} else if (!IGLLayoutObject) {
		IGLLayoutObject = reinterpret_cast<jobject>(env->NewGlobalRef(env->CallObjectMethod(
				ViewGroupManagerObject, getIGLLayoutMethodId, env->NewStringUTF(name.c_str()))));
        LOGD("AndroidSystemLayout IGLLayoutObject(%p)", IGLLayoutObject);
	}

	return IGLLayoutObject != nullptr && keepRendering;
}


void AndroidSystemLayout::createObject(int width, int height, int textureId, int layoutId) {
	JNIEnv* env;
	if (javaVM->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK) {
		return;
	}

	LOGD("AndroidSystemLayout createObject name : %s", name.c_str());
	//LOGD("AndroidSystemLayout createObject w(%d)h(%d)tex(%d)res(%d)", width, height, textureId, layoutId);
	env->CallVoidMethod(ViewGroupManagerObject, createAndAddMethodId, env->NewStringUTF(name.c_str()),
	        width, height, textureId, layoutId);
}

void AndroidSystemLayout::update() {
	updateFrame();
}

void AndroidSystemLayout::destroyObject() {
	JNIEnv* env;
	LOGD("AndroidSystemLayout destroyObject name : %s", name.c_str());
	if (javaVM->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK) {
		return;
	}

	env->CallVoidMethod(ViewGroupManagerObject, destroyAndRemoveMethodId, env->NewStringUTF(name.c_str()));
	keepRendering = false;
    createAcquired = false;

	if (IGLLayoutObject) env->DeleteGlobalRef(IGLLayoutObject);
	IGLLayoutObject = nullptr;
}

void AndroidSystemLayout::onPressEvent(int contactIndex, int x, int y) {
	JNIEnv* env;
	//LOGD("AndroidSystemLayout onPressEvent : (%d)(%d)", x, height - y);
	if (javaVM->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK) {
		return;
	}
	//reverse event position axis y
	env->CallVoidMethod(ViewGroupManagerObject, touchEventMethodId, IGLLayoutObject,
	        ACTION_DOWN, x, /*height - y*/y);
}

void AndroidSystemLayout::onMoveEvent(int contactIndex, int x, int y) {
	JNIEnv* env;
	//LOGD("AndroidSystemLayout onMoveEvent : (%d)(%d)", x, height - y);
	if (javaVM->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK) {
		return;
	}
	//reverse event position axis y
	env->CallVoidMethod(ViewGroupManagerObject, touchEventMethodId, IGLLayoutObject,
						ACTION_MOVE, x, /*height - y*/y);
}

void AndroidSystemLayout::onReleaseEvent(int contactIndex, int x, int y) {
	JNIEnv* env;
	//LOGD("AndroidSystemLayout onReleaseEvent : (%d)(%d)", x, height - y);
	if (javaVM->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK) {
		return;
	}
    //reverse event position axis y
	env->CallVoidMethod(ViewGroupManagerObject, touchEventMethodId, IGLLayoutObject,
	        ACTION_UP, x, /*height - y*/y);
}

void AndroidSystemLayout::updateSize() {
    JNIEnv* env;
    int size[2] = {0, };
    if (javaVM->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK) {
        LOGE("Failed to get JNIEnv");
        return;
    }

    jintArray retval = (jintArray) env->CallObjectMethod(ViewGroupManagerObject, layoutSizeMethodId);
    env->GetIntArrayRegion(retval, 0, 2, (jint *)size);
    if (!size[0] || !size[1]) return;
#if 0
    float widthRatio  = (float)size[0] / (float)width;
    float heightRatio  = (float)size[1] / (float)height;
    float ratio = widthRatio < heightRatio ? widthRatio : heightRatio;
    size[0] = (int) (width * ratio);
    size[1] = (int) (height * ratio);
    width = size[0];
    height = size[1];
#else
    width = size[0];
    height = size[1];
#endif
}

void AndroidSystemLayout::updateFrame() {
	JNIEnv* env;

	if (javaVM->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK) {
		LOGE("Failed to get JNIEnv");
		return;
	}

	if (IGLLayoutObject) env->CallVoidMethod(ViewGroupManagerObject, updateMethodId, IGLLayoutObject);
}

std::shared_ptr<SystemLayout> SystemLayout::create(std::string name, int w, int h, int layoutId) {
	std::shared_ptr<AndroidSystemLayout> temp =
	        std::shared_ptr<AndroidSystemLayout>(new AndroidSystemLayout(name, w, h, layoutId));
	return std::static_pointer_cast<SystemLayout>(temp);
}

#endif // __ANDROID__
