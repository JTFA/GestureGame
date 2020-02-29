//  Copyright © 2018년 Maxst. All rights reserved.

#include "WindowVideoPlayer.h"
#include <LogUtils.h>

using namespace gameplay;

WindowVideoPlayer::WindowVideoPlayer(std::string videoName)
 : VideoPlayer(videoName)
{
	LOGD("Video name : %s", videoName.c_str());
}

WindowVideoPlayer::~WindowVideoPlayer()
{
	destroy();
}

void WindowVideoPlayer::open()
{
}

void WindowVideoPlayer::start()
{
}

bool WindowVideoPlayer::isDrawable()
{
	return videoSizeAcquired;
}

void WindowVideoPlayer::update(Material* videoMaterial)
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
		Texture::Sampler* sampler = Texture::Sampler::create(videoTexture);
		sampler->setFilterMode(Texture::LINEAR, Texture::LINEAR);
		sampler->setWrapMode(Texture::CLAMP, Texture::CLAMP);

		// TODO : Jack. For front camera ar
		videoMaterial->getStateBlock()->setCullFace(true);
		videoMaterial->getStateBlock()->setDepthTest(true);
		videoMaterial->getStateBlock()->setDepthWrite(true);
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

void WindowVideoPlayer::pause()
{
}

void WindowVideoPlayer::destroy()
{
	keepRendering = false;
}

int WindowVideoPlayer::getVideoWidth()
{
	return 0;
}

int WindowVideoPlayer::getVideoHeight()
{
	return 0;
}

int WindowVideoPlayer::getState()
{
	return 0;
}

void WindowVideoPlayer::setTexture(int textureHandle)
{
}

void WindowVideoPlayer::updateVideoFrame()
{

}

std::shared_ptr<VideoPlayer> VideoPlayer::create(std::string videoName)
{
	std::shared_ptr<WindowVideoPlayer> temp = std::shared_ptr<WindowVideoPlayer>(new WindowVideoPlayer(videoName));
	return std::static_pointer_cast<VideoPlayer>(temp);
}