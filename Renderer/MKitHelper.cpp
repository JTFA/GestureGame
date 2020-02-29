#include "MKitHelper.h"

shared_ptr<MKitHelper> MKitHelper::instance = nullptr;

MKitHelper::MKitHelper() {
}

MKitHelper::~MKitHelper() {
}

void MKitHelper::launchUrl(const char * url, bool isWebView)
{
}

void MKitHelper::clear()
{
	backgroundFlipHorizontal = false;
	backgroundFlipVertical = false;
}