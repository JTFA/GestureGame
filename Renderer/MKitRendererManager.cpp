#include <cmath>
#include <memory>
#include "MKitRendererManager.h"
#include <LogUtils.h>
#include "SystemUtil.h"
#include "BackgroundYuvCameraQuad.h"
#include "BackgroundRgbCameraQuad.h"
#include "AndroidHelper.h"
#include "Utilities.h"
#include "Layout1FormNode.h"
#include "Layout2FormNode.h"
#include "Layout3FormNode.h"
#include "IconLayoutFormNode.h"
#include "TagLayoutFormNode.h"
#include "ChildLayoutFormNode.h"
#include <TextLayoutFormNode.h>
#include <VideoLayoutFormNode.h>
#include <filter/LowPassFilter.h>
#include <node/SystemLayoutNode.h>
#include <node/SystemLayoutFormNode.h>
#include <node/ImageLayoutFormNode.h>
#include <node/ImagePagesFormNode.h>
#include <node/GlobalMenuLayoutFormNode.h>
#include <node/ARComplexViewerNode.h>
#include <utility/FormUtility.h>
#include <StereoRenderer.h>

//#include <glass/nreal/NRStereoRenderer.h>

#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG "MKitRendererManager"
#endif

#define FEATURE_FIX_RENDER_SIZE
//#define FEATURE_STEREO_RENDERING_ENABLE

using namespace gameplay;

static void addDummy(Node * parent) {
	ImageNode * node = new ImageNode("dummy", "res/png/logo.png", false);

	node->setTranslation(Vector3(0.0f, 0.0f, 10000.0f));
	parent->addChild(node);
	SAFE_RELEASE(node);
}

static void addDummy2(Node * parent) {
    ImageNode * node = new ImageNode("dummy", "res/png/logo.png", false);

    node->setTranslation(Vector3(0.0f, 0.0f, -10.0f));
    parent->addChild(node);
    SAFE_RELEASE(node);
}

MKitRendererManager::MKitRendererManager()
: mainScene(nullptr), showWireFrame(false), cameraNode(nullptr), animation(nullptr)
, globalMenuNode(nullptr), wpToastNode(nullptr), wpMainControlNode(nullptr), wpTargetOnNode(nullptr)
, trackingNode(nullptr), simpleTrackingNode(nullptr), rendererType(UNKNOWN_TYPE) {
	spRenderer = std::make_shared<IRenderer>(nullptr);
}

MKitRendererManager::~MKitRendererManager() = default;

void MKitRendererManager::initialize() {
	LOGD("initialize");
	LOGD("MaxstARSDK Version : %s", maxstAR::getVersion());

	mainScene = Scene::create();

	trackableNodeMap = new map<string, Node*>;

	Camera *camera = Camera::createPerspective(45.0f, getAspectRatio(), 0.01, 1000);
	cameraNode = mainScene->addNode("camera");
	cameraNode->setCamera(camera);
	cameraNode->setTranslation(Vector3(0.0f, 0.0f, 0.0f));
	cameraNode->setRotation(Quaternion::identity());
	mainScene->setActiveCamera(camera);
	SAFE_RELEASE(camera);

	trackingNode = Node::create("trackingNode");
	simpleTrackingNode = Node::create("simpleTrackingNode");

    cameraNode->addChild(trackingNode);
	cameraNode->addChild(simpleTrackingNode);
	//mainScene->addNode(trackingNode);

    wpToastNode = new armakers::ARToastNode("ARToastNode");
    wpToastNode->setTranslation(Vector3(0.0f, 0.17f, 1.0f));
    cameraNode->addChild(wpToastNode);
	//SAFE_RELEASE(wpToastNode);	// SAFE_RELEASE 시 대상 변수를 null 처리 하여 해당 객체를 가져올 수 없음
	wpToastNode->release();

    wpTargetOnNode = new armakers::TargetOnNode("TargetOnNode");
    wpTargetOnNode->setTranslation(Vector3(0.0f, 0.0f, 1.0f));
    wpTargetOnNode->setScale(Vector3(0.75f, 0.75f, 0.75f));
    cameraNode->addChild(wpTargetOnNode);
	targetOnEnabled(false);
    wpTargetOnNode->release();

	wpMainControlNode = new armakers::ARMainControlNode("ARMainControlNode", cameraNode);
	mainScene->addNode(wpMainControlNode);
//	SAFE_RELEASE(wpMainControlNode);
	wpMainControlNode->release();

#if defined(FEATURE_STEREO_RENDERING_ENABLE)
	updateRendererType(STEREO_TYPE);
#else
	updateRendererType(MONO_TYPE);
#endif

	addDummy(trackingNode);
	addDummy(simpleTrackingNode);
    addDummy2(cameraNode);
}

void MKitRendererManager::clearContent() {
	formNodeList.clear();
	trackingNode->removeAllChildren();
	addDummy(trackingNode);
	simpleTrackingNode->removeAllChildren();
	addDummy(simpleTrackingNode);
}

void MKitRendererManager::clearContent(char * controlId) {
	Node * node = trackingNode->findNode(controlId);
	if (node != nullptr) {
		FormNode * formNode = dynamic_cast<FormNode*>(node);
		if (formNode != nullptr) {
			formNodeList.remove(formNode);
		}
		trackingNode->removeChild(node);

		SAFE_RELEASE(node);
	}

	node = simpleTrackingNode->findNode(controlId);
	if (node != nullptr) {
		FormNode * formNode = dynamic_cast<FormNode*>(node);
		if (formNode != nullptr) {
			formNodeList.remove(formNode);
		}
		simpleTrackingNode->removeChild(node);

		SAFE_RELEASE(node);
	}
}

void MKitRendererManager::resizeEvent(unsigned int width, unsigned int height) {
	maxstAR::onSurfaceChanged(width, height);
    spRenderer->resizeEvent();
}

