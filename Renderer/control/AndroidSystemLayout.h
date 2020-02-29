//
// Created by kt-hyunjun on 2019-10-22.
//

#pragma once

#ifdef __ANDROID__
#include <jni.h>
#include <control/SystemLayout.h>

namespace armakers {
	class AndroidSystemLayout : public SystemLayout {
	public:
		AndroidSystemLayout(std::string name, int w, int h, int layoutId);

		~AndroidSystemLayout();

		void initJavaVariable(JavaVM *javaVM, JNIEnv *env, jobject context);

		virtual bool isDrawable(gameplay::ImageControl *imageControl, gameplay::Form *form);

		virtual void createObject(int width, int height, int textureId, int layoutId);

		virtual void update();

		virtual void destroyObject();

		virtual void onPressEvent(int contactIndex, int x, int y);

		virtual void onMoveEvent(int contactIndex, int x, int y);

		virtual void onReleaseEvent(int contactIndex, int x, int y);

	private:
		void updateSize();

		void updateFrame();

		bool createAcquired = false;
		bool keepRendering = false;

		JavaVM *javaVM = nullptr;
		jobject ViewGroupManagerObject = 0;
		jobject IGLLayoutObject = 0;
		jmethodID createAndAddMethodId = 0;
		jmethodID destroyAndRemoveMethodId = 0;
		jmethodID updateMethodId = 0;
		jmethodID touchEventMethodId = 0;
		jmethodID getIGLLayoutMethodId = 0;
		jmethodID layoutSizeMethodId = 0;
	};
}
#endif // __ANDROID__