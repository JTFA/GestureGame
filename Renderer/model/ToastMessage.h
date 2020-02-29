//
// Created by kt-hyunjun on 2019-12-03.
//

#pragma once

#include <stdio.h>
#include <stdint.h>
#include <string>

namespace armakers {

    enum ToastType {
        WAITING_DRAW_TYPE = 0,
        IMMEDIATELY_DRAW_TYPE,
    };

    class ToastMessage {
    public:
        ToastMessage();
        ToastMessage(const std::string &msg, int32_t millisecond, ToastType type);
        virtual ~ToastMessage();

        void millisecond(int32_t t) { _millisecond = t; }
        int32_t millisecond() { return _millisecond; }

        void msg(const std::string &s) { _msg = s; }
        std::string & msg() { return _msg; }

        void type(ToastType t) { _type = t; }
        ToastType type() { return _type; }

    private:
        std::string _msg;
        int32_t _millisecond;
        ToastType _type;
    };
}

