
#include <LogUtils.h>
#include <VideoNode.h>

using namespace gameplay;

VideoNode::VideoNode(const char *id, std::shared_ptr<VideoPlayer> videoPlayer) :
		QuadNode(id, QuadNodeType::VIDEO, true)  {
    //TODO:: 임시 코드. 초기 텍스쳐가 없을 때 마지막에 생성한 텍스쳐가 일시적으로 보이는 현상이 나타남
    setTexture("res/png/video_icon.png", true);
	videoPlayer->open();
	videoPlayer->pause();

	this->videoPlayer = videoPlayer;
}

VideoNode::VideoNode(const char *id, std::shared_ptr<VideoPlayer> videoPlayer, float alpha) :
		QuadNode(id, QuadNodeType::VIDEO, alpha)  {
	//TODO:: 임시 코드. 초기 텍스쳐가 없을 때 마지막에 생성한 텍스쳐가 일시적으로 보이는 현상이 나타남
	setTexture("res/png/video_icon.png", true);
	videoPlayer->open();
	videoPlayer->pause();

	this->videoPlayer = videoPlayer;
}

VideoNode::VideoNode(const char *id, std::shared_ptr<VideoPlayer> videoPlayer, float alpha, bool isDepthWrite) :
        QuadNode(id, QuadNodeType::VIDEO, alpha, isDepthWrite)  {
    //TODO:: 임시 코드. 초기 텍스쳐가 없을 때 마지막에 생성한 텍스쳐가 일시적으로 보이는 현상이 나타남
    setTexture("res/png/video_icon.png", true);
    videoPlayer->open();
    videoPlayer->pause();

    this->videoPlayer = videoPlayer;
}

VideoNode::~VideoNode() {
	LOGD("VideoNode Destructor");
	videoPlayer->pause();
	videoPlayer->destroy();
}

void VideoNode::stopPlaying() {
	videoPlayer->pause();
}

void VideoNode::update() {
	int state = videoPlayer->getState();
	if (state == VideoPlayer::VideoState::STATE_ERROR || !isEnabled()) {
		return;
	}

	if (state == VideoPlayer::VideoState::STATE_READY || state == VideoPlayer::VideoState::STATE_PAUSE) {
        //TODO:: prepareAsync 이후에 start 호출 될 수 있도록 임시 방편 코드. 멀티쓰레드 문제가 있어 보임.
        if (videoPlayer->isDrawable()) {
            LOGD("Video. video start : %s", getId());
            videoPlayer->start();
        }
	}

	material->getStateBlock()->setCullFace(false);
	videoPlayer->update(material);
}

void VideoNode::setEnabled(bool enabled)
{
//	LOGD("Video enabled : %d", enabled);
	QuadNode::setEnabled(enabled);
	if (enabled)
	{
        //TODO:: update 함수와 동일한 이유로 수정
        int state = videoPlayer->getState();
        if (state == VideoPlayer::VideoState::STATE_READY || state == VideoPlayer::VideoState::STATE_PAUSE) {
			if (videoPlayer->isDrawable()) {
				videoPlayer->start();
			}
        }
	}
	else
	{
		videoPlayer->pause();
	}
}