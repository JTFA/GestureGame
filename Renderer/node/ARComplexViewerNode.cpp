#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG "FormNode"
#endif

#include "Label.h"
#include <LogUtils.h>

#include <MKitHelper.h>
#include <Utilities.h>

#include <utility/FormUtility.h>
#include <node/ARComplexViewerNode.h>


static const char * contextMenuForm = "contextMenu2";
static const char * formName = "contentsTemplate";
static const char * contentNodeName = "contentsNode";
static const char * contentBgNodeName = "contentBgNode";
static const char * contentBgFileName = "res/hdpi-png/bg_black.png";

static const char * ARCallFormName = "ARCallLayout";
static const char * FacilityWorkFormName = "FacilityWorkLayout";
static const char * FacilityMemoFormName = "FacilityMemoLayout";

#define CONTEXT_MUNE_BOTTOM_PADDING 20

using namespace armakers;

ARComplexViewerNode::~ARComplexViewerNode() {
    for (int type = AR_CALL_MENU; type < CLOSE_MENU; ++type)
    {
        removeNode((ARContextMenuType)type);
    }
}

ARComplexViewerNode::ARComplexViewerNode(std::string &nodeId, std::string &controlId,
        std::shared_ptr<ARComplexViewer> viewer) : gameplay::Node(nodeId.c_str()) {
    this->controlId = controlId;
    this->viewer = viewer;
    arCallNode = nullptr;

    formNode = nullptr;
    contentNode = nullptr;
    refVideoNode = nullptr;
    contentBgNode = nullptr;

    facilityWorkNode = nullptr;
    facilityMemoNode = nullptr;

    loadContextMenu();
    addContextMenuNode();

    int32_t menuType = viewer->getCurrentMenuType();
    viewer->setCurrentMenuType(UNKNOWN_MENU);
    processChangeNode((ARContextMenuType)menuType);
}

void ARComplexViewerNode::processChangeNode(ARContextMenuType type) {
    //LOGD("ARComplexViewerNode processChangeNode type(%d)", type);
    if (viewer->getCurrentMenuType() == type) return;

    removeNode((ARContextMenuType)viewer->getCurrentMenuType());
    viewer->setCurrentMenuType(type);

    switch (type) {
        case ARContextMenuType::AR_CALL_MENU:
            loadARCall();
            break;
        case ARContextMenuType::DATA_VIEWER_MENU:
            loadTemplateForm();
            loadTemplateQuad();
            break;
        case ARContextMenuType::FACILITY_WORK_MENU:
            loadFacilityWork();
            break;
        case ARContextMenuType::FACILITY_MEMO_MENU:
            loadFacilityMemo();
            break;
        default:
            break;
    }

    FormUtility::translate2Center(contextMenuNode);
    addContextItemNode();
    addTranslateContextMenu();
}

void ARComplexViewerNode::setContextMenuBadge(int count) {

    if (contextMenuNode != nullptr) {
        if ( count > 0 )
            contextMenuNode->setVisibleBadge(true);
        else
            contextMenuNode->setVisibleBadge(false);
    }
}

void ARComplexViewerNode::processChangeTemplateItem(int index) {
    //LOGD("ARComplexViewerNode processChangeTemplateItem index(%d)", index);
    if (viewer->getCurrentMenuType() != ARContextMenuType::DATA_VIEWER_MENU) {
        //LOGE("ARComplexViewerNode processChangeTemplateItem type(%d)", viewer->getCurrentMenuType());
        return;
    }

    removeNode(ARContextMenuType::DATA_VIEWER_MENU);
    viewer->setCurrentMenuType(ARContextMenuType::UNKNOWN_MENU);

    std::shared_ptr<ARContentList> contentList = std::static_pointer_cast<ARContentList>(
            viewer->getItems()[ARContextMenuType::DATA_VIEWER_MENU]);
    if (contentList->getItems().size() > index) {
        contentList->setCurrentItemIndex(index);
        processChangeNode(ARContextMenuType::DATA_VIEWER_MENU);
    } else {
        LOGE("ARComplexViewerNode index(%d) error!!", index);
    }
}

