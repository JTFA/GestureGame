//  Copyright © 2018년 Maxst. All rights reserved.

#pragma once

#include <string>
#include <memory>
#include "gameplay.h"

namespace gameplay {
	class VideoSizeListener {
	public:
		VideoSizeListener() {};
		virtual ~VideoSizeListener() {};

		virtual void updateVideoSize(int32_t w, int32_t h) = 0;
	};

	class VideoPlayer {
	public:
		enum VideoState {
			STATE_NONE = 0,
			STATE_READY = 1,
			STATE_PLAYING = 2,
			STATE_PAUSE = 3,
			STATE_DESTROYED = 4,
			STATE_ERROR = 100
		};

		static std::shared_ptr<VideoPlayer> create(std::string videoName);

		VideoPlayer(std::string videoName) { this->videoName = videoName; }

		virtual ~VideoPlayer() {}

		virtual void open() = 0;

		virtual void start() = 0;

		virtual bool isDrawable() = 0;

		virtual void update(gameplay::Material *videoMaterial) = 0;

		virtual void update(gameplay::ImageControl *imageControl) {};

		virtual void pause() = 0;

		virtual void destroy() = 0;

		virtual int getState() = 0;

		virtual int getVideoWidth() = 0;

		virtual int getVideoHeight() = 0;

		const char *getVideoName() { return videoName.c_str(); }

		void listener(VideoSizeListener *l) { _wpListener = l; }

	protected:
		bool videoSizeAcquired = false;
		std::string videoName;
		VideoSizeListener *_wpListener = nullptr;
	};
}
