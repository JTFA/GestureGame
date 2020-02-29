//
//  BackgroundCameraQuad.cpp
//  SampleViewController
//
//  Created by Kimseunglee on 2018. 4. 17..
//  Copyright © 2018년 Maxst. All rights reserved.
//

#include "BackgroundCameraQuad.h"
#include "SystemUtil.h"
#include <LogUtils.h>

using namespace gameplay;

BackgroundCameraQuad::BackgroundCameraQuad(Scene *scene) {
    const float cubeSize = 0.5f;
    Mesh* mesh = Mesh::createQuad(Vector3(-cubeSize, -cubeSize, 0), Vector3(-cubeSize, cubeSize, 0), Vector3(cubeSize, -cubeSize, 0), Vector3(cubeSize, cubeSize, 0));
    Model* model = Model::create(mesh);
	backgroundNode = scene->addNode();
	backgroundNode->setDrawable(model);
    SAFE_RELEASE(model);
    SAFE_RELEASE(mesh);
}

void BackgroundCameraQuad::updateTransform(maxstAR::CameraDevice* cameraDevice, gameplay::Camera* arCamera) {
    float widthRatio = (float)Game::getInstance()->getWidth() / cameraDevice->getWidth();
    float heightRatio = (float)Game::getInstance()->getHeight() / cameraDevice->getHeight();
    float farClipPlane = arCamera->getFarPlane() * 0.90f;
    float tanFovWidth = (1.0f / (float)Game::getInstance()->getWidth()) * (float)Game::getInstance()->getHeight();
    float frustumWidth = tanFovWidth * farClipPlane * arCamera->getAspectRatio();
    float viewWidth = (float)frustumWidth / Game::getInstance()->getWidth();
    float viewHeight = viewWidth * (widthRatio / heightRatio);

    float flipHorizontal = -1.0f;
    float flipVertical = 1.0f;

    if (SystemUtil::getInstance()->isFlipHorizontal()) {
        flipHorizontal = 1.0f;
    }

    if (SystemUtil::getInstance()->isFlipVertical()) {
        flipVertical = -1.0f;
    }

    if (widthRatio > heightRatio)
    {
        backgroundNode->setScale(widthRatio * cameraDevice->getWidth() * viewWidth * flipVertical,
        						widthRatio * cameraDevice->getHeight() * viewWidth * flipHorizontal, 0.1f);
        backgroundNode->setTranslation(0.0f, 0.0f, farClipPlane);
    }
    else
    {
        backgroundNode->setScale(heightRatio * cameraDevice->getWidth() * viewHeight * flipVertical,
                                 heightRatio * cameraDevice->getHeight() * viewHeight * flipHorizontal, 0.1f);
        backgroundNode->setTranslation(0.0f, 0.0f, farClipPlane);
    }
}

