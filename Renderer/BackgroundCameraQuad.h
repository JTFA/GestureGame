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

namespace gameplay {
	class BackgroundCameraQuad {
	public:
		BackgroundCameraQuad(Scene *scene);

		virtual void updateTransform(maxstAR::CameraDevice *cameraDevice, gameplay::Camera *arCamera);

		virtual void updateBackgroundImage(maxstAR::TrackedImage *image) = 0;

	protected:
		Node *backgroundNode = nullptr;
		Material *backgroundMaterial = nullptr;
	};
}
