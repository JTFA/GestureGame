//  Copyright © 2018년 Maxst. All rights reserved.

#ifdef __ANDROID__

#include <LogUtils.h>
#include <control/AndroidWebView.h>

using namespace armakers;

AndroidWebView::AndroidWebView(std::string name, std::string url, int w, int h)
 : WebView(name, url, w, h) {
	LOGD("name : %s", name.c_str());
}

AndroidWebView::~AndroidWebView() {
	destroyObject();

	JNIEnv* env;
	if (javaVM->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK) {
		LOGD("Failed to get JNIEnv");
		return;
	}

	if (ViewGroupManagerObject) env->DeleteGlobalRef(ViewGroupManagerObject);
	if (GLWebViewObject) env->DeleteGlobalRef(GLWebViewObject);
	if (webInfoClass) env->DeleteGlobalRef(webInfoClass);
}

void AndroidWebView::initJavaVariable(JavaVM * javaVM, JNIEnv * env, jobject context) {
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

	createAndAddMethodId = env->GetMethodID(ViewGroupManagerClass, "processCreateAndAdd", "(Ljava/lang/String;IIILcom/kt/library/opengl/model/WebInfo;)V");
	if (createAndAddMethodId == 0) {
		LOGE("Java createAndAddMethodId method is not found");
		return;
	}

	destroyAndRemoveMethodId = env->GetMethodID(ViewGroupManagerClass, "processDestroyAndRemove", "(Ljava/lang/String;)V");
	if (destroyAndRemoveMethodId == 0) {
		LOGD("Java destroyAndRemoveMethodId method is not found");
		return;
	}

	updateMethodId = env->GetMethodID(ViewGroupManagerClass, "processUpdate", "(Lcom/kt/library/opengl/view/GLWebView;)V");
	if (updateMethodId == 0) {
		LOGE("Java updateMethodId method is not found");
		return;
	}

	touchEventMethodId = env->GetMethodID(ViewGroupManagerClass, "processTouchEvent", "(Lcom/kt/library/opengl/view/GLWebView;III)V");
	if (touchEventMethodId == 0) {
		LOGE("Java touchEventMethodId method is not found");
		return;
	}

	getGLWebViewMethodId = env->GetMethodID(ViewGroupManagerClass, "getGLWebView", "(Ljava/lang/String;)Lcom/kt/library/opengl/view/GLWebView;");
	if (getGLWebViewMethodId == 0) {
		LOGE("Java getGLWebViewMethodId method is not found");
		return;
	}

	layoutSizeMethodId = env->GetMethodID(ViewGroupManagerClass, "layoutSize", "()[I");
	if (layoutSizeMethodId == 0) {
        LOGE("Java layoutSizeMethodId method is not found");
        return;
    }

	webInfoClass = reinterpret_cast<jclass>(env->NewGlobalRef(env->FindClass("com/kt/library/opengl/model/WebInfo")));
	if (webInfoClass == 0) {
		LOGD("Find webInfoClass Class Failed.\n");
	} else {
		LOGD("Found webInfoClass class.\n");
	}

	constructMethodId = env->GetMethodID(webInfoClass, "<init>", "(Ljava/lang/String;Ljava/util/Map;)V");
	if (constructMethodId == 0)
	{
		LOGD("Java webinfo constructMethodId method is not found");
		return;
	}
}

bool AndroidWebView::isDrawable(gameplay::Material* material) {
	JNIEnv* env;
	if (javaVM->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK) {
		return false;
	}

	if (!createAcquired) {
		if (width == 0 || height == 0) {
			return false;
		}

		createAcquired = true;
        updateSize();

		gameplay::Texture * texture = gameplay::Texture::create(gameplay::Texture::RGB888, width, height,
		        0, false, gameplay::Texture::TEXTURE_2D);
		gameplay::Texture::Sampler* sampler = gameplay::Texture::Sampler::create(texture);
		sampler->setFilterMode(gameplay::Texture::LINEAR, gameplay::Texture::LINEAR);
		sampler->setWrapMode(gameplay::Texture::CLAMP, gameplay::Texture::CLAMP);

		// TODO : Jack. For front camera ar
		material->getStateBlock()->setCullFace(true);
		material->getStateBlock()->setDepthTest(true);
		material->getStateBlock()->setDepthWrite(true);

		material->getStateBlock()->setBlend(true);
		material->getStateBlock()->setBlendSrc(gameplay::RenderState::BLEND_SRC_ALPHA);
		material->getStateBlock()->setBlendDst(gameplay::RenderState::BLEND_ONE_MINUS_SRC_ALPHA);

		material->getParameter("u_diffuseTexture")->setValue(sampler);

		LOGD("WebView width : %d, height : %d", width, height);
		LOGD("WebView texture id : %d", texture->getHandle());

		createObject(width, height, texture->getHandle());
		keepRendering = true;
		return false;
	} else if (!GLWebViewObject) {
		GLWebViewObject = reinterpret_cast<jobject>(env->NewGlobalRef(env->CallObjectMethod(
				ViewGroupManagerObject, getGLWebViewMethodId, env->NewStringUTF(name.c_str()))));
	}

	return GLWebViewObject != nullptr && keepRendering;
}


void AndroidWebView::createObject(int width, int height, int textureId) {
	JNIEnv* env;
	if (javaVM->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK) {
		return;
	}

	LOGD("WebView createObject name : %s", name.c_str());
	jobject jWebinfo = env->NewObject(webInfoClass, constructMethodId,
									  env->NewStringUTF(url.c_str()), nullptr);
	env->CallVoidMethod(ViewGroupManagerObject, createAndAddMethodId, env->NewStringUTF(name.c_str()),
	        width, height, textureId, jWebinfo);
	env->DeleteLocalRef(jWebinfo);
}

void AndroidWebView::update() {
	updateFrame();
}

void AndroidWebView::destroyObject() {
	JNIEnv* env;
	LOGD("WebView destroyObject name : %s", name.c_str());
	if (javaVM->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK) {
		return;
	}

	env->CallVoidMethod(ViewGroupManagerObject, destroyAndRemoveMethodId, env->NewStringUTF(name.c_str()));
	keepRendering = false;
    createAcquired = false;
}

void AndroidWebView::updateSize() {
    JNIEnv* env;
    int size[2] = {0, };
    if (javaVM->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK) {
        LOGD("Failed to get JNIEnv");
        return;
    }

    jintArray jSize = (jintArray) env->CallObjectMethod(ViewGroupManagerObject, layoutSizeMethodId);
	env->GetIntArrayRegion(jSize, 0, 2, (jint *)size);
	if (!size[0] || !size[1]) return;

    float widthRatio  = (float)size[0] / (float)width;
    float heightRatio  = (float)size[1] / (float)height;
    float ratio = widthRatio < heightRatio ? widthRatio : heightRatio;
    size[0] = (int) (width * ratio);
    size[1] = (int) (height * ratio);
    width = size[0];
    height = size[1];
}

void AndroidWebView::updateFrame() {
	JNIEnv* env;

	if (javaVM->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK) {
		LOGE("Failed to get JNIEnv");
		return;
	}

	if (GLWebViewObject) env->CallVoidMethod(ViewGroupManagerObject, updateMethodId, GLWebViewObject);
}

std::shared_ptr<WebView> WebView::create(std::string name, std::string url, int w, int h) {
	std::shared_ptr<AndroidWebView> temp =
	        std::shared_ptr<AndroidWebView>(new AndroidWebView(name, url, w, h));
	return std::static_pointer_cast<WebView>(temp);
}

#endif // __ANDROID__