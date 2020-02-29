//  Copyright © 2018년 Maxst. All rights reserved.

#ifdef __ANDROID__

#include "AndroidVideoPlayer.h"
#include <LogUtils.h>

using namespace gameplay;

AndroidVideoPlayer::AndroidVideoPlayer(std::string videoName)
 : VideoPlayer(videoName)
{
	LOGD("Video name : %s", videoName.c_str());
}

AndroidVideoPlayer::~AndroidVideoPlayer()
{
	destroy();

	JNIEnv* env;

	if (javaVM->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK)
	{
		LOGD("Failed to get JNIEnv");
		return;
	}

	env->DeleteGlobalRef(videoPlayerObject);
}

void AndroidVideoPlayer::initJavaVariable(JavaVM * javaVM, JNIEnv * env, jobject context)
{
	this->javaVM = javaVM;

	jmethodID constructorMId;
	jclass videoPlayerClass = env->FindClass("com/maxst/videoplayer/VideoPlayer");

	if (videoPlayerClass == 0) 
	{
		LOGD("Find VideoPlayer Class Failed.\n");
	}
	else 
	{
		LOGD("Found VideoPlayer class.\n");
	}

	constructorMId = env->GetMethodID(videoPlayerClass, "<init>", "(Landroid/content/Context;)V");
	if (constructorMId == 0)
	{
		LOGD("Find constructor method Failed.\n");
	}
	else 
	{
		LOGD("Found constructor method.\n");
	}

	videoPlayerObject = reinterpret_cast<jobject>(env->NewGlobalRef(env->NewObject(videoPlayerClass, constructorMId, context)));

	openMethodId = env->GetMethodID(videoPlayerClass, "openVideo", "(Ljava/lang/String;)Z");
	if (openMethodId == 0)
	{
		LOGD("Java openVideo method is not found");
		return;
	}

	getWidthMethodId = env->GetMethodID(videoPlayerClass, "getVideoWidth", "()I");
	if (getWidthMethodId == 0)
	{
		LOGD("Java getVideoWidth method is not found");
		return;
	}

	getHeightMethodId = env->GetMethodID(videoPlayerClass, "getVideoHeight", "()I");
	if (getHeightMethodId == 0)
	{
		LOGD("Java getVideoHeight method is not found");
		return;
	}

	getStateMethodId = env->GetMethodID(videoPlayerClass, "getState", "()I");
	if (getStateMethodId == 0)
	{
		LOGD("Java getState method is not found");
		return;
	}

	setTextureMethodId = env->GetMethodID(videoPlayerClass, "setTexture", "(I)V");
	if (setTextureMethodId == 0)
	{
		LOGD("Java setTexture method is not found");
		return;
	}

	startMethodId = env->GetMethodID(videoPlayerClass, "start", "()V");
	if (startMethodId == 0)
	{
		LOGD("Java start method is not found");
		return;
	}

	updateVideoFrameMethodId = env->GetMethodID(videoPlayerClass, "update", "()J");
	if (updateVideoFrameMethodId == 0)
	{
		LOGD("Java update method is not found");
		return;
	}

	pauseMethodId = env->GetMethodID(videoPlayerClass, "pause", "()V");
	if (pauseMethodId == 0)
	{
		LOGD("Java pause method is not found");
		return;
	}

	destroyMethodId = env->GetMethodID(videoPlayerClass, "destroy", "()V");
	if (destroyMethodId == 0)
	{
		LOGD("Java destroy method is not found");
		return;
	}

	isTextureDrawableMethodId = env->GetMethodID(videoPlayerClass, "isTextureDrawable", "()Z");
	if (isTextureDrawableMethodId == 0)
	{
		LOGD("Java isTextureDrawable method is not found");
		return;
	}
}

void AndroidVideoPlayer::open()
{
	JNIEnv* env;

	if (javaVM->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK)
	{
		return;
	}

	LOGD("Video name : %s", videoName.c_str());

	env->CallBooleanMethod(videoPlayerObject, openMethodId, env->NewStringUTF(videoName.c_str()));
}

void AndroidVideoPlayer::start()
{
	JNIEnv* env;

	if (javaVM->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK)
	{
		return;
	}

	env->CallVoidMethod(videoPlayerObject, startMethodId);
}

bool AndroidVideoPlayer::isDrawable()
{
	return videoSizeAcquired;
}

