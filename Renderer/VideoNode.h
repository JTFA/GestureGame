#pragma once

#include <VideoPlayer.h>
#include <QuadNode.h>

namespace gameplay {
	class VideoNode : public QuadNode {
	public:
		VideoNode(const char *id, std::shared_ptr<VideoPlayer> videoPlayer);

		VideoNode(const char *id, std::shared_ptr<VideoPlayer> videoPlayer, float alpha);

		VideoNode(const char *id, std::shared_ptr<VideoPlayer> videoPlayer, float alpha, bool isDepthWrite);

		virtual ~VideoNode();

		void update();

		void stopPlaying();

		void setEnabled(bool enabled);

	private:
		std::shared_ptr<VideoPlayer> videoPlayer = nullptr;
	};
}


