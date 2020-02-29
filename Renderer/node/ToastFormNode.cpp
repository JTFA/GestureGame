//
// Created by kt-hyunjun on 2019-12-03.
//

#include <LogUtils.h>
#include <utility/FormUtility.h>
#include <node/ToastFormNode.h>
#include <control/ToastState.h>

#define TOAST_NODE_NAME "toast_node"
#define TOAST_FORM_NAME "ToastLayout"

#define WIDTH_PADDING 40

extern "C" {

int32_t __formWidth = 0;

void onUpdateTextSize(void* _this, int w, int h) {
    auto *label = (gameplay::Label*)_this;
    if (auto *form = label->getTopLevelForm()) {
        if (auto *formNode = dynamic_cast<gameplay::FormNode*>(form->getNode())) {
            form->setWidth(std::min(w + WIDTH_PADDING, __formWidth), false);
            FormUtility::move2Center(formNode);
        }
    }
}

}; //extern "C"

using namespace armakers;

ToastDelegate::ToastDelegate() {

}

ToastDelegate::~ToastDelegate() {

}

///////////////////////////////////////////////////////////////////////////////////////////////////
ToastFormNode::ToastFormNode(ToastDelegate *wp, float alpha) : gameplay::FormNode(TOAST_FORM_NAME, TOAST_NODE_NAME)
, ExtensionRenderable(), alpha(alpha), wpToastDelegate(wp) {
    spToastState = ToastState::initialize(this);
    _control = form->getControl("msgLabel");
    if (auto *label = dynamic_cast<gameplay::Label*>(_control)) {
        label->onUpdateTextSize = onUpdateTextSize;
    }
    __formWidth = (int32_t)form->getWidth();
    applyAlpha(alpha);
    setDrawable(form);
}

ToastFormNode::~ToastFormNode() {
    //LOGD("ToastFormNode Destructor");
}

void ToastFormNode::controlEvent(gameplay::Control *control, gameplay::Control::Listener::EventType evt) {

}

void ToastFormNode::applyAlpha(float alpha) {
    gameplay::FormNode::applyAlpha(alpha);
    gameplay::FormNode::applyAlphaImproveContrast(alpha, 1, _control);
}

void ToastFormNode::extensionUpdate(int32_t elapsedTime) {
    spToastState->process(elapsedTime);
}

void ToastFormNode::applyText(std::shared_ptr<ToastMessage> sp) {
    if (auto *label = dynamic_cast<gameplay::Label*>(form->getControl("msgLabel"))) {
        label->setText(sp->msg().c_str());
    }
}

void ToastFormNode::onPushToastMessage() {
    switch (spToastState->type()) {
        case ToastStateType::DEACTIVATE_TYPE:
        case ToastStateType::WAIT_TYPE:
            spToastState = ToastState::get(ToastStateType::ACTIVATE_TYPE);
            break;
    }
}