void MKitRendererManager::onStarted() {
	spRenderer->onStarted();
	//occur event
    MKitHelper::getInstance()->onEvent(ENGINE_STARTED);
}

void MKitRendererManager::finalize() {
	LOGD("finalize call");

	trackingNode->removeAllChildren();
	simpleTrackingNode->removeAllChildren();
	layoutNodeMap.clear();
	SAFE_RELEASE(mainScene);

	for (auto &it : formNodeList) {
		//SAFE_RELEASE(it);
	}
}

void MKitRendererManager::update(float elapsedTime) {
    sensorMutex.lock();
    rotation = sensorRotation;
    sensorMutex.unlock();
    cameraNode->setRotation(rotation);

	//Matrix cameraProjectionMatrix = Matrix(projectionMatrix);
	float cameraProjectionMatrix[] = { 0.000000f, -2.000000f, 0.000000f, 0.000000f,
									   -4.100000f,0.000000f,0.000000f,0.000000,
									   0.000000f,0.000000f,1.000012f,1.000000,
									   0.000000f,0.000000f,-0.060000f,0.000000};
	mainScene->getActiveCamera()->setProjectionMatrix(cameraProjectionMatrix);

	if (isSceneCaptured) {
		trackingNode->set(scale, rotation, translation);
	} else {
		if (isTrackingSuccess) {
			Matrix pose = Matrix(poseMatrix);
			pose.getScale(&scale);
			pose.getRotation(&rotation);
			pose.getTranslation(&translation);
			trackingNode->set(scale, rotation, translation);
			setEnableNode(trackingNode, true);
#if defined(FEATURE_FIX_RENDER_SIZE)
			//TODO::Prototype - fix the render size
			//float distance = translation.distance(cameraNode->getTranslation()) * 0.1F - 0.5F;
            float distance = abs(translation.z) * 0.1F - 0.5F;
			Vector3::add(scale, Vector3(distance, distance, distance), &fixedScale);
            gameplay::Vector3 localRotate;
            rotation.computeEuler(&localRotate.x, &localRotate.y, &localRotate.z);
            Quaternion::createFromEuler(MATH_DEG_TO_RAD(0.F), MATH_DEG_TO_RAD(0.F),
                                        localRotate.z, &fixedRotation);
			simpleTrackingNode->set(fixedScale, fixedRotation, translation);
			setEnableNode(simpleTrackingNode, true);
#endif
		} else {
			setEnableNode(trackingNode, false);
#if defined(FEATURE_FIX_RENDER_SIZE)
			setEnableNode(simpleTrackingNode, false);
#endif
		}
	}
}

void MKitRendererManager::render(float elapsedTime) {
    if (!spRenderer) return;
    spRenderer->currentElapsedTime((int32_t)round(elapsedTime));
	spRenderer->onDraw();
}

void MKitRendererManager::changeRendererSize(int32_t lw, int32_t lh, int32_t rw, int32_t rh) {
	LOGD("MKitRendererManager::changeRendererSize rect(%d)(%d)(%d)(%d)", lw, lh, rw, rh);
	updateRendererType(MONO_TYPE);
	spRenderer->setRectSize(lw, lh, rw, rh);
}

void MKitRendererManager::updateRendererType(RendererType type) {
	if (rendererType == type) return;
	rendererType = type;
    LOGD("MKitRendererManager::updateRendererType rendererType::(%d)", rendererType);
	switch (type) {
		case MONO_TYPE:
			spRenderer = std::make_shared<MonoRenderer>(mainScene);
			break;
		case STEREO_TYPE:
			spRenderer = std::make_shared<StereoRenderer>(mainScene);
			break;
		default:
			spRenderer = std::make_shared<IRenderer>(mainScene);
			break;
	}
	spRenderer->showWireFrame(showWireFrame);
	spRenderer->onChange();
}


void MKitRendererManager::setEnableNode(Node *node, bool enabled) {
	node->setEnabled(enabled);
	setEnableChildrenNode(node, enabled);
}

void MKitRendererManager::setEnableChildrenNode(Node *node, bool enabled) {
	for (Node *child = node->getFirstChild(); child != nullptr; child = child->getNextSibling()) {
		if (child->getChildCount() > 0) {
			setEnableChildrenNode(child, enabled);
		}

		child->setEnabled(enabled);
	}
}

void MKitRendererManager::setProjectionMatrix(float * matrix) {
	memcpy(projectionMatrix, matrix, sizeof(float) * 16);
}

void MKitRendererManager::setTrackingResult(bool isSuccess, char * trackableId, char * trackableName, float * poseMatrix) {
	isTrackingSuccess = isSuccess;
	if (isTrackingSuccess) {
		this->trackableId = trackableId;
		this->trackableName = trackableName;
		memcpy(this->poseMatrix, poseMatrix, sizeof(float) * 16);
	}
}

void MKitRendererManager::setCameraQuaternion(float * quaternion) {
	if (!isnanf(quaternion[0] + quaternion[1] + quaternion[2] + quaternion[3])) {
        Quaternion sensorPose(quaternion);
        Quaternion x180;
        Quaternion::createFromAxisAngle(Vector3(1.F, 0.F, 0.F), MATH_DEG_TO_RAD(180.F), &x180);
        sensorPose.multiply(x180);

        Vector3 zAddRotate(0.F, 0.F, 0.F);
        sensorPose.computeEuler(&zAddRotate.x, &zAddRotate.y, &zAddRotate.z);
        Quaternion::createFromEuler(zAddRotate.x, zAddRotate.y,
        		zAddRotate.z + MATH_DEG_TO_RAD(180.F), &sensorPose);
        sensorMutex.lock();
        armakers::LowPassFilter::update(sensorRotation, sensorPose, &sensorRotation);
        sensorMutex.unlock();
    }
}

