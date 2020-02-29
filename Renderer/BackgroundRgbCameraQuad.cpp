//
//  BackgroundYuvCameraQuad.cpp
//  SampleViewController
//
//  Created by Kimseunglee on 2018. 4. 17..
//  Copyright © 2018년 Maxst. All rights reserved.
//

#include "BackgroundRgbCameraQuad.h"
#include <LogUtils.h>

using namespace gameplay;

BackgroundRgbCameraQuad::BackgroundRgbCameraQuad(Scene *scene) :
	BackgroundCameraQuad(scene) {

	backgroundMaterial = dynamic_cast<Model*>(backgroundNode->getDrawable())->setMaterial("res/shaders/unlit_texture.vert", "res/shaders/unlit_texture.frag");
	backgroundMaterial->setParameterAutoBinding("u_worldViewProjectionMatrix", "WORLD_VIEW_PROJECTION_MATRIX");
	backgroundMaterial->getStateBlock()->setDepthTest(true);
}

void BackgroundRgbCameraQuad::updateBackgroundImage(maxstAR::TrackedImage* image)
{
	if (image == nullptr)
	{
		return;
	}

	int imageWidth = image->getWidth();
	int imageHeight = image->getHeight();

	const unsigned char* imagePointer = image->getData();

	if (imagePointer != nullptr)
	{
		if (texture == nullptr)
		{
			texture = Texture::create(gameplay::Texture::RGB888, imageWidth, imageHeight, imagePointer, false, Texture::TEXTURE_2D);
			Texture::Sampler* sampler = Texture::Sampler::create(texture);
			sampler->setWrapMode(gameplay::Texture::CLAMP, gameplay::Texture::CLAMP);
			backgroundMaterial->getParameter("u_diffuseTexture")->setValue(sampler);
			LOGD("Create RGB texture");
		}
		else
		{
			texture->setData(imagePointer);
		}
	}
	else
	{
		LOGD("Camera image is null");
	}
}

