#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG "FormNode"
#endif

#include <LogUtils.h>
#include <node/ImagePagesFormNode.h>

//static const char * labelIds[]=  {
//		"label1", "label2"
//};
static const char * btnIds[]=  {
        "btn_up", "btn_down"
};
using namespace armakers;

ImagePagesFormNode::~ImagePagesFormNode() = default;
ImagePagesFormNode::ImagePagesFormNode(char *layoutName, char *controlId, std::vector<std::string> &lst, int imageWidth,
										 int imageHeight, bool sizeFit, float alpha)
		: gameplay::FormNode(layoutName, controlId) {

	this->setLocalPathList(lst);

	LOGD("ImagePagesFormNode. construct : localpathlist.size = %d", localPathList.size());

	this->controlId = controlId;
	imageControl = dynamic_cast<gameplay::ImageControl *>(form->getControl("page"));
	bottomControl= dynamic_cast<gameplay::ImageControl *>(form->getControl("bottom1"));

	auto containerControl = form->getControl("cover");

	if (imageControl == nullptr) {
		LOGD("ImagePagesFormNode is null");
		return;
	}

	imageControl->setId(controlId);
//	imageControl->addListener(this, gameplay::Control::Listener::CLICK);

	for (const char * btnId : btnIds) {
		if (gameplay::Control *control = form->getControl(btnId)) {
			//LOGD("GlobalMenuLayoutFormNode setClick. id : %s", control->getId());
			setOnClickListener(control, gameplay::Control::Listener::CLICK, btnId);
		}
	}

	float imageScale = 1.f;
	float fheight = 100.f;
	if (bottomControl != nullptr)
        fheight = bottomControl->getHeight();

	if(imageWidth > imageControl->getWidth() || imageHeight > imageControl->getHeight()) {
		if (imageWidth > imageHeight) {
			imageScale = imageControl->getWidth() / imageWidth;
		} else {
			imageScale = imageControl->getHeight() / imageHeight;
		}
	}

	imageControl->setSize(imageWidth * imageScale, imageHeight * imageScale);
	if (imageScale != 1.f) {
		form->setSize(imageWidth * imageScale, imageHeight * imageScale + fheight);
	} else {
		float width = form->getWidth();
		form->setSize(width, imageHeight * imageScale + fheight);
	}

	const char* imagePath = getCurrentImagePath();

	imageControl->setImage(imagePath);

	applyAlpha(alpha);
	setDrawable(form);
}

//opt : opt < 0 이면 UP 처리, opt > 0 이면 DOWN 처리
void ImagePagesFormNode::changeImagePage(int opt)
{
    LOGD("ImagePagesFormNode. changeImagePage : size = %d", localPathList.size());

	if ( localPathList.size() <= 0)
		return;

    if (opt < 0) {
        currentIndex -= 1;

        if ( currentIndex <= -1 )  currentIndex = localPathList.size() - 1;

        setImagePath(getCurrentImagePath());
    }
    else if ( opt > 0) {
        currentIndex += 1;
        if ( currentIndex >= localPathList.size())    currentIndex = 0;

        setImagePath(getCurrentImagePath());
    }
}

void ImagePagesFormNode::setImagePath(const char * imagePath)
{
	if (imageControl != nullptr)
	{
		imageControl->setImage(imagePath);
	}
}

void ImagePagesFormNode::setOnClickListener(gameplay::Control *control, int eventFlags, const char* eventId) {
	control->setId(eventId);
	control->addListener(this, eventFlags);
}

void ImagePagesFormNode::controlEvent(gameplay::Control *control, gameplay::Control::Listener::EventType evt) {
	LOGD("ImagePagesFormNode. controlEvent : %s, %s", control->getId(), controlId.c_str());

	if (evt == CLICK) {
		LOGD("Text click. id : %s", control->getId());
//		MKitHelper::getInstance()->onClick(control->getId());

		if (!strcmp(control->getId(), btnIds[0])) {
			MKitHelper::getInstance()->onClick(controlId.c_str(), 0);
		} else if (!strcmp(control->getId(), btnIds[1])) {
			MKitHelper::getInstance()->onClick(controlId.c_str(), 1);
		}
	}
}

void ImagePagesFormNode::drawForm(bool wireFrame) {
	if (isEnabled()) {
		form->draw(wireFrame);
	}
}