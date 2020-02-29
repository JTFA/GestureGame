//
// Created by kt-hyunjun on 2019-12-03.
//

#pragma once

#include "gameplay.h"
#include <string>
#include <FormNode.h>

#include <model/ToastMessage.h>
#include <node/ExtensionRenderable.h>

namespace armakers {

    class TargetOnFormNode : public gameplay::FormNode {
    public:
        TargetOnFormNode(float alpha = 1.0F);
        virtual ~TargetOnFormNode();

        virtual void controlEvent(gameplay::Control *control, gameplay::Control::Listener::EventType evt);
        virtual void applyAlpha(float alpha);

        float getAlpha() { return alpha; };

    protected:
        float alpha;
    };

    class TargetOnNode : public gameplay::Node {
    public:
        TargetOnNode(const std::string &nodeId);
        virtual ~TargetOnNode();

        void processEnabled(bool enable);

    private:
        TargetOnFormNode *pTargetOnFormNode;
    };
}
