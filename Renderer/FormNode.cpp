#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG "FormNode"
#endif

#include <iostream>
#include <cstdarg>
#include <cmath>

#include <LogUtils.h>
#include <FormNode.h>

#define INCREASE_OFFSET 0.25F

using namespace gameplay;
FormNode::FormNode(const char * layoutName) : Node(layoutName) {

	char strBuf[256];
	memset(strBuf, 0x00, sizeof(char));
	sprintf(strBuf, "res/common/forms/%s.form", layoutName);
	form = Form::create(strBuf);
	LOGD("form name : %s", strBuf);
}

FormNode::FormNode(const char* layoutName, const char* id) : Node(id) {

	char strBuf[256];
	memset(strBuf, 0x00, sizeof(char));
	sprintf(strBuf, "res/common/forms/%s.form", layoutName);
	form = Form::create(strBuf);
	LOGD("form name : %s", strBuf);
}

FormNode::~FormNode() {
	if (form != nullptr) {
		SAFE_RELEASE(form);
	}
}

gameplay::Rectangle FormNode::getFormBounds() {
	return form->getBounds();
}

void FormNode::applyCheckState(const char * controlId, bool isChecked) {
	auto checkBox = dynamic_cast<CheckBox *>(form->getControl(controlId));
	LOGD("applyCheckState");
	if (checkBox != nullptr) {
		LOGD("CheckBox id : %s", checkBox->getId());
		checkBox->setChecked(isChecked);
	}
}

void FormNode::onDraw(float eclipseTime) {
	return;
}


void FormNode::applyAlpha(float alpha) {
	form->setOpacity(alpha);
	for (Control* control : form->getControls()) {
		control->setOpacity(alpha);
	}
}

void FormNode::applyAlphaImproveContrast(float alpha, Control* _control) {
	float a = std::min(1.0F,  alpha + INCREASE_OFFSET);
	if (_control) _control->setOpacity(a / alpha);
}

void FormNode::applyAlphaImproveContrast(float alpha, int args, ...) {
    float a = std::min(1.0F,  alpha + INCREASE_OFFSET);
	va_list ap;
	va_start(ap, args);

	for (int i = 0; i < args; ++i) {
		Control *_control = va_arg(ap, Control*);
		if (_control) _control->setOpacity(a / alpha);
	}

	va_end(ap);
}

void FormNode::applyAlphaImproveContrast(float alpha, std::vector<Control*> &controls) {
    float a = std::min(1.0F,  alpha + INCREASE_OFFSET);
    for(Control *_control : controls) {
        if (_control) _control->setOpacity(a / alpha);
    }
}

/*
void FormNode::drawForm(bool wireFrame) {
	if (form != nullptr) {
		form->draw(false);
	}
}
*/