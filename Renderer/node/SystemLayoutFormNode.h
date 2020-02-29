//
// Created by kt-hyunjun on 2019-10-22.
//

#pragma once


#include <FormNode.h>
#include <control/SystemLayout.h>
#include <node/ExtensionRenderable.h>

namespace armakers {
    class SystemLayoutFormNode : public gameplay::FormNode, public ExtensionRenderable {
    public:
        SystemLayoutFormNode(std::shared_ptr<SystemLayout> layout, float alpha,
                             const char *formName);
        SystemLayoutFormNode(std::shared_ptr<SystemLayout> layout, float alpha, int w, int h,
                             const char *formName);

        ~SystemLayoutFormNode();

        void drawForm(bool wireFrame);

        virtual void controlEvent(gameplay::Control *control, gameplay::Control::Listener::EventType evt);

        virtual void controlPointerTrigger(gameplay::Control *control, gameplay::Control::Listener::EventType evt,
                unsigned int contactIndex, int x, int y);

        virtual void extensionUpdate(int32_t elapsedTime);

        void setEnabled(bool enabled);

        const char *getName();

    private:
        std::shared_ptr<SystemLayout> layout = nullptr;
        gameplay::ImageControl *imageControl;
    };
}