#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG "FormNode"
#endif

#include <LogUtils.h>
#include <FormNode.h>
#include <TagLayoutFormNode.h>

using namespace gameplay;

TagLayoutFormNode::TagLayoutFormNode(char *layoutName, char *imageTextPath, char *buttonControlId) :
		FormNode(layoutName) {

	auto imageControl = dynamic_cast<ImageControl *>(form->getControl("image"));
	auto button = dynamic_cast<Button *>(form->getControl("tagButton"));

	if (imageControl == nullptr) {
		LOGD("imageControl is null");
		return;
	}

	if (button == nullptr) {
		LOGD("button is null");
		return;
	}

	imageControl->setImage(imageTextPath);

	button->setId(buttonControlId);
	button->addListener(this, Control::Listener::CLICK);
	setDrawable(form);
	setEnabled(true);
}

void TagLayoutFormNode::controlEvent(Control *control, EventType evt) {
	LOGD("controlEvent. Control id : %s", control->getId());
	if (evt == CLICK) {
		MKitHelper::getInstance()->onClick(control->getId());
	}
}

void TagLayoutFormNode::drawForm(bool wireFrame) {
	if (isEnabled()) {
		form->draw(wireFrame);
	}
}

TagLayoutFormNode::~TagLayoutFormNode() {

}