/*==============================================================================
Copyright 2017 Maxst, Inc. All Rights Reserved.
==============================================================================*/


#pragma once

#include "Types.h"
#include "TrackedImage.h"

namespace maxstAR
{
	class MAXSTAR_API CloudRecognition
	{
	public:
		static CloudRecognition * getInstance();

		CloudRecognition();
		virtual ~CloudRecognition();
		bool GetFeature(TrackedImage * trackedImage, int pi4ImgWidth, int pi4ImgHeight, int pi4Stride, unsigned char* oyaPacket, int& oiLen);
		const char* jwtEncode(const char* secretKey, const char* payloadString);
	};
}