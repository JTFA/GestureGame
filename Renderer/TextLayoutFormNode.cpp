#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG "FormNode"
#endif

#include <LogUtils.h>
#include <TextLayoutFormNode.h>

using namespace gameplay;

TextLayoutFormNode::~TextLayoutFormNode() = default;

TextLayoutFormNode::TextLayoutFormNode(char *layoutName, char *imagePath, char * controlId) : FormNode(layoutName) {

	auto imageControl = dynamic_cast<ImageControl *>(form->getControl("image"));
	auto containerControl = form->getControl("cover");

	if (imageControl == nullptr) {
		LOGD("imageTitleControl is null");
		return;
	}

	if (containerControl == nullptr) {
		LOGD("containerControl is null");
		return;
	}

	containerControl->setId(controlId);
	containerControl->addListener(this, Control::Listener::CLICK);

	LOGD("imagePath : %s", imagePath);

	//imageControl->setSize(bitmapWidth * 0.3f, bitmapHeight * 1.6f);
	imageControl->setImage(imagePath);

	form->setConsumeInputEvents(false);
	imageControl->setConsumeInputEvents(false);
	setDrawable(form);
}

void TextLayoutFormNode::controlEvent(Control *control, Control::Listener::EventType evt) {
	LOGD("TextLayoutFormNode. controlEvent");
	if (evt == CLICK) {
		LOGD("Text click. id : %s", control->getId());
		MKitHelper::getInstance()->onClick(control->getId());
	}
}

void TextLayoutFormNode::drawForm(bool wireFrame) {
	if (isEnabled()) {
		form->draw(wireFrame);
	}
}