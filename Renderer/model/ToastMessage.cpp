//
// Created by kt-hyunjun on 2019-12-03.
//

#include "ToastMessage.h"

using namespace armakers;

ToastMessage::ToastMessage() : _msg(""), _millisecond(0), _type(WAITING_DRAW_TYPE) {

}

ToastMessage::ToastMessage(const std::string &msg, int32_t millisecond, ToastType type)
: _msg(msg), _millisecond(millisecond), _type(type) {

}

ToastMessage::~ToastMessage() {

}