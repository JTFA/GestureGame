
#include <LogUtils.h>
#include <node/TemplateVideoNode.h>

using namespace armakers;

TemplateVideoNode::TemplateVideoNode(const char *id, std::shared_ptr<gameplay::VideoPlayer> videoPlayer) :
		gameplay::QuadNode(id, gameplay::QuadNodeType::VIDEO, true)  {
    //TODO:: 임시 코드. 초기 텍스쳐가 없을 때 마지막에 생성한 텍스쳐가 일시적으로 보이는 현상이 나타남
    setTexture("res/png/video_icon.png", true);
    _videoPlayer = videoPlayer;
	_videoPlayer->open();
	_videoPlayer->pause();
	isUserPause = false;
}

TemplateVideoNode::TemplateVideoNode(const char *id, std::shared_ptr<gameplay::VideoPlayer> videoPlayer, float alpha) :
		QuadNode(id, gameplay::QuadNodeType::VIDEO, alpha)  {
	//TODO:: 임시 코드. 초기 텍스쳐가 없을 때 마지막에 생성한 텍스쳐가 일시적으로 보이는 현상이 나타남
	setTexture("res/png/video_icon.png", true);
    _videoPlayer = videoPlayer;
	_videoPlayer->open();
	_videoPlayer->pause();
	isUserPause = false;
}

TemplateVideoNode::TemplateVideoNode(const char *id, std::shared_ptr<gameplay::VideoPlayer> videoPlayer
		, float alpha, bool isDepthWrite) :
		QuadNode(id, gameplay::QuadNodeType::VIDEO, alpha, isDepthWrite)  {
	//TODO:: 임시 코드. 초기 텍스쳐가 없을 때 마지막에 생성한 텍스쳐가 일시적으로 보이는 현상이 나타남
	setTexture("res/png/video_icon.png", true);
	_videoPlayer = videoPlayer;
	_videoPlayer->open();
	_videoPlayer->pause();
	isUserPause = false;
}



TemplateVideoNode::~TemplateVideoNode() {
	LOGD("TemplateVideoNode Destructor");
	_videoPlayer->pause();
	_videoPlayer->destroy();
}

void TemplateVideoNode::startPlaying() {
	isUserPause = false;
    int state = _videoPlayer->getState();
    if (state == gameplay::VideoPlayer::VideoState::STATE_READY || state == gameplay::VideoPlayer::VideoState::STATE_PAUSE) {
        _videoPlayer->start();
    }
}

void TemplateVideoNode::pausePlaying() {
	isUserPause = true;
	int state = _videoPlayer->getState();
	if (state == gameplay::VideoPlayer::VideoState::STATE_PLAYING) {
		_videoPlayer->pause();
	}
}

void TemplateVideoNode::extensionUpdate(int32_t elapsedTime) {

	int state = _videoPlayer->getState();
	if (state == gameplay::VideoPlayer::VideoState::STATE_ERROR || !isEnabled()) {
		return;
	}

	if (state == gameplay::VideoPlayer::VideoState::STATE_READY || state == gameplay::VideoPlayer::VideoState::STATE_PAUSE) {
        if (_videoPlayer->isDrawable() && !isUserPause) {
            LOGD("Video. video start : %s", getId());
			_videoPlayer->start();
        }
	}

	material->getStateBlock()->setCullFace(false);
	_videoPlayer->update(material);
}

void TemplateVideoNode::setEnabled(bool enabled)
{
	//LOGD("Video enabled : %d", enabled);
	QuadNode::setEnabled(enabled);
	if (enabled)
	{
        int state = _videoPlayer->getState();
        if (state == gameplay::VideoPlayer::VideoState::STATE_READY || state == gameplay::VideoPlayer::VideoState::STATE_PAUSE) {
			if (_videoPlayer->isDrawable() && !isUserPause) {
				_videoPlayer->start();
			}
        }
	}
	else
	{
		_videoPlayer->pause();
	}
}