void AndroidVideoPlayer::update(Material* videoMaterial)
{
	if (!videoSizeAcquired)
	{
		int videoWidth = getVideoWidth();
		int videoHeight = getVideoHeight();

		if (videoWidth == 0 || videoHeight == 0) 
		{
			return;
		}

		videoSizeAcquired = true;

		if (_wpListener) _wpListener->updateVideoSize(videoWidth, videoHeight);

		Texture * videoTexture = Texture::create(gameplay::Texture::RGB888, videoWidth, videoHeight, 0, false, Texture::TEXTURE_2D);
		Texture::Sampler* sampler = Texture::Sampler::create(videoTexture);
		sampler->setFilterMode(Texture::LINEAR, Texture::LINEAR);
		sampler->setWrapMode(Texture::CLAMP, Texture::CLAMP);

		// TODO : Jack. For front camera ar
		videoMaterial->getStateBlock()->setCullFace(true);
		videoMaterial->getStateBlock()->setDepthTest(true);
		videoMaterial->getStateBlock()->setDepthWrite(true);

        videoMaterial->getStateBlock()->setBlend(true);
        videoMaterial->getStateBlock()->setBlendSrc(RenderState::BLEND_SRC_ALPHA);
        videoMaterial->getStateBlock()->setBlendDst(RenderState::BLEND_ONE_MINUS_SRC_ALPHA);

		videoMaterial->getParameter("u_diffuseTexture")->setValue(sampler);

		LOGD("Video width : %d, height : %d", videoWidth, videoHeight);
		LOGD("Video texture id : %d", videoTexture->getHandle());

		setTexture(videoTexture->getHandle());

		keepRendering = true;
		return;
	}

	if (!keepRendering)
	{
		return;
	}

	if (getState() != VideoState::STATE_PLAYING)
	{
		return;
	}

	updateVideoFrame();
}

void AndroidVideoPlayer::update(gameplay::ImageControl * imageControl)
{
	if (!videoSizeAcquired)
	{
		int videoWidth = getVideoWidth();
		int videoHeight = getVideoHeight();

		if (videoWidth == 0 || videoHeight == 0)
		{
			return;
		}

		videoSizeAcquired = true;

		Texture * videoTexture = Texture::create(gameplay::Texture::RGB888, videoWidth, videoHeight, 0, false, Texture::TEXTURE_2D);
		imageControl->setTexture(videoTexture);

		LOGD("Video width : %d, height : %d", videoWidth, videoHeight);
		LOGD("Video texture id : %d", videoTexture->getHandle());

		setTexture(videoTexture->getHandle());

		keepRendering = true;
		return;
	}

	if (!keepRendering)
	{
		return;
	}

	if (getState() != VideoState::STATE_PLAYING)
	{
		return;
	}

	updateVideoFrame();
}

void AndroidVideoPlayer::pause()
{
	JNIEnv* env;

	if (javaVM->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK)
	{
		return;
	}

	env->CallVoidMethod(videoPlayerObject, pauseMethodId);
}

void AndroidVideoPlayer::destroy()
{
	JNIEnv* env;

	if (javaVM->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK)
	{
		return;
	}

	env->CallVoidMethod(videoPlayerObject, destroyMethodId);
	videoSizeAcquired = false;
	keepRendering = false;
}

int AndroidVideoPlayer::getVideoWidth()
{
	JNIEnv* env;

	if (javaVM->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK)
	{
		LOGD("Failed to get JNIEnv");
		return 0;
	}

	return env->CallIntMethod(videoPlayerObject, getWidthMethodId);
}

int AndroidVideoPlayer::getVideoHeight()
{
	JNIEnv* env;

	if (javaVM->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK)
	{
		LOGD("Failed to get JNIEnv");
		return 0;
	}

	return env->CallIntMethod(videoPlayerObject, getHeightMethodId);
}

int AndroidVideoPlayer::getState()
{
	JNIEnv* env;

	if (javaVM->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK)
	{
		LOGD("Failed to get JNIEnv");
		return false;
	}

	return env->CallIntMethod(videoPlayerObject, getStateMethodId);
}

void AndroidVideoPlayer::setTexture(int textureHandle)
{
	JNIEnv* env;

	if (javaVM->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK)
	{
		LOGD("Failed to get JNIEnv");
		return;
	}

	env->CallVoidMethod(videoPlayerObject, setTextureMethodId, textureHandle);
}

void AndroidVideoPlayer::updateVideoFrame()
{
	JNIEnv* env;

	if (javaVM->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK)
	{
		LOGD("Failed to get JNIEnv");
		return;
	}

	env->CallLongMethod(videoPlayerObject, updateVideoFrameMethodId);
}

std::shared_ptr<VideoPlayer> VideoPlayer::create(std::string videoName)
{
	std::shared_ptr<AndroidVideoPlayer> temp = std::shared_ptr<AndroidVideoPlayer>(new AndroidVideoPlayer(videoName));
	return std::static_pointer_cast<VideoPlayer>(temp);
}

#endif // __ANDROID__