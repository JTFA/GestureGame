#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG "FormNode"
#endif

#include "Label.h"
#include <LogUtils.h>
#include <Utilities.h>
#include <utility/FormUtility.h>
#include <model/ARComplexViewer.h>
#include <node/TemplateFormNode.h>

static const char * eventIds[]=  {
		"bg1", "bg2", "bg3", "bg4"
};

static const char * imageIds[]=  {
        "image1", "image2", "image3", "image4", "imagePlay", "imagePause", "imagePrev", "imageNext"
};

static const char * labelIds[]=  {
		"label1", "label2", "label3", "label4"
};

static const char * textLabelIds[]=  {
       "titleLabel"
};


using namespace armakers;

TemplateFormNode::~TemplateFormNode() {
	LOGD("TemplateFormNode destroy");
}

TemplateFormNode::TemplateFormNode(std::string &controlId, const char *formName,
		std::shared_ptr<ARComplexViewer> viewer)
: FormNode(formName) {
    this->controlId = controlId + "::ITEM";
    this->videoControlId = controlId + "::VIDEO";
    this->pageControlId = controlId + "::PAGE";
    this->viewer = viewer;
	for (const char * imageId : imageIds) {
		if (gameplay::Control *control = form->getControl(imageId)) {
            //LOGD("GlobalMenuLayoutFormNode setClick. id : %s", control->getId());
			setOnClickListener(control, gameplay::Control::Listener::CLICK, imageId);
		}
	}

	labelBGs.clear();
	for (const char * eventId : eventIds) {
		if (gameplay::Control *control = form->getControl(eventId)) {
			//LOGD("GlobalMenuLayoutFormNode setClick. id : %s", control->getId());
			setOnClickListener(control, gameplay::Control::Listener::CLICK, eventId);
            labelBGs.push_back(control);
		}
	}

	controls.clear();
	for (const char * labelId : labelIds) {
		if (gameplay::Control *control = form->getControl(labelId)) {
			//LOGD("GlobalMenuLayoutFormNode setClick. id : %s", control->getId());
			setOnClickListener(control, gameplay::Control::Listener::CLICK, labelId);
            controls.push_back(control);

		}
	}

    texts.clear();
    for (const char * textLabelId : textLabelIds) {
        if (gameplay::Control *control = form->getControl(textLabelId)) {
            texts.push_back(control);
        }
    }

    applyAlpha(viewer->getAlpha());
    applyAlphaImproveContrast(viewer->getAlpha(), controls);
    applyAlphaImproveContrast(viewer->getAlpha(), texts);
    updateTitleText();
    updateLabelText();
	setDrawable(form);
}

void TemplateFormNode::controlEvent(gameplay::Control *control, gameplay::Control::Listener::EventType evt) {
	if (evt == CLICK) {
		//LOGD("TemplateFormNode click. id : %s", control->getId());
		if (!strcmp(control->getId(), imageIds[0]) || !strcmp(control->getId(), eventIds[0]) || !strcmp(control->getId(), labelIds[0])) {
			MKitHelper::getInstance()->onClick(controlId.c_str(), 0);
        } else if (!strcmp(control->getId(), imageIds[1]) || !strcmp(control->getId(), eventIds[1]) || !strcmp(control->getId(), labelIds[1])) {
            MKitHelper::getInstance()->onClick(controlId.c_str(), 1);
		} else if (!strcmp(control->getId(), imageIds[2]) || !strcmp(control->getId(), eventIds[2]) || !strcmp(control->getId(), labelIds[2])) {
            MKitHelper::getInstance()->onClick(controlId.c_str(), 2);
		} else if (!strcmp(control->getId(), imageIds[3]) || !strcmp(control->getId(), eventIds[3]) || !strcmp(control->getId(), labelIds[3])) {
            MKitHelper::getInstance()->onClick(controlId.c_str(), 3);
		} else if(!strcmp(control->getId(), imageIds[4])) { //imagePlay
            MKitHelper::getInstance()->onClick(videoControlId.c_str(), ARContentMediaPlayerOP::MP_START_OP);
		} else if(!strcmp(control->getId(), imageIds[5])) { //imagePause
            MKitHelper::getInstance()->onClick(videoControlId.c_str(), ARContentMediaPlayerOP::MP_PAUSE_OP);
        } else if(!strcmp(control->getId(), imageIds[6])) { //imagePrev
            MKitHelper::getInstance()->onClick(pageControlId.c_str(), ARContentPageOP::PAGE_PREV_OP);
        } else if(!strcmp(control->getId(), imageIds[7])) { //imageNext
            MKitHelper::getInstance()->onClick(pageControlId.c_str(), ARContentPageOP::PAGE_NEXT_OP);
        }
	}
}

