#pragma once

#include "MKitHelper.h"

class DummyHelper: public MKitHelper
{
public:
	void launchUrl(const char * url, bool isWebView);
	void clear();
	void onClick(const char* controlId, int resId = -1);
	void onCheckedChanged(const char * controlId, bool isChecked);
	void onTrackingFail();
	void onTrackingSuccess(const char * trackableId, const char * trackableName); 
};