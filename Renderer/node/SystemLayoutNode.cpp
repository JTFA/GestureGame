//
// Created by kt-hyunjun on 2019-12-03.
//

#include "Node.h"

#include <node/SystemLayoutNode.h>
#include <node/GlobalMenuLayoutFormNode.h>
#include <LogUtils.h>
#include <utility/FormUtility.h>
#include <utility/NodeUtility.h>
using namespace armakers;

SystemLayoutNode::SystemLayoutNode(const std::string &nodeId, std::shared_ptr<armakers::SystemLayout> layout,
                                   float alpha)
: gameplay::Node(nodeId.c_str()) , formNode(nullptr) {
    formNode = new armakers::SystemLayoutFormNode(layout, alpha, "systemLayout2");
    addChild(formNode);
    FormUtility::translate2Center(formNode);
    LOGD("SystemLayoutNode create");
}

SystemLayoutNode::SystemLayoutNode(const std::string &nodeId, std::shared_ptr<armakers::SystemLayout> layout,
                                   int w, int h, float alpha)
: gameplay::Node(nodeId.c_str()) , formNode(nullptr) {
    formNode = new armakers::SystemLayoutFormNode(layout, alpha, w, h, "systemLayout2");
    addChild(formNode);
    FormUtility::translate2Center(formNode);
    LOGD("SystemLayoutNode create");
}

SystemLayoutNode::~SystemLayoutNode() {
    LOGD("SystemLayoutNode Destructor");
    SAFE_RELEASE(formNode);
}