void ARComplexViewerNode::processChangeTemplatePage(int32_t op, int32_t pageNo) {
    if (viewer->getCurrentMenuType() != ARContextMenuType::DATA_VIEWER_MENU) {
        return;
    }

    std::shared_ptr<ARContentList> spContentList = std::static_pointer_cast<ARContentList>(
            viewer->getItems()[ARContextMenuType::DATA_VIEWER_MENU]);
    std::shared_ptr<ARContentItem> spARContentItem = spContentList->getCurrentItem();
    int32_t index = spARContentItem->getCurrentIndex();
    bool isChange = false;
    switch (op) {
        case ARContentPageOP::PAGE_PREV_OP:
            if (index > 0) {
                spARContentItem->setCurrentIndex(--index);
                isChange = true;
            }
            break;
        case ARContentPageOP::PAGE_NEXT_OP:
            if (index < spARContentItem->getLocalPathList().size() - 1) {
                spARContentItem->setCurrentIndex(++index);
                isChange = true;
            }
            break;
        case ARContentPageOP::PAGE_SELECT_OP: {
            int32_t selectIndex = pageNo -1;
            if (selectIndex >= 0 && selectIndex < spARContentItem->getLocalPathList().size() - 1) {
                spARContentItem->setCurrentIndex(selectIndex);
                isChange = true;
            }
        }
            break;
        default:
            break;
    }

    if (!isChange) return;

    removeNode(ARContextMenuType::DATA_VIEWER_MENU);
    viewer->setCurrentMenuType(ARContextMenuType::UNKNOWN_MENU);

    processChangeNode(ARContextMenuType::DATA_VIEWER_MENU);
}

void ARComplexViewerNode::processChangeTemplateMediaPlayer(int32_t op) {
    if (viewer->getCurrentMenuType() != ARContextMenuType::DATA_VIEWER_MENU) {
        return;
    }

    switch (op) {
        case ARContentMediaPlayerOP::MP_START_OP:
            if (refVideoNode) refVideoNode->startPlaying();
            break;
        case ARContentMediaPlayerOP::MP_PAUSE_OP:
            if (refVideoNode) refVideoNode->pausePlaying();
            break;
        default:
            break;
    }
}

void ARComplexViewerNode::updateVideoSize(int32_t w, int32_t h) {
    updateTemplateAspectRatio(w, h);
}

void ARComplexViewerNode::setLayoutSize(gameplay::FormNode * node, std::shared_ptr<SystemLayout> ptrSystemLayout) {
    gameplay::Rectangle rect = node->getFormBounds();
    ptrSystemLayout->setSize((int32_t)rect.width, (int32_t)rect.height);
}

void ARComplexViewerNode::loadContextMenu() {
    contextMenuNode = new ContextMenuFormNode(controlId, contextMenuForm, viewer->getAlpha());
    FormUtility::translate2Center(contextMenuNode);
}

void ARComplexViewerNode::loadARCall() {
    std::shared_ptr<ARSystemLayoutItem> item = std::static_pointer_cast<ARSystemLayoutItem>
            (viewer->getItems()[AR_CALL_MENU]);
    if (item) {
        std::shared_ptr<SystemLayout> ptrSystemLayout = item->getSystemLayout();
        arCallNode = new SystemLayoutFormNode(ptrSystemLayout, viewer->getAlpha(), ARCallFormName);
        FormUtility::translate2Center(arCallNode, false);
        setLayoutSize(arCallNode, ptrSystemLayout);
    } else {
        LOGE("ARComplexViewerNode loadARCall FAIL!!!");
    }
}

void ARComplexViewerNode::loadTemplateForm() {
    formNode = new TemplateFormNode(controlId, formName, viewer);
    FormUtility::translate2Center(formNode);
    formNode->updateControlVisible();
}

void ARComplexViewerNode::loadTemplateQuad() {
    std::shared_ptr<ARContentItem> spARContentItem = currentARContentItem();
    int32_t itemType = spARContentItem->getType();
    std::string path = spARContentItem->getCurrentLocalPath();
//    LOGD("ARComplexViewerNode loadTemplateQuad path(%s) itemType(%d)", path.c_str(), itemType);
    if (itemType == ARContentItemType::VIDEO_DATA) {
        std::shared_ptr<ARVideoItem> spARVideoItem =
                std::dynamic_pointer_cast<ARVideoItem>(spARContentItem);
        auto vp = spARVideoItem->getVideoPlayer();
        vp->listener(this);
        refVideoNode = new TemplateVideoNode(contentNodeName, vp,
                viewer->getAlpha() * viewer->getAlpha(), false);
        contentNode = refVideoNode;
        gameplay::Quaternion quaternion;
        gameplay::Quaternion::createFromEuler(MATH_DEG_TO_RAD(0.F),
                                              MATH_DEG_TO_RAD(180.F),
                                              MATH_DEG_TO_RAD(0.F), &quaternion);
        contentNode->setRotation(quaternion);
    } else {
        refVideoNode = nullptr;
        contentNode = new gameplay::ImageNode(contentNodeName, path.c_str(), false,
                viewer->getAlpha() * viewer->getAlpha(), false);
    }
    gameplay::Vector3 pos, scale;
    formNode->measureContentsTransform(pos, scale);
    contentNode->setTranslation(pos);
    contentNode->setScale(scale);
    if (gameplay::Texture::Sampler *sampler = contentNode->getSampler()) {
        auto *texture = sampler->getTexture();
        int32_t w = texture->getWidth();
        int32_t h = texture->getHeight();
        updateTemplateAspectRatio(w, h);
    }

    contentBgNode = new gameplay::ImageNode(contentBgNodeName, contentBgFileName, false,
            viewer->getAlpha() * viewer->getAlpha(), false);
    formNode->measureContentsTransform(pos, scale);
    pos.z = FORM_SCALE;
    scale.x = scale.x * (1.0F + FORM_SCALE);
    scale.y = scale.y * (1.0F + FORM_SCALE);
    contentBgNode->setTranslation(pos);
    contentBgNode->setScale(scale);
}