void MKitRendererManager::addLayout1(char *layoutName, char *imageTextPath,
									 char *checkControlId, char *buttonControlId,
									 Vector3 position, Vector3 scale,
									 float width, float height) {

	FormNode *formNode = new Layout1FormNode(layoutName, imageTextPath, checkControlId,
											 buttonControlId);
	formNodeList.push_back(formNode);

	Vector3 formScale = Utilities::getScale(formNode->getFormBounds(), scale);
	Vector3 translation = Utilities::getTranslate(width, height, position);

	formNode->setTranslation(translation);
	formNode->setScale(formScale);

	trackingNode->addChild(formNode);
	SAFE_RELEASE(formNode);
}

void MKitRendererManager::addChildLayout(char *layoutName, char *imageTextPath,
										 char *checkControlId, char *buttonControlId,
										 Vector3 position, Vector3 scale,
										 float width, float height, bool isChild) {

	FormNode *formNode = new ChildLayoutFormNode(layoutName, imageTextPath, checkControlId,
											 buttonControlId);
	formNodeList.push_back(formNode);

	Vector3 formScale = Utilities::getScale(formNode->getFormBounds(), scale);
	Vector3 translation = Utilities::getTranslate(width, height, position);

	formNode->setTranslation(translation);
	formNode->setScale(formScale);

	if(isChild) checkNode->addChild(formNode);
	else itemNode->addChild(formNode);
}

void MKitRendererManager::addLayout2(char *layoutName, char *imageTextPath,
									 char *checkControlId, char *buttonControlId,
									 Vector3 position, Vector3 scale) {

	FormNode *formNode = new Layout2FormNode(layoutName, imageTextPath,
											 checkControlId, buttonControlId);
	formNodeList.push_back(formNode);
	Vector3 formScale = Vector3(0.001f, -0.001f, 0.001f);
	Vector3 translation = Vector3(0, 0, 0);

	formNode->setTranslation(translation);
	formNode->setScale(formScale);
	trackingNode->addChild(formNode);
}

void MKitRendererManager::addLayout3(char *layoutName, char *checkControlId, char *buttonControlId,
									 Vector3 position, Vector3 scale) {
	FormNode *formNode = new Layout3FormNode(layoutName, checkControlId, buttonControlId);
	formNodeList.push_back(formNode);
	Vector3 formScale = Vector3(0.001f, -0.001f, 0.001f);
	Vector3 translation = Vector3(0, 0, 0);

	formNode->setTranslation(translation);
	formNode->setScale(formScale);
	trackingNode->addChild(formNode);
}

void MKitRendererManager::addIconLayout(char *layoutName, char *textPath, Vector3 position,
										Vector3 scale, float width, float height) {
	FormNode *formNode = new IconLayoutFormNode(layoutName, textPath);
	formNodeList.push_back(formNode);

	Vector3 formScale = Utilities::getScale(formNode->getFormBounds(), scale);
	Vector3 translation = Utilities::getTranslate(width, height, position);

	formNode->setTranslation(translation);
	formNode->setScale(formScale);
}

void MKitRendererManager::addTextLayout(char *layoutName, char *textPath, char * controlId,
										Vector3 position, Vector3 scale,
										float width, float height) {
	FormNode *formNode = new TextLayoutFormNode(layoutName, textPath, controlId);
	formNodeList.push_back(formNode);

	Vector3 formScale = Utilities::getScale(formNode->getFormBounds(), scale);
	Vector3 translation = Utilities::getTranslate(width, height, position);

	formNode->setTranslation(translation);
	formNode->setScale(formScale);
	trackingNode->addChild(formNode);
//	SAFE_RELEASE(formNode);
}

void MKitRendererManager::addTagLayout(char *layoutName, char *imageTextPath, char *buttonControlId,
									   Vector3 position, Vector3 scale, float width, float height) {

	FormNode *formNode = new TagLayoutFormNode(layoutName, imageTextPath, buttonControlId);
	formNodeList.push_back(formNode);

	Vector3 formScale = Utilities::getScale(formNode->getFormBounds(), scale);
	Vector3 translation = Utilities::getTranslate(width, height, position);

	formNode->setTranslation(translation);
	formNode->setScale(formScale);
	itemNode->addChild(formNode);
}

void MKitRendererManager::addImage(const char *name, const char *imagePath, Vector3 position, Vector3 rotation, Vector3 scale, bool modulateAlpha) {
	ImageNode * node = new ImageNode(name, imagePath, false, modulateAlpha);

	Quaternion quaternion;
	Quaternion::createFromEuler(MATH_DEG_TO_RAD(rotation.x),
										  MATH_DEG_TO_RAD(rotation.y),
										  MATH_DEG_TO_RAD(rotation.z), &quaternion);

	node->setTranslation(position);
	node->setRotation(quaternion);
	node->setScale(scale);
	trackingNode->addChild(node);
	SAFE_RELEASE(node);
}

void MKitRendererManager::addImageAndAlpha(const char *controlId, const char *imagePath, Vector3 position, Vector3 rotation, Vector3 scale, float alpha) {
    /////////////////////////////////
    // kcshin@KT 2019-12-04
    // -- controlId 없는 경우에만 새 Node 생성
    // -- 노드 존재시에는 벡터값만 세팅

    ImageNode * node = NULL;

    Node* match = trackingNode->findNode(controlId, false, true);
    if ( match == NULL )    {
        node = new ImageNode(controlId, imagePath, false, alpha);
    } else {
        node = (ImageNode*)match;
    }

	gameplay::Quaternion quaternion;
	gameplay::Quaternion::createFromEuler(MATH_DEG_TO_RAD(rotation.x),
										  MATH_DEG_TO_RAD(rotation.y),
										  MATH_DEG_TO_RAD(rotation.z), &quaternion);

	node->setTranslation(position);
	node->setRotation(quaternion);
	node->setScale(scale);

    if ( match == NULL )    {
        //trackingNode->addChild(node);
        cameraNode->addChild(node);
        SAFE_RELEASE(node);
    }
    //
    /////////////////////////////////
}

