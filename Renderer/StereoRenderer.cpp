//
// Created by kt-hyunjun on 2020-01-25.
//


#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG "RENDERER"
#endif

#include <LogUtils.h>

#include <StereoRenderer.h>

#ifdef __clang__
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCDFAInspection"
#endif

//#define FEATURE_FRAME_BUFFER_TEST

using namespace gameplay;

StereoRenderer::StereoRenderer(Scene *scene) : IRenderer(scene)
, _frameBuffer(nullptr), _capture(false) {

}

StereoRenderer::~StereoRenderer() {
    clearFrameBuffer();
};

void StereoRenderer::onDraw(){
    onFrameBufferDraw();
    onDefaultDraw();
}

void StereoRenderer::onStarted() {
    updateFrameBuffer();
}

void StereoRenderer::onStopped() {
    clearFrameBuffer();
}

void StereoRenderer::resizeEvent() {
    updateFrameBuffer();
}

bool StereoRenderer::onInternalDraw(Node *node) {
    processDrawNode(node);
    processUpdateNode(node);
    return true;
}

void StereoRenderer::onFrameBufferDraw() {
#if defined(FEATURE_FRAME_BUFFER_TEST)
    if (_frameBuffer) {
        _frameBuffer->bind();
        auto *pGame = gameplay::Game::getInstance();
        gameplay::Rectangle viewport(0, 0, pGame->getWidth(), pGame->getHeight());
        pGame->setViewport(viewport);
        pGame->clearColor(gameplay::Vector4(0, 0, 0, 0));
        pGame->clearBitField(gameplay::Game::CLEAR_COLOR_DEPTH);
        wpMainScene->visit(this, &MonoRenderer::onInternalDraw);
        if (_capture) processCapture(&_capture, "fb_capture");
    }
#endif
}

void StereoRenderer::onDefaultDraw() {
    auto *pGame = gameplay::Game::getInstance();

    pGame->clear(Game::CLEAR_COLOR_DEPTH, Vector4(0, 0, 0, 0), 1.0F, 0);


    gameplay::FrameBuffer::bindDefault();
    gameplay::Rectangle leftViewport(0, 0, pGame->getWidth() / 2, pGame->getHeight());
    pGame->setViewport(leftViewport);
    wpMainScene->visit(this, &StereoRenderer::onInternalDraw);

    gameplay::FrameBuffer::bindDefault();
    gameplay::Rectangle rightViewport(pGame->getWidth() / 2, 0, pGame->getWidth() / 2, pGame->getHeight());
    pGame->setViewport(rightViewport);
    wpMainScene->visit(this, &StereoRenderer::onInternalDraw);
}

void StereoRenderer::updateFrameBuffer() {
#if defined(FEATURE_FRAME_BUFFER_TEST)
    auto *pGame = gameplay::Game::getInstance();
    if (pGame->getState() == gameplay::Game::RUNNING) {
        clearFrameBuffer();
        _frameBuffer = gameplay::FrameBuffer::create("custom", pGame->getWidth(), pGame->getHeight());
        LOGD("MonoRenderer::updateFrameBuffer w/h(%d/%d)",pGame->getWidth(), pGame->getHeight());
    }
#endif
}

void StereoRenderer::clearFrameBuffer() {
#if defined(FEATURE_FRAME_BUFFER_TEST)
    gameplay::FrameBuffer::resetFrameBuffer();
    SAFE_RELEASE(_frameBuffer);
#endif
}