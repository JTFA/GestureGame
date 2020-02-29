#pragma once

#include <gameplay.h>

#include <MaxstAR.h>
#include <CameraDevice.h>
#include <TrackerManager.h>
#include <BackgroundCameraQuad.h>
#include <memory>
#include <vector>
#include <map>
#include <string>
#include <list>
#include <mutex>
#include <SearchingIndicatorNode.h>
#include <FormNode.h>
#include <VideoPlayer.h>
#include <control/WebView.h>
#include <VideoNode.h>
#include <ImageNode.h>
#include <MonoRenderer.h>

#include <control/SystemLayout.h>
#include <node/WebViewNode.h>
#include <node/SystemLayoutFormNode.h>
#include <model/ARComplexViewer.h>
#include <node/ARToastNode.h>
#include <node/ARMainControlNode.h>
#include <node/TargetOnFormNode.h>

namespace gameplay {
/**
 * Main game class.
 */
	class MKitRendererManager : public Game {
	public:

		/**
         * Constructor.
         */
		MKitRendererManager();

		~MKitRendererManager();

		/**
         * 모든 증강 Node 삭제
         */
		void clearContent();

		/**
         * controlId에 해당하는 증강 Node 삭제
         */
		void clearContent(char *controlId);

		/**
         * Called when surface resized
         * @param width : Surface width
         * @param height : Surface height
         */
		void resizeEvent(unsigned int width, unsigned int height);

		void onStarted();

		/**
         * @see Game::keyEvent
         */
		void keyEvent(Keyboard::KeyEvent evt, int key);

		/*
        ** Set projection matrix
        */
		void setProjectionMatrix(float *matrix);

		/**
        ** Set tracking result
        */
		void setTrackingResult(bool isSuccess, char *trackableId = nullptr,
							   char *trackableName = nullptr, float *poseMatrix = nullptr);

		/**
        ** Set camera Quaternion
        */
		void setCameraQuaternion(float *quaternion);

		/**
         * add form
         */
		void addLayout1(char *layoutName, char *imageTextPath, char *checkControlId,
						char *buttonControlId, Vector3 position, Vector3 scale,
						float width, float height);

		void addChildLayout(char *layoutName, char *imageTextPath, char *checkControlId,
							char *buttonControlId, Vector3 position, Vector3 scale,
							float width, float height, bool isChild);

		void addLayout2(char *layoutName, char *imageTextPath, char *checkControlId,
						char *buttonControlId, Vector3 position, Vector3 scale);

		void addLayout3(char *layoutName, char *checkControlId, char *buttonControlId,
		        Vector3 position, Vector3 scale);

		void addIconLayout(char *layoutName, char *iconPath, Vector3 position, Vector3 scale,
						   float width, float height);

		/*
        ** 이미지로 저장된 텍스트용 레이아웃 추가
        */
		void addTextLayout(char *layoutName, char *textPath, char *controlId,
						   Vector3 position, Vector3 scale,
						   float width, float height);

		void
		addTagLayout(char *layoutName, char *imageTextPath, char *buttonControlId, Vector3 position,
					 Vector3 scale, float width, float height);

		void addNode(char *name);

		void createNode(char *nodeName, bool isChecked);

		void addNodeMap(char *nodeName, bool isChecked);

		/**
         * Add augmented image
         * @param name : node name
         * @param imagePath : path image
         * @param position : position
         * @param rotation : rotation
         * @param scale : scale
         */
		void addImage(const char *name, const char *imagePath, Vector3 position, Vector3 rotation,
					  Vector3 scale, bool modulateAlpha);

		void addImageAndAlpha(const char *controlId, const char *imagePath, Vector3 position,
							  Vector3 rotation, Vector3 scale, float alpha);

		void addImageLayout(char *layoutName, char *imagePath, float imageWidth, float imageHeight,
							char *controlId, Vector3 position, Vector3 scale);

		void addImagePagesLayoutAndAlpha(char *layoutName, char * controlId, std::vector<std::string> &lst,
										 float imageWidth, float imageHeight, Vector3 position,
										 Vector3 rotation, Vector3 scale, float alpha);
		void changeImagePage(char *controlId, int opt);

		void addImageLayout1(char *layoutName, char *imagePath, float imageWidth, float imageHeight,
							 char *controlId, Vector3 position, Vector3 rotation, Vector3 scale);

		void addImageLayoutAndAlpha(char *layoutName, char *controlId, char *imagePath, float imageWidth,
							   float imageHeight, Vector3 position, Vector3 rotation, Vector3 scale,
							   float alpha);

		void addImageLayoutChange(char *nodeId, char *layoutName, char *imagePath, float imageWidth,
								  float imageHeight, char *controlId, Vector3 position,
								  Vector3 scale);

		void changeImageLayout(char *nodeId, char *layoutName, char *imagePath);

		/**
         * Add augmented video player
         * @param videoPlayer : VideoPlayer
         * @param position : position
         * @param rotation : rotation
         * @param scale : scale
         */
		void addVideo(std::shared_ptr<VideoPlayer> videoPlayer, Vector3 position, Vector3 rotation,
					  Vector3 scale);

		void addVideoAndAlpha(std::shared_ptr<VideoPlayer> videoPlayer, char *controlId,
							  Vector3 position, Vector3 rotation, Vector3 scale, float alpha);

		void addWebViewAndAlpha(std::shared_ptr<armakers::WebView> webView, Vector3 position,
		        Vector3 rotation, Vector3 scale, float alpha);

		/**
             * Add augmented video player to form node
             * @param controlId : Java id
             * @param videoPlayer : VideoPlayer
             * @param position : position
             * @param rotation : rotation
             * @param scale : scale
             */
		void
		addVideoLayout(char *controlId, std::shared_ptr<VideoPlayer> videoPlayer, Vector3 position,
					   Vector3 rotation, Vector3 scale);

