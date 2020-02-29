//
// Created by kt-hyunjun on 2020-01-25.
//

#pragma once

#include <gameplay.h>

namespace gameplay {

    typedef enum RendererType {
        UNKNOWN_TYPE = -1,
        MONO_TYPE = 0,
        STEREO_TYPE = 1,
    } RendererType;

    class IRenderer {
    public:
        IRenderer(Scene *scene);

        virtual ~IRenderer();

        virtual void onDraw() {

        }

        virtual void onStarted() {

        }

        virtual void onStopped() {

        }

        virtual void onChange() {

        }

        virtual void resizeEvent() {

        }

        virtual void setCaptures() {

        }

        virtual void setRectSize(int32_t lw, int32_t lh, int32_t rw,int32_t rh) {

        }

        void currentElapsedTime(int32_t t) { _currentElapsedTime = t; }

        void showWireFrame(bool ret) { _showWireFrame = ret; }

    protected:
        void processDrawNode(Node *node);

        void processUpdateNode(Node *node);

        void processCapture(bool *capture, const char* fileName);

    protected:
        Scene *wpMainScene;
        int32_t _currentElapsedTime;
        bool _showWireFrame;
    };

    class MonoRenderer : public IRenderer {
    public:
        MonoRenderer(Scene *scene);
        virtual ~MonoRenderer();

        virtual void onDraw();

        virtual void onStarted();

        virtual void onStopped();

        virtual void resizeEvent();

        virtual void setCaptures() {
            _capture = true;
        }

        virtual void setRectSize(int32_t lw, int32_t lh, int32_t rw,int32_t rh);

    private:
        bool onInternalDraw(Node *node);

        void onFrameBufferDraw();

        void onDefaultDraw();

        void onRectFirstDraw();

        void onRectSecondDraw();

        void updateFrameBuffer();

        void clearFrameBuffer();

    private:
        FrameBuffer *_frameBuffer;
        bool _capture;
        int32_t _lw;
        int32_t _lh;
        int32_t _rw;
        int32_t _rh;
        bool _setRect;
        bool _isToggle;
    };

}


