//
// Created by kt-hyunjun on 2019-12-03.
//

#include <LogUtils.h>
#include <control/ToastState.h>

#define SMOOTH_HIDE_TIME 300

using namespace armakers;

std::vector< std::shared_ptr<ToastState> > ToastState::cache;

std::shared_ptr<ToastState> ToastState::initialize(armakers::ToastFormNode *wp) {
    cache.clear();
    cache.resize(ToastStateType::STATE_MAX);
    cache[ToastStateType::ACTIVATE_TYPE] = std::shared_ptr<ToastState>(new ToastActivate(wp));
    cache[ToastStateType::SHOW_TYPE] = std::shared_ptr<ToastState>(new ToastShow(wp));
    cache[ToastStateType::SMOOTH_HIDE_TYPE] = std::shared_ptr<ToastState>(new ToastSmoothHide(wp));
    cache[ToastStateType::DEACTIVATE_TYPE] = std::shared_ptr<ToastState>(new ToastDeactivate(wp));
    cache[ToastStateType::WAIT_TYPE] = std::shared_ptr<ToastState>(new ToastWait(wp));
    return cache[ToastStateType::ACTIVATE_TYPE];
}

std::shared_ptr<ToastState> ToastState::get(ToastStateType type){
    return cache[type];
}

ToastState::ToastState(ToastFormNode *wp) : wp(wp) {
    _remainElapsedTime = 0;
}

ToastState::~ToastState() {

}

void ToastState::process(int32_t elapsedTime) {

}

void ToastState::changeState(std::shared_ptr<ToastState> state, std::shared_ptr<ToastMessage> next,
        ToastDelegate *delegate) {
    state->toastMessage(next);
    state->configRemainElapsedTime();
    wp->setEnabled(true);
    wp->applyAlpha(wp->getAlpha());
    wp->applyText(next);
    wp->setToastState(state);
    delegate->removeMessage();
}
///////////////////////////////////////////////////////////////////////////////////////////////////
void ToastActivate::process(int32_t elapsedTime) {
    auto d = wp->getToastDelegate();

    if (d->isEmptyMessage()) {
        wp->setEnabled(false);
        wp->setToastState(cache[ToastStateType::WAIT_TYPE]);
    } else {
        auto msg = d->getMessage();
        auto state = cache[ToastStateType::SHOW_TYPE];
        changeState(state, msg, d);
    }
}

void ToastShow::process(int32_t elapsedTime) {
    auto d = wp->getToastDelegate();
    if (!d->isEmptyMessage() && !_spNext) _spNext = d->getMessage();

    _remainElapsedTime -= elapsedTime;
    //LOGD("ToastShow remainElapsedTime (%d)(%s)(%d)", _remainElapsedTime, _toastMessage->msg().c_str(), d->msgSize());
    if (_remainElapsedTime > 0) {
        if (_spNext && _spNext->type() == ToastType::IMMEDIATELY_DRAW_TYPE) {
            auto state = cache[ToastStateType::SHOW_TYPE];
            changeState(state, _spNext, d);
            _spNext.reset();
        }
        return;
    }

    if (!_spNext) {
        auto state = cache[ToastStateType::SMOOTH_HIDE_TYPE];
        state->toastMessage(_toastMessage);
        state->remainElapsedTime(SMOOTH_HIDE_TIME);
        wp->applyAlpha(wp->getAlpha());
        wp->setToastState(state);
    } else {
        auto state = cache[ToastStateType::SHOW_TYPE];
        changeState(state, _spNext, d);
        _spNext.reset();
    }
}

void ToastSmoothHide::process(int32_t elapsedTime) {
    auto d = wp->getToastDelegate();
    if (!d->isEmptyMessage() && !_spNext) _spNext = d->getMessage();

    if (_spNext) {
        auto state = cache[ToastStateType::SHOW_TYPE];
        changeState(state, _spNext, d);
        _spNext.reset();
        return;
    }

    _remainElapsedTime -= elapsedTime;
    if (_remainElapsedTime > 0) {
        auto hideAlpha = wp->getAlpha() * ((float)_remainElapsedTime / (float)SMOOTH_HIDE_TIME);
        wp->applyAlpha(hideAlpha);
        return;
    }

    auto state = cache[ToastStateType::DEACTIVATE_TYPE];
    wp->setEnabled(true);
    //wp->applyAlpha(wp->getAlpha());
    wp->applyText(std::shared_ptr<ToastMessage>(new ToastMessage()));
    wp->setToastState(state);
    _spNext.reset();
}

void ToastDeactivate::process(int32_t elapsedTime) {
    auto d = wp->getToastDelegate();
    if (d->isEmptyMessage()) {
        wp->setEnabled(false);
        wp->setToastState(cache[ToastStateType::WAIT_TYPE]);
    } else {
        auto msg = d->getMessage();
        auto state = cache[ToastStateType::SHOW_TYPE];
        changeState(state, msg, d);
    }
}

void ToastWait::process(int32_t elapsedTime) {

}