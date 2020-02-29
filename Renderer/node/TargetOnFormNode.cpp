//
// Created by kt-hyunjun on 2019-12-03.
//

#include <LogUtils.h>
#include <utility/FormUtility.h>
#include <node/TargetOnFormNode.h>

#define TARGET_ON_NODE_NAME "target_on"
#define TARGET_ON_FORM_NAME "TargetOn"

using namespace armakers;

TargetOnFormNode::TargetOnFormNode(float alpha) : gameplay::FormNode(TARGET_ON_FORM_NAME, TARGET_ON_NODE_NAME)
, alpha(alpha) {
    //applyAlpha(alpha);
    setDrawable(form);
}

TargetOnFormNode::~TargetOnFormNode() {
    //LOGD("ToastFormNode Destructor");
}

void TargetOnFormNode::controlEvent(gameplay::Control *control, gameplay::Control::Listener::EventType evt) {

}

void TargetOnFormNode::applyAlpha(float alpha) {
    gameplay::FormNode::applyAlpha(alpha);
}

/////////////////////////////////////////////////////////////
TargetOnNode::TargetOnNode(const std::string &nodeId) : gameplay::Node(nodeId.c_str()) {
    pTargetOnFormNode = new TargetOnFormNode();
    addChild(pTargetOnFormNode);
    FormUtility::translate2Center(pTargetOnFormNode);
}

TargetOnNode::~TargetOnNode() {
    //LOGD("TargetOnNode Destructor");
    SAFE_RELEASE(pTargetOnFormNode);
}

void TargetOnNode::processEnabled(bool enable) {
    //LOGD("TargetOnNode processEnabled (%d)", enable);
    setEnabled(enable);
    pTargetOnFormNode->setEnabled(enable);
}