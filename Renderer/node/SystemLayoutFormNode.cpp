//
// Created by kt-hyunjun on 2019-10-22.
//


#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG "FormNode"
#endif


#include <LogUtils.h>
#include <node/SystemLayoutFormNode.h>

using namespace armakers;

SystemLayoutFormNode::~SystemLayoutFormNode() {
    LOGD("SystemLayoutFormNode destroy");
}

SystemLayoutFormNode::SystemLayoutFormNode(std::shared_ptr<SystemLayout> layout, float alpha,
                                           const char *formName)
        : FormNode(formName), ExtensionRenderable() {
    imageControl = dynamic_cast<gameplay::ImageControl *>(form->getControl("image"));
    if (imageControl == nullptr) {
        LOGD("imageTitleControl is null");
        return;
    }

    //form->addListener(this, Control::Listener::CLICK | Control::Listener::PRESS | Control::Listener::RELEASE);
    //imageControl->setId("systemLayout");
    imageControl->addListener(this, gameplay::Control::Listener::PRESS | gameplay::Control::Listener::RELEASE | gameplay::Control::Listener::MOVE);
    applyAlpha(alpha);
    setDrawable(form);
    this->layout = layout;
}
SystemLayoutFormNode::SystemLayoutFormNode(std::shared_ptr<SystemLayout> layout, float alpha,
                                           int w, int h, const char *formName)
: FormNode(formName), ExtensionRenderable() {
    imageControl = dynamic_cast<gameplay::ImageControl *>(form->getControl("image"));
    if (imageControl == nullptr) {
        LOGD("imageTitleControl is null");
        return;
    }
    form->setSize(w, h);
    imageControl->setSize(w, h);
    auto containerControl = form->getControl("cover");
    if (containerControl) {
        containerControl->setSize(w, h);
    }
    //form->addListener(this, Control::Listener::CLICK | Control::Listener::PRESS | Control::Listener::RELEASE);
    //imageControl->setId("systemLayout");
    imageControl->addListener(this, gameplay::Control::Listener::PRESS | gameplay::Control::Listener::RELEASE | gameplay::Control::Listener::MOVE);
    applyAlpha(alpha);
    setDrawable(form);
    this->layout = layout;

}

void SystemLayoutFormNode::controlEvent(gameplay::Control *control, gameplay::Control::Listener::EventType evt) {

}

void SystemLayoutFormNode::controlPointerTrigger(gameplay::Control *control, gameplay::Control::Listener::EventType evt,
                                                 unsigned int contactIndex, int x, int y) {
    //LOGD("SystemLayoutFormNode. controlPointerTrigger evt(0x%x)", evt);
    switch (evt) {
        case PRESS:
            layout->onPressEvent(contactIndex, x, y);
            break;
        case RELEASE:
            layout->onReleaseEvent(contactIndex, x, y);
            break;
        case MOVE:
            layout->onMoveEvent(contactIndex, x, y);
            break;
        default:
            break;
    }
}

void SystemLayoutFormNode::drawForm(bool wireFrame) {
    if (isEnabled()) {
        form->draw(wireFrame);
    }
}

void SystemLayoutFormNode::extensionUpdate(int32_t elapsedTime) {
    //LOGD("SystemLayoutFormNode. extensionUpdate");
    if (layout->isDrawable(imageControl, form)) {
        layout->update();
    }
}

void SystemLayoutFormNode::setEnabled(bool enabled) {
    Node::setEnabled(enabled);
}

const char* SystemLayoutFormNode::getName() {
    return layout ? layout->getName() : nullptr;
}
