//
// Created by kt-hyunjun on 2020-01-25.
//

#pragma once

#include <gameplay.h>
#include <MonoRenderer.h>

namespace gameplay {
    class StereoRenderer : public IRenderer {
    public:
        StereoRenderer(Scene *scene);
        virtual ~StereoRenderer();

        virtual void onDraw();

        virtual void onStarted();

        virtual void onStopped();

        virtual void resizeEvent();

        virtual void setCaptures() {
            _capture = true;
        }

    private:
        bool onInternalDraw(Node *node);

        void onFrameBufferDraw();

        void onDefaultDraw();

        void updateFrameBuffer();

        void clearFrameBuffer();

    private:
        FrameBuffer *_frameBuffer;
        bool _capture;
    };

}


