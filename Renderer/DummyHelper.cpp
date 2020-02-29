#include <LogUtils.h>

#define LOG_NDEBUG 0

#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG "DummyHelper"
#endif

#include "DummyHelper.h"

shared_ptr<MKitHelper> MKitHelper::getInstance() {
	if (instance == nullptr) {
		instance = shared_ptr<MKitHelper>(new DummyHelper());
	}
	return instance;
}

void DummyHelper::launchUrl(const char *url, bool isWebView) {
}

void DummyHelper::onTrackingFail() {
}

void DummyHelper::onTrackingSuccess(const char * trackableId, const char * trackableName) {
}

void DummyHelper::onClick(const char* controlId, int resId) {
}

void DummyHelper::onCheckedChanged(const char * controlId, bool isChecked) {
}

void DummyHelper::clear() {
	MKitHelper::clear();
}

