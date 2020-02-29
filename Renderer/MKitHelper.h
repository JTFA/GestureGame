#pragma once

#ifdef __ANDROID__
#include <jni.h>
#else
#endif

#include <stdio.h>
#include <string>
#include <memory>


#define ENGINE_STARTED  0

using namespace std;

class MKitHelper {
public:
    MKitHelper();
	virtual ~MKitHelper();

	static shared_ptr<MKitHelper> getInstance();

	virtual void clear();
	virtual void launchUrl(const char * url, bool isWebView);
	virtual void onClick(const char* controlId, int resId = -1) = 0;
	virtual void onCheckedChanged(const char * controlId, bool isChecked) = 0;
	virtual void onTrackingFail() = 0;
	virtual void onTrackingSuccess(const char * trackableId, const char * trackableName) = 0;
	virtual void onEvent(int eventId) = 0;

protected:
	static shared_ptr<MKitHelper> instance;

private:

	bool backgroundFlipHorizontal = false;
	bool backgroundFlipVertical = false;
};