void MKitRendererManager::addImageLayout(char *layoutName, char *imagePath, float imageWidth, float imageHeight,
										 char * controlId, Vector3 position, Vector3 scale) {
	FormNode *formNode = new armakers::ImageLayoutFormNode(layoutName, imagePath, imageWidth, imageHeight, controlId);
    formNodeList.push_back(formNode);

    Vector3 formScale = Utilities::getScale(formNode->getFormBounds(), scale);
	Vector3 translation = Utilities::getTranslate(imageWidth, imageHeight, position);

//	LOGD("addImageLayout1 position : %f , %f , %f :: scale : %f , %f , %f", position.x, position.y, position.z, scale.x scale.y, scale.z);
	formNode->setTranslation(translation);
    formNode->setScale(formScale);

//	formNode->setTranslation(position);
//	formNode->setScale(scale);

//	LOGD("addImageLayout2 position : %f , %f , %f :: scale : %f , %f , %f", position.x, position.y, position.z, scale.x scale.y, scale.z);
    //trackingNode->addChild(formNode);
	//cameraNode->addChild(formNode);
	mainScene->addNode(formNode);

}

void MKitRendererManager::addImageLayoutChange(char *nodeId, char *layoutName, char *imagePath, float imageWidth, float imageHeight,
										 char * controlId, Vector3 position, Vector3 scale) {
	FormNode *formNode = new armakers::ImageLayoutFormNode(layoutName, imagePath, imageWidth, imageHeight, controlId);
	formNodeList.push_back(formNode);

	Vector3 formScale = Utilities::getScale(formNode->getFormBounds(), scale);
	Vector3 translation = Utilities::getTranslate(imageWidth, imageHeight, position);

	formNode->setTranslation(translation);
	formNode->setScale(formScale);

	LOGD("addImageLayoutChange nodeId : %s, layoutName : %s", formNode->getId(), layoutName);

	formNode->setId(nodeId);

	LOGD("addImageLayoutChange nodeId : %s, layoutName : %s", formNode->getId(), layoutName);
	trackingNode->addChild(formNode);
}

void MKitRendererManager::changeImageLayout(char *nodeId, char *layoutName, char *imagePath) {

	LOGD("MKitRendererManager changeImageLayout nodeId : %s, layoutName : %s", nodeId, layoutName);
    auto *changeImageLayoutFormNode = dynamic_cast<armakers::ImageLayoutFormNode *>(trackingNode->findNode(nodeId));

	LOGD("MKitRendererManager changeImageLayout : %s, isEnabled:%d", changeImageLayoutFormNode->getId(), changeImageLayoutFormNode->isEnabled());

	changeImageLayoutFormNode->setImagePath(imagePath);
}

void MKitRendererManager::addImageLayout1(char *layoutName, char *imagePath, float imageWidth, float imageHeight,
										  char * controlId, Vector3 position,Vector3 rotation, Vector3 scale) {
	FormNode *formNode = new armakers::ImageLayoutFormNode(layoutName, imagePath, imageWidth, imageHeight, controlId);
	formNodeList.push_back(formNode);

	Quaternion quaternion;
	Quaternion::createFromEuler(MATH_DEG_TO_RAD(rotation.x), MATH_DEG_TO_RAD(rotation.y),
			MATH_DEG_TO_RAD(rotation.z), &quaternion);

	Vector3 formScale = Utilities::getScale(formNode->getFormBounds(), scale);
	Vector3 translation = Utilities::getTranslate(imageWidth, imageHeight, position);

	formNode->setTranslation(translation);
	formNode->setRotation(quaternion);
	formNode->setScale(formScale);
	trackingNode->addChild(formNode);
}

void MKitRendererManager::addImagePagesLayoutAndAlpha(char *layoutName, char * controlId, std::vector<std::string> &lst,
													  float imageWidth, float imageHeight, Vector3 position,
													  Vector3 rotation, Vector3 scale, float alpha) {
	FormNode *formNode = new armakers::ImagePagesFormNode(layoutName, controlId, lst, imageWidth, imageHeight, true, alpha);
	formNodeList.push_back(formNode);

	Quaternion quaternion;
	Quaternion::createFromEuler(MATH_DEG_TO_RAD(rotation.x), MATH_DEG_TO_RAD(rotation.y),
								MATH_DEG_TO_RAD(rotation.z), &quaternion);

	formNode->setTranslation(position);
	formNode->setScale(scale);
	formNode->setRotation(quaternion);

	trackingNode->addChild(formNode);
}

void MKitRendererManager::changeImagePage(char *controlId, int opt) {

    auto *changeImageFormNode = dynamic_cast<armakers::ImagePagesFormNode *>(trackingNode->findNode(controlId));

    if (changeImageFormNode == nullptr) {
        LOGD("changeImagePage : 1st changeImageFormNode is null : %s", controlId);
        return;
    }
    changeImageFormNode->changeImagePage(opt);
}

void MKitRendererManager::addImageLayoutAndAlpha(char *layoutName, char * controlId, char *imagePath,
		float imageWidth, float imageHeight, Vector3 position,
		Vector3 rotation, Vector3 scale, float alpha) {

	FormNode *formNode = new armakers::ImageLayoutFormNode(layoutName, controlId, imagePath, imageWidth, imageHeight, true, alpha);
	formNodeList.push_back(formNode);

	Quaternion quaternion;
	Quaternion::createFromEuler(MATH_DEG_TO_RAD(rotation.x), MATH_DEG_TO_RAD(rotation.y),
			MATH_DEG_TO_RAD(rotation.z), &quaternion);

	formNode->setTranslation(position);
	formNode->setScale(scale);
	formNode->setRotation(quaternion);

	trackingNode->addChild(formNode);
}

