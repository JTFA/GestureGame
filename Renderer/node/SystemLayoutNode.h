//
// Created by kt-hyunjun on 2019-12-14.
//

#pragma once

#include "gameplay.h"
#include <string>
#include <queue>
#include <memory>

#include <node/SystemLayoutFormNode.h>

namespace armakers {
    class SystemLayoutNode : public gameplay::Node {
    public:
        SystemLayoutNode(const std::string &nodeId, std::shared_ptr<armakers::SystemLayout> layout,
                float alpha);
        SystemLayoutNode(const std::string &nodeId, std::shared_ptr<armakers::SystemLayout> layout,
                int w, int h, float alpha);
        virtual ~SystemLayoutNode();
    private:
        armakers::SystemLayoutFormNode *formNode;
    };
}
