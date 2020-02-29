#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG "FormNode"
#endif

#include "Label.h"
#include <LogUtils.h>

#include <node/GlobalMenuLayoutFormNode.h>

static const char * eventIds[]=  {
		"bg1", "bg2", "bg3", "bg4"
};

static const char * imageIds[]=  {
        "image1", "image2", "image3", "image4"
};

static const char * labelIds[]=  {
		"label1", "label2", "label3", "label4"
};

const char * badgeLabelId = "badge_label";
const char * badgeBackId = "badge_back";
const char * badgeContainerId = "badge_container";

using namespace armakers;

GlobalMenuLayoutFormNode::~GlobalMenuLayoutFormNode() {

}

GlobalMenuLayoutFormNode::GlobalMenuLayoutFormNode(char *layoutName, char * controlId, float alpha)
: gameplay::FormNode(layoutName, controlId) {
    this->controlId = controlId;
    this->alpha = alpha;
	for (const char * imageId : imageIds) {
		if (gameplay::Control *control = form->getControl(imageId)) {
            //LOGD("GlobalMenuLayoutFormNode setClick. id : %s", control->getId());
			setOnClickListener(control, gameplay::Control::Listener::CLICK, imageId);
		}
	}
	for (const char * eventId : eventIds) {
		if (gameplay::Control *control = form->getControl(eventId)) {
			//LOGD("GlobalMenuLayoutFormNode setClick. id : %s", control->getId());
			setOnClickListener(control, gameplay::Control::Listener::CLICK, eventId);
		}
	}
	for (const char * labelId : labelIds) {
		if (gameplay::Control *control = form->getControl(labelId)) {
			//LOGD("GlobalMenuLayoutFormNode setClick. id : %s", control->getId());
			setOnClickListener(control, gameplay::Control::Listener::CLICK, labelId);
		}
	}

    applyAlpha(alpha);
	setDrawable(form);

	setVisibleBadge(false);
}

void GlobalMenuLayoutFormNode::controlEvent(gameplay::Control *control, gameplay::Control::Listener::EventType evt) {
	if (evt == CLICK) {
		//LOGD("GlobalMenuLayoutFormNode click. id : %s", control->getId());
		if (!strcmp(control->getId(), imageIds[0]) || !strcmp(control->getId(), eventIds[0]) || !strcmp(control->getId(), labelIds[0])) {
			MKitHelper::getInstance()->onClick(controlId.c_str(), 0);
		} else if (!strcmp(control->getId(), imageIds[1]) || !strcmp(control->getId(), eventIds[1]) || !strcmp(control->getId(), labelIds[1])) {
			MKitHelper::getInstance()->onClick(controlId.c_str(), 1);
		} else if (!strcmp(control->getId(), imageIds[2]) || !strcmp(control->getId(), eventIds[2]) || !strcmp(control->getId(), labelIds[2])) {
			MKitHelper::getInstance()->onClick(controlId.c_str(), 2);
		} else if (!strcmp(control->getId(), imageIds[3]) || !strcmp(control->getId(), eventIds[3]) || !strcmp(control->getId(), labelIds[3])) {
			MKitHelper::getInstance()->onClick(controlId.c_str(), 3);
		}
	}
}

void GlobalMenuLayoutFormNode::drawForm(bool wireFrame) {
	if (isEnabled()) {
		form->draw(wireFrame);
	}
}

void GlobalMenuLayoutFormNode::onDraw(float eclipseTime) {

}

//private
void GlobalMenuLayoutFormNode::setOnClickListener(gameplay::Control *control, int eventFlags, const char* eventId) {
	control->setId(eventId);
	control->addListener(this, eventFlags);
}

void GlobalMenuLayoutFormNode::checkTimeout(float eclipseTime) {

}

void GlobalMenuLayoutFormNode::applyAlpha(float alpha) {
	for (gameplay::Control* control : form->getControls()) {
		if (!strcmp(control->getId(), "text1") || !strcmp(control->getId(), "text2")) {
			gameplay::FormNode::applyAlphaImproveContrast(alpha, 1, control);
		} else {
			control->setOpacity(alpha);
		}
	}

    gameplay::Control* labelCtl = form->getControl(badgeLabelId);
	if (labelCtl != nullptr) {
		gameplay::FormNode::applyAlphaImproveContrast(alpha, 1, labelCtl);
	}
}

void GlobalMenuLayoutFormNode::setBadgeText(char *text) {
	setVisibleBadge(true);

    gameplay::Label *label = dynamic_cast<gameplay::Label*>(form->getControl(badgeLabelId));
	if (label != nullptr) {
		label->setText(text);
	}
}

void GlobalMenuLayoutFormNode::setVisibleBadge(bool visible) {
    gameplay::Control *control = form->getControl(badgeContainerId);
	if (control != nullptr) {
		control->setVisible(visible);
	}
}