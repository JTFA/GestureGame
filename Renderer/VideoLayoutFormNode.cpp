#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG "FormNode"
#endif

#include <LogUtils.h>
#include <VideoLayoutFormNode.h>

using namespace gameplay;

VideoLayoutFormNode::~VideoLayoutFormNode() {
	videoPlayer->pause();
	videoPlayer->destroy();
}

VideoLayoutFormNode::VideoLayoutFormNode(char * controlId, std::shared_ptr<VideoPlayer> videoPlayer) : FormNode("videoLayout") {

	imageControl = dynamic_cast<ImageControl *>(form->getControl("image"));
	if (imageControl == nullptr) {
		LOGD("imageTitleControl is null");
		return;
	}

	imageControl->setId(controlId);
	imageControl->addListener(this, Control::Listener::CLICK);

/*
	float imageScale = 1.f;

	if(imageWidth > imageControl->getWidth() || imageHeight > imageControl->getHeight()) {
		if (imageWidth > imageHeight) {
			imageScale = imageControl->getWidth() / imageWidth;
		} else {
			imageScale = imageControl->getHeight() / imageHeight;
		}
	}

	imageControl->setSize(imageWidth * imageScale, imageHeight * imageScale);
*/

	isPlaying = true;

	setDrawable(form);

	videoPlayer->open();
	videoPlayer->pause();
	this->videoPlayer = videoPlayer;
}

void VideoLayoutFormNode::controlEvent(Control *control, Control::Listener::EventType evt) {
	LOGD("VideoLayoutFormNode. controlEvent");
	if (evt == CLICK) {
		LOGD("VideoLayoutFormNode click. id : %s", control->getId());
		MKitHelper::getInstance()->onClick(control->getId());

		// toggle play / stop
		isPlaying = !isPlaying;
	}
}

void VideoLayoutFormNode::drawForm(bool wireFrame) {
	if (isEnabled()) {
		form->draw(wireFrame);
	}
}

void VideoLayoutFormNode::stopPlaying() {
	videoPlayer->pause();
}

void VideoLayoutFormNode::update() {
	int state = videoPlayer->getState();
	if (state == VideoPlayer::VideoState::STATE_ERROR || !isEnabled() || !isPlaying) {
		return;
	}

	if (state == VideoPlayer::VideoState::STATE_READY || state == VideoPlayer::VideoState::STATE_PAUSE) {
		LOGD("Video. video start : %s", getId());
		videoPlayer->start();
	}

	//material->getStateBlock()->setCullFace(false);
	videoPlayer->update(imageControl);

}

void VideoLayoutFormNode::setEnabled(bool enabled)
{
	//LOGD("Video enabled : %d", enabled);
	Node::setEnabled(enabled);
	if (isPlaying) {
		int state = videoPlayer->getState();
		if (state == VideoPlayer::VideoState::STATE_READY || state == VideoPlayer::VideoState::STATE_PAUSE) {
			videoPlayer->start();
		}
	} else {
		videoPlayer->pause();
	}
}