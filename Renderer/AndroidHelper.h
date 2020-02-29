#pragma once

#ifdef __ANDROID__
#include "MKitHelper.h"
#include <jni.h>

class AndroidHelper: public MKitHelper
{
public:
	void initJavaVariables(JavaVM * javaVM, jobject activityObject, jobject jniHelperObject);
	void launchUrl(const char * url, bool isWebView);
	void clear();
	void onClick(const char* controlId, int resId = -1);
	void onCheckedChanged(const char * controlId, bool isChecked);
	void onTrackingFail();
	void onTrackingSuccess(const char * trackableId, const char * trackableName);
	void onEvent(int eventId);

private:
    static shared_ptr<AndroidHelper> instance;

	jobject jniHelperObject = 0;
	jobject activityObject = 0;
	jmethodID onClickMethodId = 0;
	jmethodID onCheckedChangedMethodId = 0;
	jmethodID onTrackingFailMethodId = 0;
	jmethodID onTrackingSuccessMethodId = 0;
	jmethodID onEventMethodId = 0;
	JavaVM * javaVM = nullptr;
};

#endif // __ANDROID__