		/**
         * add Global Menu
         */

		void addGlobalMenuLayout(char *controlId, Vector3 position, Vector3 rotation, Vector3 scale,
								 float alpha);

		void showGlobalMenuLayout(char *controlId, Vector3 position, Vector3 rotation, Vector3 scale,
							 float alpha);

		void hideGlobalMenuLayout();

		void setGlobalMenuBadgeText(char *controlId, char *text);

		void setGlobalMenuBadgeVisible(char *controlId, bool visible);

		/*
         * android system UI
         */
		void addSystemLayoutAndAlpha(std::shared_ptr<armakers::SystemLayout> layout, Vector3 position,
									 Vector3 rotation, Vector3 scale, float alpha);

		void removeSystemLayout(const char *name);

		bool hasSystemLayout(const char *name);

		void showSystemLayoutAndAlpha(std::shared_ptr<armakers::SystemLayout> layout, Vector3 position,
									  Vector3 rotation, Vector3 scale, float alpha);

		/**
         * Contents Template
         */
		void addContentsTemplate(char *nodeId, char *controlId, Vector3 position, Vector3 rotation,
								 Vector3 scale, float alpha);

		void addARComplexViewer(std::string &nodeId, std::string &controlId,
								std::shared_ptr<armakers::ARComplexViewer> viewer);

		void changeContextMenu(std::string &nodeId, int32_t menuType);
		void setContextMenuBadge(std::string &nodeId, int32_t count);

		void changeItemIndex(std::string &nodeId, int32_t itemIndex);

		void changePage(std::string &nodeId, int32_t op, int32_t pageNo);

		void changeMediaPlayer(std::string &nodeId, int32_t op);

		void setEnableContextMenu(std::string &nodeId, bool enable);

		int32_t getCurrentComplexMenu(std::string &nodeId);

		int32_t getComplexContentPage(std::string &nodeId);

		void show3DToast(std::string &msg, int32_t ms, int32_t type);

		/**
         * Change check state of specified control id
         * @param checkControlId : CheckBox control id
         * @param isChecked : check or not
         */
		void applyCheckState(char *checkControlId, bool isChecked);

		/**
         * Capture AR scene
         */
		void captureScene(bool toggle);

		void addTrackableNode(char *equipmentId);

		bool checkEquipmentId(char *equipmentId);

		void addTrackableMap(char *equipmentId, char *trackableId);

		void addMainControlMenu(std::shared_ptr<armakers::SystemLayout> layout, Vector3 position,
						   Vector3 rotation, Vector3 scale, float alpha);

		void removeMainControlMenu();

		void addFixedSystemLayout(std::shared_ptr<armakers::SystemLayout> layout, char *systemFormName, Vector3 position, float alpha);
		void addFixedSharedImage(char *layoutName, char *controlId, char *imagePath, int imageWidth, int imageHeight, bool isSmall, float alpha);
		void removeFixedNode(char * controlId);

		void targetOnEnabled(bool enable);
        /**
         * @see renderer rect size
         */
        void changeRendererSize(int32_t lw, int32_t lh, int32_t rw,int32_t rh);

		void addCustomViewLayout(std::shared_ptr<armakers::SystemLayout> layout, std::string &nodeID,
							int *size, const gameplay::Vector3 &position,
							const gameplay::Vector3 &rotation, const gameplay::Vector3 &scale,
							float alpha);

		void removeCustomViewLayout(std::string& nodeID);

	protected:

		/**
         * @see Game::initialize
         */
		void initialize();

		/**
         * @see Game::finalize
         */
		void finalize();

		/**
         * @see Game::update
         */
		void update(float elapsedTime);

		/**
         * @see Game::render
         */
		void render(float elapsedTime);

    private:
        void updateRendererType(RendererType type);

		/**
         * Node 와 Node 의 Children node 의 상태를 변경한다.
         * @param node : Target node
         * @param enabled : true - active, false - inactive
         */
		void setEnableNode(Node *node, bool enabled);

		/**
         * Children node 의 상태를 변경한다.
         * @param node : Parent node
         * @param enabled : true - active, false - inactive
         */
		void setEnableChildrenNode(Node *node, bool enabled);

		BackgroundCameraQuad *backgroundCameraQuad;

		Animation *animation;

		Scene *mainScene;
		Node *cameraNode;
		Node *trackingNode;
		Node *simpleTrackingNode;
		Node *itemNode;
		Node *checkNode;
		Node *currentNode;
		bool showWireFrame;
		Node *globalMenuNode;
        armakers::ARToastNode *wpToastNode;
        armakers::ARMainControlNode *wpMainControlNode;
        armakers::TargetOnNode *wpTargetOnNode;

		bool isTrackingSuccess = false;
		std::string trackableId;
		std::string trackableName;
		float projectionMatrix[16];
		float poseMatrix[16];

		/**
         * 모든 FormNode 저장. Form 안에 있는 CheckBox 나 Button 등을 빠르게 찾아내기 위한 목적
         */
		std::list<FormNode *> formNodeList;

		std::list<string> equipmentList;

		map<string, Node *> *trackableNodeMap;

		std::map<string, armakers::SystemLayoutFormNode *> layoutNodeMap;

		bool isSceneCaptured = false;
		Vector3 scale;
		Quaternion rotation;
		Vector3 translation;

		Vector3 fixedScale;
        Quaternion fixedRotation;

		Quaternion sensorRotation;
		std::mutex sensorMutex;

		RendererType rendererType;
		std::shared_ptr<IRenderer> spRenderer;
	};
}