#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG "FormNode"
#endif

#include "Label.h"

#include <LogUtils.h>
#include <model/ARComplexViewer.h>
#include <node/ContextMenuFormNode.h>

static const char * eventIds[]=  {
		"bg1", "bg2", "bg3", "bg4", "bg5"
};

static const char * imageIds[]=  {
        "image1", "image2", "image3", "image4", "image5"
};

static const char * labelIds[]=  {
		"label1", "label2", "label3", "label4", "label5"
};

static const char * disableIds[]=  {
		"disable1", "disable2", "disable3", "disable4"
};

static const char * badgeLabelId = "badge_label";
static const char * badgeBackId = "badge_back";
static const char * badgeContainerId = "badge_container";

using namespace armakers;

ContextMenuFormNode::~ContextMenuFormNode() {

}

ContextMenuFormNode::ContextMenuFormNode(std::string &controlId, const char *formName, float alpha)
: gameplay::FormNode(formName) {
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

	for (const char * disableId : disableIds) {
		if (gameplay::Control *control = form->getControl(disableId)) {
			menuDisable.push_back(control);
		}
	}

    applyAlpha(alpha);
	setDrawable(form);

	setVisibleBadge(false);
}

void ContextMenuFormNode::controlEvent(gameplay::Control *control, gameplay::Control::Listener::EventType evt) {
	if (evt == CLICK) {
		LOGD("ContextMenuFormNode click. id %s : %s", controlId.c_str(), control->getId());
		if (!strcmp(control->getId(), imageIds[0]) || !strcmp(control->getId(), eventIds[0]) || !strcmp(control->getId(), labelIds[0])) {
			MKitHelper::getInstance()->onClick(controlId.c_str(), ARContextMenuType::AR_CALL_MENU);
		} else if (!strcmp(control->getId(), imageIds[1]) || !strcmp(control->getId(), eventIds[1]) || !strcmp(control->getId(), labelIds[1])) {
            MKitHelper::getInstance()->onClick(controlId.c_str(), ARContextMenuType::DATA_VIEWER_MENU);
		} else if (!strcmp(control->getId(), imageIds[2]) || !strcmp(control->getId(), eventIds[2]) || !strcmp(control->getId(), labelIds[2])) {
            MKitHelper::getInstance()->onClick(controlId.c_str(), ARContextMenuType::FACILITY_WORK_MENU);
		} else if (!strcmp(control->getId(), imageIds[3]) || !strcmp(control->getId(), eventIds[3]) || !strcmp(control->getId(), labelIds[3])) {
            MKitHelper::getInstance()->onClick(controlId.c_str(), ARContextMenuType::FACILITY_MEMO_MENU);
		}  else if (!strcmp(control->getId(), imageIds[4]) || !strcmp(control->getId(), eventIds[4]) || !strcmp(control->getId(), labelIds[4])) {
            MKitHelper::getInstance()->onClick(controlId.c_str(), ARContextMenuType::CLOSE_MENU);
        }
	}
}

void ContextMenuFormNode::drawForm(bool wireFrame) {
	if (isEnabled()) {
		form->draw(wireFrame);
	}
}

//private
void ContextMenuFormNode::setOnClickListener(gameplay::Control *control, int eventFlags, const char* eventId) {
	control->setId(eventId);
	control->addListener(this, eventFlags);
}

void ContextMenuFormNode::applyAlpha(float alpha) {
	for (gameplay::Control* control : form->getControls()) {
		if (!strcmp(control->getId(), "text1") || !strcmp(control->getId(), "text2") || !strcmp(control->getId(), badgeContainerId)) {
			gameplay::FormNode::applyAlphaImproveContrast(alpha, 1, control);
		} else {
			control->setOpacity(alpha);
		}
	}

	gameplay::Control* backCtl = form->getControl(badgeBackId);
	if (backCtl != nullptr) {
		backCtl->setOpacity(alpha);
	}
}

void ContextMenuFormNode::setBadgeText(char *text) {
	setVisibleBadge(true);

	gameplay::Label *label = dynamic_cast<gameplay::Label*>(form->getControl(badgeLabelId));
	if (label != nullptr) {
		label->setText(text);
	}
}

void ContextMenuFormNode::setVisibleBadge(bool visible) {
	gameplay::Control *control = form->getControl(badgeContainerId);
	if (control != nullptr) {
		control->setVisible(visible);
	}
}

void ContextMenuFormNode::setEnableMenu(bool enable) {
	for (auto &it : menuDisable) {
		it->setVisible(!enable);
	}
}