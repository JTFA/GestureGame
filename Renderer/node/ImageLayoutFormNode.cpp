#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG "FormNode"
#endif

#include <LogUtils.h>
#include <node/ImageLayoutFormNode.h>

using namespace armakers;

ImageLayoutFormNode::~ImageLayoutFormNode() = default;

ImageLayoutFormNode::ImageLayoutFormNode(char *layoutName, char *imagePath, int imageWidth,
										 int imageHeight, char *controlId)
										 : gameplay::FormNode(layoutName, controlId) {

//	auto imageControl = dynamic_cast<ImageControl *>(form->getControl("image"));
	imageControl = dynamic_cast<gameplay::ImageControl *>(form->getControl("image"));

	auto containerControl = form->getControl("cover");

	if (imageControl == nullptr) {
		LOGD("imageTitleControl is null");
		return;
	}

	imageControl->setId(controlId);
	imageControl->addListener(this, gameplay::Control::Listener::CLICK);

	float imageScale = 1.f;

	if(imageWidth > imageControl->getWidth() || imageHeight > imageControl->getHeight()) {
		if (imageWidth > imageHeight) {
			imageScale = imageControl->getWidth() / imageWidth;
		} else {
			imageScale = imageControl->getHeight() / imageHeight;
		}
	}

	imageControl->setSize(imageWidth * imageScale, imageHeight * imageScale);

	LOGD("ImageLayoutFormNode construct imagePath : %s", imagePath);

	imageControl->setImage(imagePath);

	setDrawable(form);
}

ImageLayoutFormNode::ImageLayoutFormNode(char *layoutName, char *controlId, char *imagePath, int imageWidth,
										 int imageHeight, bool sizeFit, float alpha)
										 : gameplay::FormNode(layoutName, controlId) {

//	auto imageControl = dynamic_cast<ImageControl *>(form->getControl("image"));
	imageControl = dynamic_cast<gameplay::ImageControl *>(form->getControl("image"));

	auto containerControl = form->getControl("cover");

	if (imageControl == nullptr) {
		LOGD("imageTitleControl is null");
		return;
	}

	imageControl->setId(controlId);
	imageControl->addListener(this, gameplay::Control::Listener::CLICK);

	float imageScale = 1.f;

	if(imageWidth > imageControl->getWidth() || imageHeight > imageControl->getHeight()) {
		if (imageWidth > imageHeight) {
			imageScale = imageControl->getWidth() / imageWidth;
		} else {
			imageScale = imageControl->getHeight() / imageHeight;
		}
	}

	imageControl->setSize(imageWidth * imageScale, imageHeight * imageScale);
	if (imageScale != 1.f) {
        form->setSize(imageWidth * imageScale, imageHeight * imageScale);
	} else {
        float width = form->getWidth();
        form->setSize(width, imageHeight * imageScale);
	}

	LOGD("ImageLayoutFormNode size : %f, %f", imageControl->getWidth(), imageControl->getHeight());

	LOGD("ImageLayoutFormNode construct imagePath : %s", imagePath);

	imageControl->setImage(imagePath);

	applyAlpha(alpha);
	setDrawable(form);
}

void ImageLayoutFormNode::setImagePath(const char * imagePath)
{
    if (imageControl != nullptr)
    {
        imageControl->setImage(imagePath);
    }
}

void ImageLayoutFormNode::controlEvent(gameplay::Control *control, gameplay::Control::Listener::EventType evt) {
	LOGD("ImageLayoutFormNode. controlEvent");
	if (evt == CLICK) {
		LOGD("Text click. id : %s", control->getId());
		MKitHelper::getInstance()->onClick(control->getId());
	}
}

void ImageLayoutFormNode::drawForm(bool wireFrame) {
	if (isEnabled()) {
        //LOGD("ImageLayoutFormNode. drawForm");
		form->draw(wireFrame);
	}
}