//  Copyright © 2018년 Maxst. All rights reserved.

#pragma once

#ifdef __ANDROID__
#include <jni.h>
#include "VideoPlayer.h"

namespace gameplay {
	class AndroidVideoPlayer : public VideoPlayer {
	public:
		AndroidVideoPlayer(std::string videoName);

		~AndroidVideoPlayer();

		void initJavaVariable(JavaVM *javaVM, JNIEnv *env, jobject context);

		void open();

		void start();

		bool isDrawable();

		void update(gameplay::Material *videoMaterial);

		void update(gameplay::ImageControl *imageControl);

		void pause();

		void destroy();

		int getState();

		int getVideoWidth();

		int getVideoHeight();

	private:
		void setTexture(int textureHandle);

		void updateVideoFrame();

		bool keepRendering = false;

		JavaVM *javaVM = nullptr;
		jobject videoPlayerObject = 0;
		jmethodID openMethodId = 0;
		jmethodID getWidthMethodId = 0;
		jmethodID getHeightMethodId = 0;
		jmethodID setTextureMethodId = 0;
		jmethodID startMethodId = 0;
		jmethodID getStateMethodId = 0;
		jmethodID pauseMethodId = 0;
		jmethodID updateVideoFrameMethodId = 0;
		jmethodID destroyMethodId = 0;
		jmethodID isTextureDrawableMethodId = 0;
	};
}
#endif // __ANDROID__
