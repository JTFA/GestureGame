//  Copyright © 2018년 Maxst. All rights reserved.

#pragma once

#ifdef __ANDROID__
#include <jni.h>
#include <control/WebView.h>

namespace armakers {
	class AndroidWebView : public WebView {
	public:
		AndroidWebView(std::string name, std::string url, int w, int h);

		~AndroidWebView();

		void initJavaVariable(JavaVM *javaVM, JNIEnv *env, jobject context);

		virtual bool isDrawable(gameplay::Material *material);

		virtual void createObject(int width, int height, int textureId);

		virtual void update();

		virtual void destroyObject();

	private:
		void updateSize();

		void updateFrame();

		bool createAcquired = false;
		bool keepRendering = false;

		JavaVM *javaVM = nullptr;
		jobject ViewGroupManagerObject = 0;
		jobject GLWebViewObject = 0;
		jmethodID createAndAddMethodId = 0;
		jmethodID destroyAndRemoveMethodId = 0;
		jmethodID updateMethodId = 0;
		jmethodID touchEventMethodId = 0;
		jmethodID getGLWebViewMethodId = 0;
		jmethodID layoutSizeMethodId = 0;
		jclass webInfoClass = 0;
		jmethodID constructMethodId = 0;

	};
}
#endif // __ANDROID__
