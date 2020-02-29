//
//  BackgroundCameraQuad.cpp
//  SampleViewController
//
//  Created by Kimseunglee on 2018. 4. 17..
//  Copyright © 2018년 Maxst. All rights reserved.
//

#include "BackgroundYuvCameraQuad.h"
#include "SystemUtil.h"
#include <LogUtils.h>

using namespace gameplay;

BackgroundYuvCameraQuad::BackgroundYuvCameraQuad(Scene *scene) :
	BackgroundCameraQuad(scene) {

#ifdef __ANDROID__
	backgroundMaterial = dynamic_cast<Model*>(backgroundNode->getDrawable())->setMaterial("res/shaders/yuvtexture.vert", "res/shaders/yuvtexture_android.frag");
#else
	backgroundMaterial = dynamic_cast<Model*>(backgroundNode->getDrawable())->setMaterial("res/shaders/yuvtexture.vert", "res/shaders/yuvtexture.frag");
#endif
	backgroundMaterial->setParameterAutoBinding("u_worldViewProjectionMatrix", "WORLD_VIEW_PROJECTION_MATRIX");
	backgroundMaterial->getStateBlock()->setDepthTest(true);
}

void BackgroundYuvCameraQuad::updateBackgroundImage(maxstAR::TrackedImage* image)
{
    if (image == nullptr)
    {
        return;
    }

    int imageWidth = image->getWidth();
    int imageHeight = image->getHeight();

    const unsigned char* imagePointer = image->getData();
    
    if(imagePointer != nullptr)
    {
        unsigned char* imageY = (unsigned char*)imagePointer;
        unsigned char* imageUV = (unsigned char*)(imagePointer + imageWidth * imageHeight);
        
        if(textureY == nullptr)
        {
            textureY = Texture::create(gameplay::Texture::LUMINANCE, imageWidth, imageHeight, imageY, false, Texture::TEXTURE_2D);
            textureUV = Texture::create(gameplay::Texture::LUMINANCE_ALPHA, imageWidth / 2, imageHeight / 2, imageUV, false, Texture::TEXTURE_2D);
            Texture::Sampler* samplerY = Texture::Sampler::create(textureY);
            Texture::Sampler* samplerUV = Texture::Sampler::create(textureUV);
            samplerY->setWrapMode(gameplay::Texture::CLAMP, gameplay::Texture::CLAMP);
            samplerUV->setWrapMode(gameplay::Texture::CLAMP, gameplay::Texture::CLAMP);
            backgroundMaterial->getParameter("SamplerY")->setValue(samplerY);
            backgroundMaterial->getParameter("SamplerUV")->setValue(samplerUV);
            LOGD("Create Y, UV texture");
        }
        else
        {
            textureY->setData(imageY);
            textureUV->setData(imageUV);
        }
    }
    else
    {
		LOGD ("Camera image is null");
    }
}

