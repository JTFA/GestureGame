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
    class ToastState;

    class ToastDelegate {
    public:
        ToastDelegate();
        virtual ~ToastDelegate();

        virtual std::shared_ptr<ToastMessage> getMessage() = 0;
        virtual void removeMessage() = 0;
        virtual bool isEmptyMessage() = 0;
        virtual int32_t msgSize() = 0;
    };

    class ToastFormNode : public gameplay::FormNode, public ExtensionRenderable {
    public:
        ToastFormNode(ToastDelegate *wp, float alpha = 0.7F);
        virtual ~ToastFormNode();

        virtual void controlEvent(gameplay::Control *control, gameplay::Control::Listener::EventType evt);
        virtual void applyAlpha(float alpha);
        virtual void extensionUpdate(int32_t elapsedTime);
        void applyText(std::shared_ptr<ToastMessage> sp);
        void onPushToastMessage();

        ToastDelegate* getToastDelegate() { return wpToastDelegate; }
        void setToastState(std::shared_ptr<ToastState> s) { spToastState = s; }
        float getAlpha() { return alpha; };

    protected:
        float alpha;
        ToastDelegate *wpToastDelegate;
        std::shared_ptr<ToastState> spToastState;
        gameplay::Control *_control;
    };
}
