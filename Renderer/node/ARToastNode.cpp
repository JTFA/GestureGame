//
// Created by kt-hyunjun on 2019-12-03.
//

#include "Node.h"

#include <LogUtils.h>
#include <utility/FormUtility.h>
#include <node/ARToastNode.h>

using namespace armakers;

ARToastNode::ARToastNode(const std::string &nodeId) : gameplay::Node(nodeId.c_str()) {
    pToastFormNode = new ToastFormNode(this);
    addChild(pToastFormNode);
    FormUtility::translate2Center(pToastFormNode);
    /*
    queue.push(std::shared_ptr<ToastMessage>(new ToastMessage("123", 3000, WAITING_DRAW_TYPE)));
    queue.push(std::shared_ptr<ToastMessage>(new ToastMessage("456789", 1000, WAITING_DRAW_TYPE)));
    queue.push(std::shared_ptr<ToastMessage>(new ToastMessage("654123456", 1000, WAITING_DRAW_TYPE)));
    queue.push(std::shared_ptr<ToastMessage>(new ToastMessage("456", 1000, WAITING_DRAW_TYPE)));
    queue.push(std::shared_ptr<ToastMessage>(new ToastMessage("789123111231231", 3000, WAITING_DRAW_TYPE)));
    */
}

ARToastNode::~ARToastNode() {
    //LOGD("ToastFormNode Destructor");
    SAFE_RELEASE(pToastFormNode);
}

void ARToastNode::process(std::shared_ptr<ToastMessage> sp) {
    auto isEvent = queue.empty();
    queue.push(sp);
    if (isEvent) pToastFormNode->onPushToastMessage();
}
