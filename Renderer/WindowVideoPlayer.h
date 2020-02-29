//  Copyright © 2018년 Maxst. All rights reserved.

#pragma once

#include "VideoPlayer.h"

namespace gameplay {
	class WindowVideoPlayer : public VideoPlayer {
	public:
		WindowVideoPlayer(std::string videoName);

		~WindowVideoPlayer();

		void open();

		void start();

		bool isDrawable();

		void update(gameplay::Material *videoMaterial);

		void pause();

		void destroy();

		int getState();

		int getVideoWidth();

		int getVideoHeight();

	private:
		void setTexture(int textureHandle);

		void updateVideoFrame();

		bool keepRendering = false;
	};
}
