#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG "FormNode"
#endif

#include <LogUtils.h>
#include "FormNode.h"
#include <Layout1FormNode.h>

using namespace gameplay;

Layout1FormNode::Layout1FormNode(char *layoutName, char *imageTextPath, char *checkControlId, char *buttonControlId) :
		FormNode(layoutName) {

	auto checkBox = dynamic_cast<CheckBox *>(form->getControl("childCheckBox"));
	auto imageControl = dynamic_cast<ImageControl *>(form->getControl("image"));
	auto button = dynamic_cast<Button *>(form->getControl("checkButton"));

	if (checkBox == nullptr) {
		LOGD("checkBox is null");
		return;
	}

	if (imageControl == nullptr) {
		LOGD("imageControl is null");
		return;
	}

	if (button == nullptr) {
		LOGD("button is null");
		return;
	}

	checkBox->setId(checkControlId);
	checkBox->addListener(this, Control::Listener::VALUE_CHANGED);

	imageControl->setImage(imageTextPath);

	button->setId(buttonControlId);
	button->addListener(this, Control::Listener::CLICK);

	setDrawable(form);
	setEnabled(true);
}

void Layout1FormNode::controlEvent(Control *control, EventType evt) {
	LOGD("controlEvent. Control id : %s", control->getId());
	if (evt == CLICK) {
		MKitHelper::getInstance()->onClick(control->getId());
	} else if (evt == VALUE_CHANGED) {
		auto checkBox = (CheckBox *) control;
		MKitHelper::getInstance()->onCheckedChanged(control->getId(), checkBox->isChecked());
	}
}

void Layout1FormNode::drawForm(bool wireFrame) {
	if (isEnabled()) {
		form->draw(wireFrame);
	}
}

Layout1FormNode::~Layout1FormNode() = default;