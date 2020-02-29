//
//  BackgroundYuvCameraQuad.h
//  SampleViewController
//
//  Created by Kimseunglee on 2018. 4. 17..
//  Copyright © 2018년 Maxst. All rights reserved.
//

#pragma once

#include "gameplay.h"
#include <CameraDevice.h>
#include <TrackedImage.h>
#include "BackgroundCameraQuad.h"

namespace gameplay {
	class BackgroundRgbCameraQuad : public BackgroundCameraQuad {
	public:
		BackgroundRgbCameraQuad(Scene *scene);

		void updateBackgroundImage(maxstAR::TrackedImage *image);

	private:
		Texture *texture = nullptr;
	};
}