void MKitRendererManager::addVideo(std::shared_ptr<VideoPlayer> videoPlayer, Vector3 position,
								   Vector3 rotation, Vector3 scale) {
	const char *nodeId = videoPlayer->getVideoName();
	LOGD("Video node id : %s", nodeId);
	VideoNode *node = new VideoNode(nodeId, videoPlayer);
	Quaternion quaternion;
	Quaternion::createFromEuler(MATH_DEG_TO_RAD(rotation.x), MATH_DEG_TO_RAD(rotation.y),
			MATH_DEG_TO_RAD(rotation.z), &quaternion);

	node->setTranslation(position);
	node->setRotation(quaternion);
	node->setScale(scale);
	trackingNode->addChild(node);
	SAFE_RELEASE(node);
}

void MKitRendererManager::addVideoAndAlpha(std::shared_ptr<VideoPlayer> videoPlayer, char *controlId, Vector3 position,
                                   Vector3 rotation, Vector3 scale, float alpha) {
    LOGD("Video node id : %s", controlId);
    VideoNode *node = new VideoNode(controlId, videoPlayer, alpha);
    Quaternion quaternion;
    Quaternion::createFromEuler(MATH_DEG_TO_RAD(rotation.x), MATH_DEG_TO_RAD(rotation.y),
                                          MATH_DEG_TO_RAD(rotation.z), &quaternion);

    node->setTranslation(position);
    node->setRotation(quaternion);
    node->setScale(scale);
    trackingNode->addChild(node);
    SAFE_RELEASE(node);
}

void MKitRendererManager::addVideoLayout(char * controlId, std::shared_ptr<VideoPlayer> videoPlayer, Vector3 position,
										 Vector3 rotation, Vector3 scale) {
	const char *nodeId = videoPlayer->getVideoName();
	LOGD("Video node id : %s", nodeId);
	VideoLayoutFormNode *formNode = new VideoLayoutFormNode(controlId, videoPlayer);

	Quaternion quaternion;
	Quaternion::createFromEuler(MATH_DEG_TO_RAD(rotation.x), MATH_DEG_TO_RAD(rotation.y),
										  MATH_DEG_TO_RAD(rotation.z), &quaternion);

	Vector3 formScale = Utilities::getScale(formNode->getFormBounds(), scale);
	Vector3 translation = Utilities::getTranslate(640, 480, position);

//	formNode->setTranslation(position);
	formNode->setTranslation(translation);
	formNode->setScale(formScale);
	formNode->setRotation(quaternion);

	trackingNode->addChild(formNode);
	SAFE_RELEASE(formNode);

}

void MKitRendererManager::addWebViewAndAlpha(std::shared_ptr<armakers::WebView> webView, Vector3 position,
										   Vector3 rotation, Vector3 scale, float alpha) {
	const char *nodeId = webView->getName();
	LOGD("WebView node id : %s", nodeId);
    armakers::WebViewNode *node = new armakers::WebViewNode(nodeId, webView, alpha);

	Quaternion quaternion;
	Quaternion::createFromEuler(MATH_DEG_TO_RAD(rotation.x), MATH_DEG_TO_RAD(rotation.y),
										  MATH_DEG_TO_RAD(rotation.z), &quaternion);

	node->setTranslation(position);
	node->setRotation(quaternion);
	node->setScale(scale);
	trackingNode->addChild(node);
	SAFE_RELEASE(node);
}

void MKitRendererManager::addGlobalMenuLayout(char * controlId, Vector3 position, Vector3 rotation,
		Vector3 scale, float alpha) {
	auto *formNode = new armakers::GlobalMenuLayoutFormNode((char *)"globalMenu", controlId, alpha);

	Quaternion quaternion;
	Quaternion::createFromEuler(MATH_DEG_TO_RAD(rotation.x),
										  MATH_DEG_TO_RAD(rotation.y),
										  MATH_DEG_TO_RAD(rotation.z), &quaternion);

	Rectangle rect = formNode->getFormBounds();
	Vector3 formScale = scale;//Utilities::getScale2(rect, scale);
	Vector3 translation = position;// = Utilities::getTranslate(rect.width, rect.height, position);

	formNode->setTranslation(translation);
	formNode->setScale(formScale);
	formNode->setRotation(quaternion);

	trackingNode->addChild(formNode);
	SAFE_RELEASE(formNode);
}

void MKitRendererManager::showGlobalMenuLayout(char * controlId, Vector3 position, Vector3 rotation, Vector3 scale, float alpha) {
    if (globalMenuNode == nullptr) {
        auto *formNode = new armakers::GlobalMenuLayoutFormNode((char *)"globalMenu2", controlId, alpha);

        Vector3 cameraTranslation = cameraNode->getTranslation();
        //scale
        float distance = position.distance(cameraTranslation);
        Rectangle rect = formNode->getFormBounds();
        Vector3 formScale = scale;// Utilities::getScale4Form2(rect, scale, distance);
        //position
        Vector3 translation;
        Quaternion cameraQuaternion = cameraNode->getRotation();
        cameraQuaternion.rotatePoint(position, &translation);
        //rotation
        Quaternion quaternion;
        Vector3 rotate(0.F, 0.F, 0.F);
        cameraQuaternion.computeEuler(&rotate.x, &rotate.y, &rotate.z);
        rotate.x = MATH_RAD_TO_DEG(rotate.x);
        rotate.y = MATH_RAD_TO_DEG(rotate.y);
        rotate.z = MATH_RAD_TO_DEG(rotate.z);
//        rotate.add(gameplay::Vector3(180.F, 0.F, 0.F));
        rotation.add(rotate);
        Quaternion::createFromEuler(MATH_DEG_TO_RAD(rotation.x), MATH_DEG_TO_RAD(rotation.y),
                                              MATH_DEG_TO_RAD(rotation.z), &quaternion);

        formNode->setTranslation(translation);
        formNode->setScale(formScale);
        formNode->setRotation(quaternion);
        mainScene->addNode(formNode);
        globalMenuNode = formNode;
        SAFE_RELEASE(formNode);
    }
}

