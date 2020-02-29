#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG "FormNode"
#endif


#include <LogUtils.h>
#include <FormNode.h>
#include <IconLayoutFormNode.h>

using namespace gameplay;

IconLayoutFormNode::~IconLayoutFormNode() {

}

IconLayoutFormNode::IconLayoutFormNode(char *layoutName, char *imagePath) : FormNode(layoutName) {
	auto imageControl = dynamic_cast<ImageControl *>(form->getControl("image"));

	if (imageControl == nullptr) {
		LOGD("imageTitleControl is null");
		return;
	}

	imageControl->setImage(imagePath);
	setDrawable(form);
	setEnabled(true);
}

void IconLayoutFormNode::controlEvent(Control *control, Control::Listener::EventType evt) {

}

void IconLayoutFormNode::drawForm(bool wireFrame) {
	if (isEnabled()) {
		form->draw(wireFrame);
	}
}