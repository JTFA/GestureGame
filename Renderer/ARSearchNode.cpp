//
//  ARSearchNode.cpp
//  SampleViewController
//
//  Created by kwanghee on 2018. 6. 1..
//  Copyright © 2018년 Maxst. All rights reserved.
//

#include "ARSearchNode.h"
#include "gameplay.h"

#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>

using namespace gameplay;

ARSearchNode::ARSearchNode(const char *id, Vector3 translation, Quaternion rotation, Vector3 scale):Node(id),
_materialAlpha(nullptr), _materialAlphaValue(0.4), _state(DONE), _elapsedTime(0), _duration(500), _animationSpeed(400),
_lastDuration(500)
{
    addQuadModelAndNode(1.0f);
    _material = setTextureUnlitMaterial(dynamic_cast<Model*>(getDrawable()), "res/png/square_a60.png", false, false);
    setTranslation(translation);
    setScale(scale);
    gameplay::Quaternion quaternion;
    gameplay::Quaternion::createFromEuler(rotation.x, rotation.y, rotation.z, &quaternion);
    setRotation(quaternion);
    
    for (int i = 0; i < POINT_COUNT; i++)
    {
        Node* node = makeNodeQuadModel(0.3f);
        setTextureUnlitMaterial(dynamic_cast<Model*>(node->getDrawable()), "res/png/circle.png", false, true);
        node->setTranslation(points[i]);
        node->setScale(1);
        addChild(node);
        pointNodeList.push_back(node);
    }
}

ARSearchNode* ARSearchNode::create(const char* id, Vector3 translation, Quaternion rotation, Vector3 scale)
{
    ARSearchNode* node = new ARSearchNode(id, translation, rotation, scale);
    return node;
}

void ARSearchNode::addQuadModelAndNode(float size)
{
    Mesh* mesh = Mesh::createQuad(Vector3(-size / 2, -size / 2, 0), Vector3(-size / 2, size / 2, 0),
                                  Vector3( size / 2, -size / 2, 0), Vector3( size / 2, size / 2, 0));
    Model* model = Model::create(mesh);
    setDrawable(model);
    SAFE_RELEASE(model);
    SAFE_RELEASE(mesh);
}

Node* ARSearchNode::makeNodeQuadModel(float size)
{
    Node* node = Node::create();
    Mesh* mesh = Mesh::createQuad(Vector3(-size / 2, -size / 2, 0), Vector3(-size / 2, size / 2, 0),
                                  Vector3( size / 2, -size / 2, 0), Vector3( size / 2, size / 2, 0));
    Model* model = Model::create(mesh);
    node->setDrawable(model);
    SAFE_RELEASE(model);
    SAFE_RELEASE(mesh);
    
    return node;
}

Material* ARSearchNode::setTextureUnlitMaterial(Model* model, const char* texturePath, bool mipmap, bool isPoint)
{
    Material* material;
    if (isPoint)
    {
        material = model->setMaterial("res/shaders/textured.vert", "res/shaders/textured.frag");
        material->setParameterAutoBinding("u_worldViewProjectionMatrix", "WORLD_VIEW_PROJECTION_MATRIX");
    }
    else
    {
        material = model->setMaterial("res/shaders/textured.vert", "res/shaders/textured.frag", "MODULATE_ALPHA");
        material->setParameterAutoBinding("u_worldViewProjectionMatrix", "WORLD_VIEW_PROJECTION_MATRIX");
        _materialAlpha = material->getParameter("u_modulateAlpha");
        _materialAlpha->setValue(_materialAlphaValue);
    }
    
    // Load the texture from file.
    Texture::Sampler* sampler = material->getParameter("u_diffuseTexture")->setValue(texturePath, mipmap);
    if (mipmap)
        sampler->setFilterMode(Texture::LINEAR_MIPMAP_LINEAR, Texture::LINEAR);
    else
        sampler->setFilterMode(Texture::LINEAR, Texture::LINEAR);
    sampler->setWrapMode(Texture::CLAMP, Texture::CLAMP);
    
    material->getStateBlock()->setCullFace(true);
    material->getStateBlock()->setDepthTest(true);
    material->getStateBlock()->setDepthWrite(true);
    
    material->getStateBlock()->setBlend(true);
    material->getStateBlock()->setBlendSrc(RenderState::BLEND_SRC_ALPHA);
    material->getStateBlock()->setBlendDst(RenderState::BLEND_ONE_MINUS_SRC_ALPHA);
    
    return material;
}

void ARSearchNode::searching(Vector3 translation, Quaternion rotation, Vector3 scale, float elapsedTime)
{
    if (_state != SEARCHING)
    {
        _state = SEARCHING;
        _materialAlphaValue = 0.4;
        _materialAlpha->setValue(_materialAlphaValue);
        setEnabled(true);
        setTranslation(translation);
        setScale(scale);
        setRotation(rotation);
        _elapsedTime = 0;
        
        for (auto itor = pointNodeList.begin(); itor != pointNodeList.end(); itor++)
        {
            Node* node = (*itor);
            node->setScale(1);
            node->setEnabled(true);
        }
    }
    else
    {
        _elapsedTime += elapsedTime;
        
        float scaleSize = std::abs(std::sin(_elapsedTime / _animationSpeed * M_PI)) + 1;
        for (int i = 0; i < pointNodeList.size(); i++)
        {
            if ((int) (_elapsedTime / _animationSpeed) % POINT_COUNT == i)
            {
                pointNodeList[i]->setScale(scaleSize);
            }
            else
            {
                pointNodeList[i]->setScale(1);
            }
        }
    }
}

float ARSearchNode::getAnimateProgress(float elapsedTime, float duration)
{
    _elapsedTime += elapsedTime;
    
    float currentTime = 0.0f;
    
    currentTime = fmodf(_elapsedTime, duration);
    
    float percentComplete = currentTime / (float) duration;
    return percentComplete;
}

void ARSearchNode::foundTrackable(Vector3 translation, Quaternion rotation, Vector3 scale, float elapsedTime)
{
    if (_state == SEARCHING)
    {
        _state = FINDING;
        _elapsedTime = 0;
        setTranslation(translation);
        setRotation(rotation);
    }
    else if (_state == FINDING)
    {
        float percentComplete = getAnimateProgress(elapsedTime, _duration);
        Vector3 scaleSize = scale * percentComplete;
        setTranslation(translation);
        setScale(scaleSize);
        setRotation(rotation);
        
        for (int i = 0; i < pointNodeList.size(); i++)
        {
            pointNodeList[i]->setScale(0.05/scaleSize.x, 0.05/scaleSize.y, 1);
        }
        
        if (percentComplete >= 0.9)
        {
            _state = FOUND;
            _elapsedTime = 0;
            setScale(scale);
        }
    }
    else if (_state == FOUND)
    {
        float percentComplete = getAnimateProgress(elapsedTime, _lastDuration);
        _materialAlphaValue = percentComplete * 0.4 + 0.4;
        _materialAlpha->setValue(_materialAlphaValue);
        setTranslation(translation);
        setScale(scale);
        setRotation(rotation);
        
        if (_elapsedTime > _lastDuration)
        {
            _state = DONE;
            setEnabled(false);
            
            for (int i = 0; i < pointNodeList.size(); i++)
            {
                pointNodeList[i]->setEnabled(false);
            }
        }
    }
}
