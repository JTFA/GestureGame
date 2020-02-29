#pragma once

#include <VideoPlayer.h>
#include <FormNode.h>

namespace gameplay {
	class VideoLayoutFormNode : public FormNode {
	public:
		VideoLayoutFormNode(char *controlId, std::shared_ptr<VideoPlayer> videoPlayer);

		~VideoLayoutFormNode();

		void drawForm(bool wireFrame);

		void controlEvent(Control *control, EventType evt);

		void update();

		void stopPlaying();

		void setEnabled(bool enabled);

	private:
		std::shared_ptr<VideoPlayer> videoPlayer = nullptr;
		ImageControl *imageControl;
		bool isPlaying;
	};
}