void TemplateFormNode::measureContentsTransform(gameplay::Vector3 &pos, gameplay::Vector3 &scale) {
	if(gameplay::Control* control = form->getControl("contentsContainer")) {
		gameplay::Rectangle rect = control->getBounds();
        gameplay::Rectangle formRect = getFormBounds();

		scale.x = rect.width * FORM_SCALE;
		scale.y = rect.height * FORM_SCALE;
		scale.z = 0;

        pos.x = -(formRect.width * 0.5F - (rect.x + (rect.width * 0.5F))) * FORM_SCALE;
        pos.y = -(formRect.height * 0.5F - (rect.y + (rect.height * 0.5F))) * FORM_SCALE;
        pos.z = -FORM_SCALE;

        //LOGD("TemplateFormNode measureContentsTransform : scale(%f/%f/%f)", scale.x, scale.y, scale.z);
        //LOGD("TemplateFormNode measureContentsTransform : pos(%f/%f/%f)", pos.x, pos.y, pos.z);
	}
}

void TemplateFormNode::drawForm(bool wireFrame) {
	if (isEnabled()) {
		form->draw(wireFrame);
	}
}

void TemplateFormNode::applyAlpha(float alpha) {
    for (auto *control : form->getControls()) {
        control->setOpacity(alpha);
    }

    for (const char * eventId : eventIds) {
        if (gameplay::Control *control = form->getControl(eventId)) {
            //LOGD("TemplateFormNode applyAlpha. id : %s", control->getId());
            control->setOpacity(alpha);
        }
    }
}

void TemplateFormNode::onDraw(float eclipseTime) {

}

void TemplateFormNode::updateControlVisible() {
    std::shared_ptr<ARContentList> spContentList = std::static_pointer_cast<ARContentList>(
            viewer->getItems()[ARContextMenuType::DATA_VIEWER_MENU]);
    auto content = spContentList->getCurrentItem();
    if (gameplay::Control *control = form->getControl(imageIds[4])) {
        control->setVisible(content->getType() == ARContentItemType::VIDEO_DATA);
    }
    if (gameplay::Control *control = form->getControl(imageIds[5])) {
        control->setVisible(content->getType() == ARContentItemType::VIDEO_DATA);
    }
    if (gameplay::Control *control = form->getControl(imageIds[6])) {
        control->setVisible(content->getLocalPathList().size() > 1);
    }
    if (gameplay::Control *control = form->getControl(imageIds[7])) {
        control->setVisible(content->getLocalPathList().size() > 1);
    }
}

//private
void TemplateFormNode::updateTitleText() {
	auto *label = dynamic_cast<gameplay::Label*>(texts[0]);
	if (label) label->setText(viewer->facilityName().c_str());
}

void TemplateFormNode::updateLabelText() {
    std::shared_ptr<ARContentList> spContentList = std::static_pointer_cast<ARContentList>(
            viewer->getItems()[ARContextMenuType::DATA_VIEWER_MENU]);
    auto items = spContentList->getItems();
    for (int32_t i = 0; i < controls.size(); ++i) {
        auto *label = dynamic_cast<gameplay::Label*>(controls[i]);
        std::string text = i < items.size() ? items[i]->getMenuTitle() : std::string("");

        if (text.length() == 0) {
            label->setVisible(false);
            if (labelBGs.size() > i) {
                labelBGs[i]->setVisible(false);
            }
        } else {
            label->setVisible(true);
            if (labelBGs.size() > i) {
                labelBGs[i]->setVisible(true);
            }
            label->setText(text.c_str());
        }

    }
}

void TemplateFormNode::setOnClickListener(gameplay::Control *control, int eventFlags, const char* eventId) {
	control->setId(eventId);
	control->addListener(this, eventFlags);
}