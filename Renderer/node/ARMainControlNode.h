//
// Created by kt-hyunjun on 2019-12-14.
//

#pragma once

#include "gameplay.h"
#include <string>
#include <queue>
#include <memory>

#include <node/ExtensionRenderable.h>
#include <node/MainControlFormNode.h>

namespace armakers {
    class ARMainControlNode : public gameplay::Node, public ExtensionRenderable {
    public:
        ARMainControlNode(const std::string &nodeId, gameplay::Node *cameraNode);
        virtual ~ARMainControlNode();

        virtual void extensionUpdate(int32_t elapsedTime);

        void setFormNode(gameplay::FormNode *node);

        void removeFormNode();
    private:
        gameplay::Node *_wpCameraNode;
//        MainControlFormNode *_formNode;
        gameplay::FormNode *_formNode;
    };
}
