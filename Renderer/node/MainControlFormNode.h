//
// Created by kt-hyunjun on 2019-12-14.
//

#pragma once

#include "gameplay.h"
#include <string>
#include <FormNode.h>

#define MAIN_CONTROL_NODE_NAME "MainControlNode"
#define MAIN_CONTROL_FORM_NAME "MainControlLayout"

namespace armakers {

    class MainControlFormNode : public gameplay::FormNode {
    public:
        MainControlFormNode(float alpha = 0.7F);
        virtual ~MainControlFormNode();

        void alpha(float a) { _alpha = a; };
        float alpha() { return _alpha; };

        virtual void controlEvent(gameplay::Control *control, gameplay::Control::Listener::EventType evt);
        virtual void applyAlpha(float alpha);

    protected:
        float _alpha;
    };
}
