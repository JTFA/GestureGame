//  Copyright © 2018년 Maxst. All rights reserved.

#include "ARNodeContainer.h"
#include <LogUtils.h>

using namespace gameplay;

int ARNodeContainer::containerId = 0;

ARNodeContainer::ARNodeContainer(const int newId, const char * newTrackableId) : id(newId), trackableId(newTrackableId), isAugmentOrder(false), orderDuration(0), orderElapsedTime(0)
{
}

ARNodeContainer::~ARNodeContainer()
{
    videoRendererList.clear();
}

void ARNodeContainer::addNode(QuadNode * newNode)
{
    newNode->setAnimationListener(this);
    nodeList.push_back(newNode);
}

QuadNode* ARNodeContainer::findNode(const char* id)
{
    for (auto itor = nodeList.begin(); itor != nodeList.end(); itor++) {
        if (strcmp((*itor)->getId(), id) == 0)
        {
            return (*itor);
        }
    }
    
    return nullptr;
}

void ARNodeContainer::addVideoRenderer(std::shared_ptr<VideoRenderer> videoRenderer, QuadNode * node)
{
    node->setAnimationListener(this);
    videoRendererList.push_back(videoRenderer);
    nodeList.push_back(node);
}

void ARNodeContainer::setEnabled(bool toggle, float elapsedTime)
{
    this->enabled = toggle;
    
    if (isAugmentOrder) {
        augmentOrder(toggle, elapsedTime);
    } else {
        augmentBatch(toggle, elapsedTime);
        updateVideo(toggle, elapsedTime);
    }
}

void ARNodeContainer::augmentOrder(bool enabled, float elapsedTime)
{
    if (!enabled) {
        orderElapsedTime = 0;
        augmentVideo(false);
        updateVideo(false, elapsedTime);
        augmentContent(false, elapsedTime);
        return;
    }
    
    orderElapsedTime += elapsedTime;
    
    int orderIndex = orderElapsedTime / orderDuration;
    
    if (orderIndex < 0 || orderIndex >= orderList.size())
    {
        augmentVideo(false);
        updateVideo(false, elapsedTime);
        augmentContent(false, elapsedTime);
        return;
    }
    
    const char* currentType = orderList.at(orderIndex);
    
    if (orderIndex > 0)
    {
        const char* prevType = orderList.at(orderIndex - 1);
        if (strcmp(prevType, "video") == 0)
        {
            augmentVideo(false);
            updateVideo(false, elapsedTime);
        } else
        {
            augmentContent(false, elapsedTime, prevType);
        }
    }
    
    if (strcmp(currentType, "video") == 0)
    {
        augmentVideo(enabled);
        updateVideo(enabled, elapsedTime);
    } else
    {
        augmentContent(enabled, elapsedTime, currentType);
    }
}

void ARNodeContainer::augmentBatch(bool enabled, float elapsedTime)
{
    augmentVideo(enabled);
    augmentContent(enabled, elapsedTime);
}

void ARNodeContainer::augmentVideo(bool enabled)
{
    for (std::list<std::shared_ptr<VideoRenderer>>::iterator itor = videoRendererList.begin();
         itor != videoRendererList.end();
         itor++)
    {
        std::shared_ptr<VideoPlayer> videoPlayer = (*itor)->getVideoPlayer();
        int videoPlayerState = videoPlayer->getState();
        if (enabled)
        {
            if (videoPlayerState == VideoState::STATE_READY || videoPlayerState == VideoState::STATE_PAUSE)
            {
                videoPlayer->start();
            }
        } else
        {
            videoPlayer->pause();
        }
    }
}

void ARNodeContainer::augmentContent(bool enabled, float elapsedTime, const char* type)
{
    for (std::list<QuadNode *>::iterator itor = nodeList.begin();
         itor != nodeList.end();
         itor++)
    {
        bool isAbleAugment = (type != nullptr) ? strcmp(type, (*itor)->getType()) == 0 : true;
        
        if (isAbleAugment)
        {
            (*itor)->setEnabled(enabled, elapsedTime);
        }
    }
}

bool ARNodeContainer::getEnabled()
{
    return this->enabled;
}

void ARNodeContainer::updateVideo(bool enabled, float elapsedTime)
{
    if (enabled)
    {
        for (std::list<std::shared_ptr<VideoRenderer>>::iterator it = videoRendererList.begin();
             it != videoRendererList.end(); it++)
        {
            int videoPlayerState = (*it)->getVideoPlayer()->getState();
            if (videoPlayerState == VideoState::STATE_PLAYING)
            {
                (*it)->updateVideo(elapsedTime);
            }
        }
    }
}

void ARNodeContainer::setAugmentOrder(const char* type1, const char* type2, const char* type3, float duration)
{
    isAugmentOrder = true;
    orderList.clear();
    orderList.push_back(type1);
    orderList.push_back(type2);
    orderList.push_back(type3);
    
    orderDuration = duration;
}

void ARNodeContainer::setAugmentBatch()
{
    isAugmentOrder = false;
}

void ARNodeContainer::onBeginAnimation(QuadNode::AnimationListener::AnimationType type, QuadNode *node)
{
    
}

void ARNodeContainer::onEndAnimation(QuadNode::AnimationListener::AnimationType type, QuadNode *node)
{
    
}

void ARNodeContainer::touchEvent(Touch::TouchEvent event, int x, int y)
{
    for (auto itor = nodeList.begin(); itor != nodeList.end(); itor++)
    {
        QuadNode* node = (*itor);
        if (node->isEnabled())
        {
            node->touchEvent(event, x, y);
        }
    }
}