void ARComplexViewerNode::updateTemplateAspectRatio(int32_t w, int32_t h) {
    gameplay::Vector3 scale = contentNode->getScale();
    if (w > h) {
        scale.y = scale.y * ((float)h/(float)w);
    } else {
        scale.x = scale.x * ((float)w/(float)h);
    }
    contentNode->setScale(scale);
}


void ARComplexViewerNode::loadFacilityWork() {
    std::shared_ptr<ARSystemLayoutItem> item = std::static_pointer_cast<ARSystemLayoutItem>
            (viewer->getItems()[FACILITY_WORK_MENU]);
    if (item) {
        std::shared_ptr<SystemLayout> ptrSystemLayout = item->getSystemLayout();
        facilityWorkNode = new SystemLayoutFormNode(ptrSystemLayout, viewer->getAlpha(), FacilityWorkFormName);
        FormUtility::translate2Center(facilityWorkNode, false);
        setLayoutSize(facilityWorkNode, ptrSystemLayout);
    } else {
        LOGE("ARComplexViewerNode loadFacilityWork FAIL!!!");
    }
}

void ARComplexViewerNode::loadFacilityMemo() {
    std::shared_ptr<ARSystemLayoutItem> item = std::static_pointer_cast<ARSystemLayoutItem>
            (viewer->getItems()[FACILITY_MEMO_MENU]);
    if (item) {
        std::shared_ptr<SystemLayout> ptrSystemLayout = item->getSystemLayout();
        facilityMemoNode = new SystemLayoutFormNode(ptrSystemLayout, viewer->getAlpha(), FacilityMemoFormName);
        FormUtility::translate2Center(facilityMemoNode, false);
        setLayoutSize(facilityMemoNode, ptrSystemLayout);
    } else {
        LOGE("ARComplexViewerNode loadFacilityMemo FAIL!!!");
    }
}

void ARComplexViewerNode::addTranslateContextMenu() {
    gameplay::Rectangle contextRect = contextMenuNode->getFormBounds();
    gameplay::Rectangle contentRect;
    switch (viewer->getCurrentMenuType()) {
        case ARContextMenuType::AR_CALL_MENU:
            contentRect = arCallNode->getFormBounds();
            break;
        case ARContextMenuType::DATA_VIEWER_MENU:
            contentRect = formNode->getFormBounds();
            break;
        case ARContextMenuType::FACILITY_WORK_MENU:
            contentRect = facilityWorkNode->getFormBounds();
            break;
        case ARContextMenuType::FACILITY_MEMO_MENU:
            contentRect = facilityMemoNode->getFormBounds();
            break;
        default:
            break;
    }

    float moveX = (contentRect.width - contextRect.width) * 0.5F * FORM_SCALE;
    float moveY = (contentRect.height * 0.5F + contextRect.height * 0.5F + CONTEXT_MUNE_BOTTOM_PADDING) * FORM_SCALE; // padding 0.2F

    gameplay::Vector3 translation;
    translation.x = moveX;
    translation.y = -moveY;
    translation.z = 0;

    contextMenuNode->translate(translation);
}

void ARComplexViewerNode::addContextMenuNode() {
    if (contextMenuNode) addChild(contextMenuNode);
}

void ARComplexViewerNode::addContextItemNode() {
    if (arCallNode) addChild(arCallNode);

    if (formNode) addChild(formNode);
    if (contentBgNode) addChild(contentBgNode);
    if (contentNode) addChild(contentNode);

    if (facilityWorkNode) addChild(facilityWorkNode);

    if (facilityMemoNode) addChild(facilityMemoNode);
}

