#pragma once

#include <VideoPlayer.h>
#include <QuadNode.h>
#include <node/ExtensionRenderable.h>

namespace armakers {

	class TemplateVideoNode : public gameplay::QuadNode, public ExtensionRenderable {
	public:
		TemplateVideoNode(const char *id, std::shared_ptr<gameplay::VideoPlayer> videoPlayer);

		TemplateVideoNode(const char *id, std::shared_ptr<gameplay::VideoPlayer> videoPlayer, float alpha);

		TemplateVideoNode(const char *id, std::shared_ptr<gameplay::VideoPlayer> videoPlayer,
				float alpha, bool isDepthWrite);

		virtual ~TemplateVideoNode();

        virtual void extensionUpdate(int32_t elapsedTime);

        void startPlaying();

		void pausePlaying();

		void setEnabled(bool enabled);

	private:
		std::shared_ptr<gameplay::VideoPlayer> _videoPlayer = nullptr;
		bool isUserPause;
	};
}


