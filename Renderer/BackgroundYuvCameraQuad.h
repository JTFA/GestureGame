//
//  BackgroundCameraQuad.h
//  SampleViewController
//
//  Created by Kimseunglee on 2018. 4. 17..
//  Copyright © 2018년 Maxst. All rights reserved.
//

#include "gameplay.h"
#include <CameraDevice.h>
#include <TrackedImage.h>
#include "BackgroundCameraQuad.h"

namespace gameplay {
	class BackgroundYuvCameraQuad : public BackgroundCameraQuad {
	public:
		BackgroundYuvCameraQuad(Scene *scene);

		void updateBackgroundImage(maxstAR::TrackedImage *image);

	private:
		Texture *textureY = nullptr;
		Texture *textureUV = nullptr;

	};
}
