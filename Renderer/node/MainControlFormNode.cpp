//
// Created by kt-hyunjun on 2019-12-03.
//

#include <LogUtils.h>
#include <utility/FormUtility.h>
#include <node/MainControlFormNode.h>

using namespace armakers;

MainControlFormNode::MainControlFormNode(float alpha) : gameplay::FormNode(MAIN_CONTROL_FORM_NAME, MAIN_CONTROL_NODE_NAME)
, _alpha(alpha) {
    applyAlpha(_alpha);
    setDrawable(form);
}

MainControlFormNode::~MainControlFormNode() {
    //LOGD("MainControlFormNode Destructor");
}

void MainControlFormNode::controlEvent(gameplay::Control *control, gameplay::Control::Listener::EventType evt) {

}

void MainControlFormNode::applyAlpha(float alpha) {
    gameplay::FormNode::applyAlpha(alpha);
    //gameplay::FormNode::applyAlphaImproveContrast(alpha, 1, _control);
}
