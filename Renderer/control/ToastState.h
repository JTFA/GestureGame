//
// Created by kt-hyunjun on 2019-12-03.
//

#pragma once

#include <vector>

#include <node/ToastFormNode.h>
#include <model/ToastMessage.h>

namespace armakers {
    enum ToastStateType {
        ACTIVATE_TYPE = 0,
        SHOW_TYPE,
        SMOOTH_HIDE_TYPE,
        DEACTIVATE_TYPE,
        WAIT_TYPE,
        STATE_MAX
    };

    class ToastState {
        //static
    public:
        static std::shared_ptr<ToastState> initialize(ToastFormNode *wp);
        static std::shared_ptr<ToastState> get(ToastStateType type);
    protected:
        static std::vector< std::shared_ptr<ToastState> > cache;

    public:
        ToastState(ToastFormNode *wp);
        virtual ~ToastState();

        virtual void process(int32_t elapsedTime);

        void configRemainElapsedTime() { _remainElapsedTime = _toastMessage ? _toastMessage->millisecond() : 0; }
        void toastMessage(std::shared_ptr<ToastMessage> s) { _toastMessage = s; }
        void remainElapsedTime(int32_t t) { _remainElapsedTime = t; }
        ToastStateType type() { return _type; }

    protected:
        void changeState(std::shared_ptr<ToastState> s, std::shared_ptr<ToastMessage> next, ToastDelegate *delegate);

    protected:
        ToastFormNode *wp;
        std::shared_ptr<ToastMessage> _toastMessage;
        int32_t _remainElapsedTime;
        ToastStateType _type;
    };

    class ToastActivate : public ToastState {
    public:
        ToastActivate(ToastFormNode *wp) : ToastState(wp) { _type = ACTIVATE_TYPE; };
        virtual ~ToastActivate() {};

        virtual void process(int32_t elapsedTime);
    };

    class ToastShow : public ToastState {
    public:
        ToastShow(ToastFormNode *wp) : ToastState(wp) { _type = SHOW_TYPE; };

        virtual ~ToastShow() {};

        virtual void process(int32_t elapsedTime);

    protected:
        std::shared_ptr<ToastMessage> _spNext;
    };

    class ToastSmoothHide : public ToastState {
    public:
        ToastSmoothHide(ToastFormNode *wp) : ToastState(wp) { _type = SMOOTH_HIDE_TYPE; };
        virtual ~ToastSmoothHide() {};

        virtual void process(int32_t elapsedTime);

    protected:
        std::shared_ptr<ToastMessage> _spNext;
    };

    class ToastDeactivate : public ToastState {
    public:
        ToastDeactivate(ToastFormNode *wp) : ToastState(wp) { _type = DEACTIVATE_TYPE; };
        virtual ~ToastDeactivate() {};

        virtual void process(int32_t elapsedTime);
    };

    class ToastWait : public ToastState {
    public:
        ToastWait(ToastFormNode *wp) : ToastState(wp) { _type = WAIT_TYPE; };
        virtual ~ToastWait() {};

        virtual void process(int32_t elapsedTime);
    };
}