void MKitRendererManager::hideGlobalMenuLayout() {
    if (globalMenuNode) {
        mainScene->removeNode(globalMenuNode);
        SAFE_RELEASE(globalMenuNode);
        globalMenuNode = nullptr;
    }
}

void MKitRendererManager::setGlobalMenuBadgeText(char *controlId, char *text) {
    auto *formNode = dynamic_cast<armakers::GlobalMenuLayoutFormNode*>(trackingNode->findNode(controlId));
    if (formNode != nullptr) {
        formNode->setBadgeText(text);
    }
}

void MKitRendererManager::setGlobalMenuBadgeVisible(char *controlId, bool visible) {
    auto *formNode = dynamic_cast<armakers::GlobalMenuLayoutFormNode*>(trackingNode->findNode(controlId));
    if (formNode != nullptr) {
		formNode->setVisibleBadge(visible);
    }
}

void MKitRendererManager::addSystemLayoutAndAlpha(std::shared_ptr<armakers::SystemLayout> layout, Vector3 position,
												  Vector3 rotation, Vector3 scale, float alpha) {
	//const char *nodeId = layout->getName();
	//LOGD("SystemLayout node id : %s", nodeId);
	LOGD("addSystemLayoutAndAlpha alpha: (%f)", alpha);
	armakers::SystemLayoutFormNode *formNode = new armakers::SystemLayoutFormNode(layout, alpha, "systemLayout");

    Vector3 cameraTranslation = cameraNode->getTranslation();
    //scale
    float distance = position.distance(cameraTranslation);
    Rectangle rect = formNode->getFormBounds();
    Vector3 formScale = Utilities::getScale4Form(rect, scale, distance);

    //position
    Vector3 translation;
    Quaternion cameraQuaternion = cameraNode->getRotation();
    cameraQuaternion.rotatePoint(position, &translation);

    //rotation
    Quaternion quaternion;
    Vector3 rotate;
    Vector3 rotate2;
    Vector3::cross(translation, cameraTranslation, &rotate);
    cameraQuaternion.computeEuler(&rotate2.x, &rotate2.y, &rotate2.z);
    rotate.add(rotate2);
    Quaternion::createFromEuler(rotate.x, rotate.y, rotate.z, &quaternion);

    //form position transfer
    Vector3 formTranslation(-rect.width * 0.02F / distance,
                            -rect.height * 0.02F / distance, 0.F);
    Vector3 formTranslation2;
    quaternion.rotatePoint(formTranslation, &formTranslation2);
    translation.add(formTranslation2);

    formNode->setTranslation(translation);
    formNode->setRotation(quaternion);
    formNode->setScale(formScale);
	layoutNodeMap.insert(std::make_pair(formNode->getName(), formNode));
    mainScene->addNode(formNode);
}

void MKitRendererManager::removeSystemLayout(const char* name) {
    auto findIter = layoutNodeMap.find(name);
    if (findIter != layoutNodeMap.end()) {
		armakers::SystemLayoutFormNode *formNode = findIter->second;
        mainScene->removeNode(formNode);
        SAFE_RELEASE(formNode);
        layoutNodeMap.erase(name);
    }
}

void MKitRendererManager::showSystemLayoutAndAlpha(std::shared_ptr<armakers::SystemLayout> layout, Vector3 position,
												  Vector3 rotation, Vector3 scale, float alpha) {
	armakers::SystemLayoutFormNode *formNode = new armakers::SystemLayoutFormNode(layout, alpha, "systemLayout");
	formNode->setId(layout->getName());

	Quaternion quaternion;
	Quaternion::createFromEuler(MATH_DEG_TO_RAD(rotation.x), MATH_DEG_TO_RAD(rotation.y),
										  MATH_DEG_TO_RAD(rotation.z), &quaternion);

	Vector3 formScale = scale;//Utilities::getScale2(rect, scale);
	Vector3 translation = position;// = Utilities::getTranslate(rect.width, rect.height, position);

	formNode->setTranslation(translation);
	formNode->setScale(formScale);
	formNode->setRotation(quaternion);

	trackingNode->addChild(formNode);
	SAFE_RELEASE(formNode);
}

bool MKitRendererManager::hasSystemLayout(const char* name) {
    return layoutNodeMap.find(name) != layoutNodeMap.end();
}

void MKitRendererManager::addARComplexViewer(std::string &nodeId, std::string &controlId,
        std::shared_ptr<armakers::ARComplexViewer> viewer) {
	LOGD("addARComplexViewer");
    armakers::ARComplexViewerNode *node = new armakers::ARComplexViewerNode(nodeId, controlId, viewer);

	Quaternion quaternion;
	Quaternion::createFromEuler(MATH_DEG_TO_RAD(viewer->getRotation().x), MATH_DEG_TO_RAD(viewer->getRotation().y),
										  MATH_DEG_TO_RAD(viewer->getRotation().z), &quaternion);

#if defined(FEATURE_FIX_RENDER_SIZE)
	viewer->getPos().add(Vector3(0.3F, 0.F, 0.F));
	node->setTranslation(viewer->getPos());
#else
	node->setTranslation(viewer->getPos());
#endif
	node->setRotation(quaternion);
	node->setScale(viewer->getScale());
#if defined(FEATURE_FIX_RENDER_SIZE)
    simpleTrackingNode->addChild(node);
#else
	trackingNode->addChild(node);
#endif
	SAFE_RELEASE(node);
}

