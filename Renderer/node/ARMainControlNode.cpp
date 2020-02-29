//
// Created by kt-hyunjun on 2019-12-03.
//

#include "Node.h"

#include <LogUtils.h>
#include <utility/FormUtility.h>
#include <utility/NodeUtility.h>
#include <node/ARMainControlNode.h>

using namespace armakers;

ARMainControlNode::ARMainControlNode(const std::string &nodeId, gameplay::Node *cameraNode) : gameplay::Node(nodeId.c_str())
, ExtensionRenderable(), _wpCameraNode(cameraNode), _formNode(nullptr) {
//    _formNode = new MainControlFormNode();
//    addChild(_formNode);
//    FormUtility::translate2Center(_formNode);
}

ARMainControlNode::~ARMainControlNode() {
    //LOGD("ARMainControlNode Destructor");
    SAFE_RELEASE(_formNode);
}

void ARMainControlNode::setFormNode(gameplay::FormNode *node) {
    removeFormNode();

    _formNode = node;
    FormUtility::translate2Center(_formNode, false);
    addChild(_formNode);
}

void ARMainControlNode::removeFormNode() {
    if (_formNode) {
        removeChild(_formNode);
        _formNode->release();
        _formNode = nullptr;
    }
}

void ARMainControlNode::extensionUpdate(int32_t elapsedTime) {
    gameplay::Vector3 translation;
    gameplay::Quaternion quaternion;
    NodeUtility::transformLookAt(_wpCameraNode, gameplay::Vector3(0.F, 0.F, 1.F),
            &translation, &quaternion);

    if (!isEnabled()) {
        if (translation.y > 0.9F) { //65 degree
            setTranslation(_wpCameraNode->getTranslation() + translation);
            setRotation(quaternion);
            setEnabled(true);
            if (_formNode) {
                _formNode->setEnabled(true);
            }
        }
    } else {
        if (translation.y < 0.65F) { //40 degree
            setEnabled(false);
            if (_formNode) {
                _formNode->setEnabled(false);
            }
        }
    }
}