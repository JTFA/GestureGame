#pragma once

#include "gameplay.h"
#include "Node.h"
#include <memory>
#include <string>

#include <ImageNode.h>


#include <model/ARComplexViewer.h>
#include <node/TemplateVideoNode.h>
#include <node/SystemLayoutFormNode.h>
#include <node/TemplateFormNode.h>
#include <node/ContextMenuFormNode.h>
#include <node/ExtensionRenderable.h>

namespace armakers {
    class ARComplexViewerNode : public gameplay::Node, public gameplay::VideoSizeListener {
    public:
        ARComplexViewerNode(std::string &nodeId, std::string &controlId,
                            std::shared_ptr<ARComplexViewer> viewer);

        ~ARComplexViewerNode();

        void processChangeNode(ARContextMenuType type);
        void setContextMenuBadge(int count);

        void processChangeTemplateItem(int32_t index);

        void processChangeTemplatePage(int32_t op, int32_t pageNo = 0);

        void processChangeTemplateMediaPlayer(int32_t op);

        virtual void updateVideoSize(int32_t w, int32_t h);

        void setContextMenuEnable(bool enable);

        int32_t getCurrentMenuType();

        int32_t getCurrentViewerPage();

    protected:
        void applyAlpha(float alpha);

    private:
        void setLayoutSize(gameplay::FormNode *node, std::shared_ptr<SystemLayout> ptrSystemLayout);

        void loadContextMenu();

        void loadARCall();

        void loadTemplateForm();

        void loadTemplateQuad();

        void updateTemplateAspectRatio(int32_t w, int32_t h);

        void loadFacilityWork();

        void loadFacilityMemo();

        void addTranslateContextMenu();

        void addContextMenuNode();

        void addContextItemNode();

        std::shared_ptr<ARContentItem> currentARContentItem();

        void setOnClickListener(gameplay::Control *control, int eventFlags, const char *eventId);

        void removeNode(ARContextMenuType type);

    private:
        std::string controlId;
        std::shared_ptr<ARComplexViewer> viewer;

        ContextMenuFormNode *contextMenuNode;

        SystemLayoutFormNode *arCallNode;

        TemplateFormNode *formNode;
        gameplay::QuadNode *contentNode;
        TemplateVideoNode *refVideoNode;
        gameplay::QuadNode *contentBgNode;

        SystemLayoutFormNode *facilityWorkNode;
        SystemLayoutFormNode *facilityMemoNode;
    };
}