void MKitRendererManager::changeContextMenu(std::string &nodeId, int32_t menuType) {
#if defined(FEATURE_FIX_RENDER_SIZE)
    Node *fNode = simpleTrackingNode->findNode(nodeId.c_str());
#else
    Node *fNode = trackingNode->findNode(nodeId.c_str());
#endif
    if (!fNode) { LOGE("changeContextMenu node not found"); return; }
    armakers::ARComplexViewerNode *node = dynamic_cast<armakers::ARComplexViewerNode*>(fNode);
    if (!node) { LOGE("changeContextMenu casting FAIL!!"); return; }
    node->processChangeNode((armakers::ARContextMenuType)menuType);
}

void MKitRendererManager::setContextMenuBadge(std::string &nodeId, int32_t count) {
#if defined(FEATURE_FIX_RENDER_SIZE)
    Node *fNode = simpleTrackingNode->findNode(nodeId.c_str());
#else
    Node *fNode = trackingNode->findNode(nodeId.c_str());
#endif
	if (!fNode) { LOGE("setContextMenuBadge node not found"); return; }
	armakers::ARComplexViewerNode *node = dynamic_cast<armakers::ARComplexViewerNode*>(fNode);
	if (!node) { LOGE("setContextMenuBadge casting FAIL!!"); return; }
	node->setContextMenuBadge(count);
}

void MKitRendererManager::changeItemIndex(std::string &nodeId, int32_t itemIndex) {
#if defined(FEATURE_FIX_RENDER_SIZE)
    Node *fNode = simpleTrackingNode->findNode(nodeId.c_str());
#else
    Node *fNode = trackingNode->findNode(nodeId.c_str());
#endif
    if (!fNode) { LOGE("changeItemIndex node not found"); return; }
    armakers::ARComplexViewerNode *node = dynamic_cast<armakers::ARComplexViewerNode*>(fNode);
    if (!node) { LOGE("changeItemIndex casting FAIL!!"); return; }
    node->processChangeTemplateItem(itemIndex);
}

void MKitRendererManager::changePage(std::string &nodeId, int32_t op, int32_t pageNo) {
#if defined(FEATURE_FIX_RENDER_SIZE)
    Node *fNode = simpleTrackingNode->findNode(nodeId.c_str());
#else
    Node *fNode = trackingNode->findNode(nodeId.c_str());
#endif
    if (!fNode) { LOGE("changePage node not found"); return; }
    armakers::ARComplexViewerNode *node = dynamic_cast<armakers::ARComplexViewerNode*>(fNode);
    if (!node) { LOGE("changePage casting FAIL!!"); return; }
    node->processChangeTemplatePage(op, pageNo);
}

void MKitRendererManager::changeMediaPlayer(std::string &nodeId, int32_t op) {
#if defined(FEATURE_FIX_RENDER_SIZE)
    Node *fNode = simpleTrackingNode->findNode(nodeId.c_str());
#else
    Node *fNode = trackingNode->findNode(nodeId.c_str());
#endif
	if (!fNode) { LOGE("changeMediaPlayer node not found"); return; }
    armakers::ARComplexViewerNode *node = dynamic_cast<armakers::ARComplexViewerNode*>(fNode);
	if (!node) { LOGE("changeMediaPlayer casting FAIL!!"); return; }
	node->processChangeTemplateMediaPlayer(op);
}

void MKitRendererManager::setEnableContextMenu(std::string &nodeId, bool enable) {
#if defined(FEATURE_FIX_RENDER_SIZE)
    Node *fNode = simpleTrackingNode->findNode(nodeId.c_str());
#else
    Node *fNode = trackingNode->findNode(nodeId.c_str());
#endif
	if (!fNode) { LOGE("changeMediaPlayer node not found"); return; }
	armakers::ARComplexViewerNode *node = dynamic_cast<armakers::ARComplexViewerNode*>(fNode);
	if (!node) { LOGE("changeMediaPlayer casting FAIL!!"); return; }
	node->setContextMenuEnable(enable);
}

int32_t MKitRendererManager::getCurrentComplexMenu(std::string &nodeId) {
#if defined(FEATURE_FIX_RENDER_SIZE)
    Node *fNode = simpleTrackingNode->findNode(nodeId.c_str());
#else
    Node *fNode = trackingNode->findNode(nodeId.c_str());
#endif
	if (!fNode) { LOGE("changeContextMenu node not found"); return armakers::ARContextMenuType::UNKNOWN_MENU; }
	armakers::ARComplexViewerNode *node = dynamic_cast<armakers::ARComplexViewerNode*>(fNode);
	if (!node) { LOGE("changeContextMenu casting FAIL!!"); return armakers::ARContextMenuType::UNKNOWN_MENU; }

	return node->getCurrentMenuType();
}

int32_t MKitRendererManager::getComplexContentPage(std::string &nodeId) {
#if defined(FEATURE_FIX_RENDER_SIZE)
    Node *fNode = simpleTrackingNode->findNode(nodeId.c_str());
#else
    Node *fNode = trackingNode->findNode(nodeId.c_str());
#endif
	if (!fNode) { LOGE("changeContextMenu node not found"); return -1; }
	armakers::ARComplexViewerNode *node = dynamic_cast<armakers::ARComplexViewerNode*>(fNode);
	if (!node) { LOGE("changeContextMenu casting FAIL!!"); return -1; }

	return node->getCurrentViewerPage();
}

