/*==============================================================================
Copyright 2017 Maxst, Inc. All Rights Reserved.
==============================================================================*/


#pragma once

#include <string>
#include <vector>
#include <memory>
#include "Types.h"

namespace maxstAR
{
	/**
	* @brief This class is for camera calibration
	*/
	class MAXSTAR_API CameraCalibration
	{
	public:
		static CameraCalibration* getInstance();

		virtual bool setImage(int width, int height, unsigned char* data) = 0;

		virtual bool calibrate(float* result, int method = 0) = 0;

		virtual bool clearImage() = 0;
	};
}
