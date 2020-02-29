//  Copyright © 2018년 Maxst. All rights reserved.

#pragma once

#include <string>
#include <memory>
#include <map>
#include <list>
#include "gameplay.h"
#include "VideoRenderer.h"
#include "QuadNode.h"

namespace gameplay {
	class ARNodeContainer : public QuadNode::AnimationListener {
	public:
		static int containerId;

		ARNodeContainer(const int id, const char *trackableId);

		~ARNodeContainer();

		void addNode(QuadNode *);

		void addVideoRenderer(std::shared_ptr <VideoRenderer> videoRenderer, QuadNode *node);

		void setEnabled(bool toggle, float elapsedTime);

		bool getEnabled();

		void updateVideo(bool enabled, float elapsedTime);

		const int id;
		const char *trackableId;

		void
		setAugmentOrder(const char *type1, const char *type2, const char *type3, float duration);

		void setAugmentBatch();


		/**
         * @see ARNode::AnimationListener::onBeginAnimation
         */
		void onBeginAnimation(AnimationType type, QuadNode *node);

		/**
         * @see ARNode::AnimationListener::onEndAnimation
         */
		void onEndAnimation(AnimationType type, QuadNode *node);

		void touchEvent(Touch::TouchEvent event, int x, int y);

		QuadNode *findNode(const char *id);

	private:
		enum VideoState {
			STATE_NONE = 0,
			STATE_READY = 1,
			STATE_PLAYING = 2,
			STATE_PAUSE = 3,
			STATE_DESTROYED = 4,
			STATE_ERROR = 100
		};

		void augmentOrder(bool enabled, float elapsedTime);

		void augmentBatch(bool enabled, float elapsedTime);

		void augmentVideo(bool enabled);

		void augmentContent(bool enabled, float elapsedTime, const char *type = nullptr);

		std::list <std::shared_ptr<VideoRenderer>> videoRendererList;
		std::list<QuadNode *> nodeList;
		bool enabled = false;

		float orderElapsedTime;
		float beginOrderElapsedTime;

		bool isAugmentOrder = false;
		float orderDuration;
		std::vector<const char *> orderList;
	};
}
