//
// Created by kt-hyunjun on 2020-01-25.
//


#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG "RENDERER"
#endif

#include <LogUtils.h>
#include <VideoNode.h>
#include <VideoLayoutFormNode.h>
#include <node/ExtensionRenderable.h>
#include <node/GlobalMenuLayoutFormNode.h>
#include <MonoRenderer.h>

#ifdef __clang__
#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCDFAInspection"
#endif

//#define FEATURE_FRAME_BUFFER_TEST

using namespace gameplay;

IRenderer::IRenderer(Scene *scene)
: wpMainScene(scene), _currentElapsedTime(0), _showWireFrame(false) {

}

IRenderer::~IRenderer() = default;

void IRenderer::processDrawNode(Node *node) {
    //LOGD("IRenderer::processDrawNode id(%s)", node->getId());
    if (node->isEnabled()) {
        /*
            auto formNode = dynamic_cast<FormNode *>(node);
            if (formNode != nullptr) {
                formNode->drawForm(false);
            } else
        */
        {
            Drawable *drawable = node->getDrawable();
            if (drawable) {
                drawable->draw(_showWireFrame);
            }
        }
    }
}

void IRenderer::processUpdateNode(Node *node) {
    auto videoNode = dynamic_cast<VideoNode *>(node);
    if (videoNode != nullptr && videoNode->isEnabled()) {
        videoNode->update();
    }

    auto videoFormNode = dynamic_cast<VideoLayoutFormNode *>(node);
    if (videoFormNode != nullptr && videoFormNode->isEnabled()) {
        videoFormNode->update();
    }

    auto renderAble = dynamic_cast<armakers::ExtensionRenderable *>(node);
    if (renderAble != nullptr) {
        renderAble->extensionUpdate(_currentElapsedTime);
    }
}

void IRenderer::processCapture(bool *capture, const char* fileName) {
    auto *temp = gameplay::FrameBuffer::getCurrent();
    LOGD("IRenderer::captureScene current fb (%d)(%d)(%s)(%s)", temp->getWidth(), temp->getHeight(),
         temp->getId(), fileName);
    auto *image = gameplay::FrameBuffer::createScreenshot();
    if (image) {
        auto *data = image->getData();
        std::string fPath("/sdcard/Android/data/com.example.mediapipemultihandstrackingapp/files/");
        fPath.append(fileName);
        fPath.append(".raw");
        auto *fp = fopen(fPath.c_str(), "w+b");
        fwrite(data, 1, image->getWidth() * image->getHeight() * 4, fp);
        fclose(fp);
    } else {
        LOGD("IRenderer::captureScene image is null");
    }
    SAFE_RELEASE(image);
    *capture = false;
}

///////////////////////////////////////////////////////////

MonoRenderer::MonoRenderer(Scene *scene) : IRenderer(scene)
, _frameBuffer(nullptr), _capture(false)
,_lw(0), _lh(0), _rw(0), _rh(0), _isToggle(true), _setRect(false) {

}

MonoRenderer::~MonoRenderer() {
    clearFrameBuffer();
};

void MonoRenderer::onDraw(){
    onFrameBufferDraw();
    if (_setRect) {
        if (_isToggle) {
            onRectFirstDraw();
        } else {
            onRectSecondDraw();
        }
        _isToggle = !_isToggle;
    } else {
        onDefaultDraw();
    }
}

void MonoRenderer::onStarted() {
    updateFrameBuffer();
}

void MonoRenderer::onStopped() {
    clearFrameBuffer();
}

void MonoRenderer::resizeEvent() {
    updateFrameBuffer();
}

void MonoRenderer::setRectSize(int32_t lw, int32_t lh, int32_t rw,int32_t rh) {
    _lw = lw;
    _lh = lh;
    _rw = rw;
    _rh = rh;
    _setRect = true;
}

bool MonoRenderer::onInternalDraw(Node *node) {
    processDrawNode(node);
    processUpdateNode(node);
    return true;
}

void MonoRenderer::onFrameBufferDraw() {
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
    } else {
        updateFrameBuffer();
        LOGD("MonoRenderer::onFrameBufferDraw error");
    };
#endif
}

void MonoRenderer::onDefaultDraw() {
#if defined(FEATURE_FRAME_BUFFER_TEST)
    gameplay::FrameBuffer::bindDefault();
    auto *pGame = gameplay::Game::getInstance();
    gameplay::Rectangle viewport(0, 0, pGame->getWidth(), pGame->getHeight());
    pGame->setViewport(viewport);
#endif
    wpMainScene->visit(this, &MonoRenderer::onInternalDraw);
}

void MonoRenderer::onRectFirstDraw() {
    auto *pGame = gameplay::Game::getInstance();
    gameplay::Rectangle viewport(0, 0, _lw, _lh);
    pGame->setViewport(viewport);
    wpMainScene->visit(this, &MonoRenderer::onInternalDraw);
}

void MonoRenderer::onRectSecondDraw() {
    auto *pGame = gameplay::Game::getInstance();
    gameplay::Rectangle viewport(0, 0, _rw, _rh);
    pGame->setViewport(viewport);
    wpMainScene->visit(this, &MonoRenderer::onInternalDraw);
}

void MonoRenderer::updateFrameBuffer() {
#if defined(FEATURE_FRAME_BUFFER_TEST)
    auto *pGame = gameplay::Game::getInstance();
    if (pGame->getState() == gameplay::Game::RUNNING) {
        clearFrameBuffer();
        _frameBuffer = gameplay::FrameBuffer::create("custom", pGame->getWidth(), pGame->getHeight());
        LOGD("MonoRenderer::updateFrameBuffer w/h(%d/%d)",pGame->getWidth(), pGame->getHeight());
    } else {
        LOGD("MonoRenderer::updateFrameBuffer state error");
    }
#endif
}

void MonoRenderer::clearFrameBuffer() {
#if defined(FEATURE_FRAME_BUFFER_TEST)
    gameplay::FrameBuffer::resetFrameBuffer();
    SAFE_RELEASE(_frameBuffer);
#endif
}