void MKitRendererManager::show3DToast(std::string &msg, int32_t ms, int32_t type) {
    if (wpToastNode) wpToastNode->process(std::shared_ptr<armakers::ToastMessage>(
            new armakers::ToastMessage(msg, ms, (armakers::ToastType)type))
            );
}

void MKitRendererManager::applyCheckState(char *checkControlId, bool isChecked) {
	LOGD("checkControlId : %s, isChecked:%d", checkControlId, isChecked);
	for (auto &it : formNodeList) {
		it->applyCheckState(checkControlId, isChecked);
	}
}

void MKitRendererManager::addMainControlMenu(std::shared_ptr<armakers::SystemLayout> layout,
										gameplay::Vector3 position, gameplay::Vector3 rotation,
										gameplay::Vector3 scale, float alpha) {

	if (wpMainControlNode) {
		armakers::SystemLayoutFormNode *formNode = new armakers::SystemLayoutFormNode(layout, alpha, "mainHeaderLayout");
		formNode->setId(layout->getName());
		gameplay::Rectangle rect = formNode->getFormBounds();
		layout->setSize(rect.width, rect.height);

		wpMainControlNode->setFormNode(formNode);
	}
}

void MKitRendererManager::removeMainControlMenu() {
	if (wpMainControlNode) {
		wpMainControlNode->removeFormNode();
	}
}

void MKitRendererManager::addCustomViewLayout(std::shared_ptr<armakers::SystemLayout> layout,
        std::string& nodeID, int *size, const gameplay::Vector3 &position,
        const gameplay::Vector3 &rotation, const gameplay::Vector3 &scale, float alpha) {
	auto *temp = cameraNode->findNode(nodeID.c_str());
	if (temp) {
		return;
	}

    Quaternion quaternion;
    Quaternion::createFromEuler(MATH_DEG_TO_RAD(rotation.x), MATH_DEG_TO_RAD(rotation.y),
                                MATH_DEG_TO_RAD(-90 + rotation.z), &quaternion);


    armakers::SystemLayoutNode *layoutNode = new armakers::SystemLayoutNode(nodeID.c_str(), layout,
    		size[0], size[1], alpha);

	layoutNode->setTranslation(position);
	layoutNode->setRotation(quaternion);
	layoutNode->setScale(scale);

    cameraNode->addChild(layoutNode);
    SAFE_RELEASE(layoutNode);
}

void MKitRendererManager::removeCustomViewLayout(std::string& nodeID) {
	auto *layoutNode = cameraNode->findNode(nodeID.c_str());
	if (layoutNode) {
		cameraNode->removeChild(layoutNode);
	}
}

void MKitRendererManager::addFixedSystemLayout(std::shared_ptr<armakers::SystemLayout> layout, char *systemFormName, Vector3 position, float alpha) {
	armakers::SystemLayoutFormNode *formNode = new armakers::SystemLayoutFormNode(layout, alpha, systemFormName);
	formNode->setId(layout->getName());
	gameplay::Rectangle rect = formNode->getFormBounds();

	FormUtility::translate2Center(formNode, false);
	formNode->setTranslation(position);

	cameraNode->addChild(formNode);

	SAFE_RELEASE(formNode);
}

void MKitRendererManager::addFixedSharedImage(char *layoutName, char *controlId, char *imagePath,
		int imageWidth, int imageHeight, bool isSmall, float alpha) {
	armakers::ImageLayoutFormNode *formNode = new armakers::ImageLayoutFormNode(layoutName,
			controlId, imagePath, imageWidth, imageHeight, true, alpha);

	FormUtility::translate2Center(formNode);
	if (isSmall) {
		gameplay::Rectangle bound = formNode->getFormBounds();
		gameplay::Vector3 pos = Vector3(-0.49f, 0.16f, 1.f);
		formNode->setTranslation(pos);
	} else {
		gameplay::Vector3 trans = formNode->getTranslation();
		trans.z = 1.f;
		formNode->setTranslation(trans);
	}

	cameraNode->addChild(formNode);

	SAFE_RELEASE(formNode);
}

void MKitRendererManager::removeFixedNode(char * controlId) {
	Node * node = cameraNode->findNode(controlId);
	if (node != nullptr) {
		cameraNode->removeChild(node);
	}
}

void MKitRendererManager::targetOnEnabled(bool enable) {
    if (wpTargetOnNode) {
        wpTargetOnNode->processEnabled(enable);
    }
}

void MKitRendererManager::captureScene(bool toggle) {
	LOGD("MKitRendererManager::captureScene");
    //isSceneCaptured = toggle;
	spRenderer->setCaptures();
}

void MKitRendererManager::addNode(char *name) {
	auto it = trackableNodeMap->find(name);
	trackingNode->addChild(it->second);
}

void MKitRendererManager::addNodeMap(char *nodeName, bool isChecked) {
	if(isChecked) trackableNodeMap->insert(make_pair(nodeName, checkNode));
	else trackableNodeMap->insert(make_pair(nodeName, itemNode));
}

void MKitRendererManager::createNode(char *nodeName, bool isChecked) {
	if(isChecked) checkNode = Node::create(nodeName);
	else itemNode = Node::create(nodeName);
}

void MKitRendererManager::keyEvent(Keyboard::KeyEvent evt, int key) {
}

#ifdef MAXST_CUSTOMIZE

#ifdef WIN32
using namespace gameplay;

#ifndef _WINDOWS_
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

/**
 * Main entry point.
 */
extern "C" int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmdLine, int cmdShow)
{
	Game* game = new MKitRendererManager();
	Platform* platform = Platform::create(game, "../../../../GameplayRes/"); // Don't miss the last slash!!
	GP_ASSERT(platform);
	int result = platform->enterMessagePump();
	delete platform;

	if (game != nullptr)
	{
		delete game;
		game = nullptr;
	}
	return result;
}
#endif
#endif 