std::shared_ptr<ARContentItem> ARComplexViewerNode::currentARContentItem() {
    std::shared_ptr<ARContentList> contentList = std::static_pointer_cast<ARContentList>(
            viewer->getItems()[ARContextMenuType::DATA_VIEWER_MENU]);
    return contentList->getCurrentItem();
}

void ARComplexViewerNode::setOnClickListener(gameplay::Control *control, int eventFlags, const char* eventId) {
	//control->setId(eventId);
	//control->addListener(this, eventFlags);
}

void ARComplexViewerNode::removeNode(ARContextMenuType type) {
    switch (type) {
        case ARContextMenuType::AR_CALL_MENU:
            //LOGI("ARComplexViewerNode removeNode arCallNode (%p)", arCallNode);
            if (arCallNode) {
                std::shared_ptr<ARSystemLayoutItem> item =
                        std::static_pointer_cast<ARSystemLayoutItem>(viewer->getItems()[type]);
                if (std::shared_ptr<SystemLayout> ptrSystemLayout = item->getSystemLayout()) {
                    ptrSystemLayout->destroyObject();
                }
                removeChild(arCallNode);
                arCallNode->release();
                arCallNode = nullptr;
            }
                break;
        case ARContextMenuType::DATA_VIEWER_MENU:
            //LOGI("ARComplexViewerNode removeNode formNode (%p)", formNode);
            if (formNode) {
                removeChild(formNode);
                formNode->release();
                formNode = nullptr;
            }
            //LOGI("ARComplexViewerNode removeNode contentNode (%p)", contentNode);
            if (contentNode) {
                removeChild(contentNode);
                contentNode->release();
                contentNode = nullptr;
                refVideoNode = nullptr;
            }

            if (contentBgNode) {
                removeChild(contentBgNode);
                contentBgNode->release();
                contentBgNode = nullptr;
            }
                break;
        case ARContextMenuType::FACILITY_WORK_MENU:
            //LOGI("ARComplexViewerNode removeNode facilityWorkNode (%p)", facilityWorkNode);
            if (facilityWorkNode) {
                std::shared_ptr<ARSystemLayoutItem> item =
                        std::static_pointer_cast<ARSystemLayoutItem>(viewer->getItems()[type]);
                if (std::shared_ptr<SystemLayout> ptrSystemLayout = item->getSystemLayout()) {
                    ptrSystemLayout->destroyObject();
                }
                removeChild(facilityWorkNode);
                facilityWorkNode->release();
                facilityWorkNode = nullptr;
            }
                break;
        case ARContextMenuType::FACILITY_MEMO_MENU:
            //LOGI("ARComplexViewerNode removeNode facilityMemoNode (%p)", facilityMemoNode);
            if (facilityMemoNode) {
                std::shared_ptr<ARSystemLayoutItem> item =
                        std::static_pointer_cast<ARSystemLayoutItem>(viewer->getItems()[type]);
                if (std::shared_ptr<SystemLayout> ptrSystemLayout = item->getSystemLayout()) {
                    ptrSystemLayout->destroyObject();
                }
                removeChild(facilityMemoNode);
                facilityMemoNode->release();
                facilityMemoNode = nullptr;
            }
                break;
        default:
            break;
    }
}

void ARComplexViewerNode::applyAlpha(float alpha) {
#if 0
	for (Control* control : form->getControls()) {
		if (!strcmp(control->getId(), "text1") || !strcmp(control->getId(), "text2")) {
			continue;
		}
		control->setOpacity(alpha);
	}
#endif
}

void ARComplexViewerNode::setContextMenuEnable(bool enable) {
    if (contextMenuNode != nullptr) {
        contextMenuNode->setEnableMenu(enable);
    }
}

int32_t ARComplexViewerNode::getCurrentMenuType() {
    return viewer->getCurrentMenuType();
}

int32_t ARComplexViewerNode::getCurrentViewerPage() {
    if (viewer->getCurrentMenuType() != ARContextMenuType::DATA_VIEWER_MENU) {
        return -1;
    }

    std::shared_ptr<ARContentList> spContentList = std::static_pointer_cast<ARContentList>(
            viewer->getItems()[ARContextMenuType::DATA_VIEWER_MENU]);
    std::shared_ptr<ARContentItem> spARContentItem = spContentList->getCurrentItem();
    return spARContentItem->getCurrentIndex();
}