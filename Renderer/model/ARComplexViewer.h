//
// Created by kt-hyunjun on 2019-11-26.
//

#pragma once

#include <vector>
#include <string>

#include "gameplay.h"
#include <control/SystemLayout.h>
#include <VideoPlayer.h>

namespace armakers {
    enum ARContextMenuType {
        UNKNOWN_MENU = -1,
        AR_CALL_MENU,
        DATA_VIEWER_MENU,
        FACILITY_WORK_MENU,
        FACILITY_MEMO_MENU,
        CLOSE_MENU,
    };

    inline std::string ARContextMenuTypeToString(ARContextMenuType v) {
        switch (v) {
            case AR_CALL_MENU:
                return "AR_CALL_MENU";
            case DATA_VIEWER_MENU:
                return "DATA_VIEWER_MENU";
            case FACILITY_WORK_MENU:
                return "FACILITY_WORK_MENU";
            case FACILITY_MEMO_MENU:
                return "FACILITY_MEMO_MENU";
            default:
                return "UNKNOWN_MENU";
        }
    }

    enum ARContentItemType {
        UNKNOWN_DATA = -1,
        IMAGE_DATA,
        DOC_DATA,
        VIDEO_DATA,
    };

    enum ARContentPageOP {
        PAGE_PREV_OP = 0,
        PAGE_NEXT_OP,
        PAGE_SELECT_OP,
    };

    enum ARContentMediaPlayerOP {
        MP_START_OP = 0,
        MP_PAUSE_OP,
    };

    class ARContextMenu {
    public:
        ARContextMenu();

        virtual ~ARContextMenu();

        void setType(int32_t t) { type = t; }

        int32_t getType() { return type; }

        void setBgImagePath(const std::string &s) { bgImagePath = s; }

        std::string &getBgImagePath() { return bgImagePath; }

        void setText(const std::string &s) { text = s; }

        std::string &getText() { return text; }

    private:
        int32_t type;
        std::string bgImagePath;
        std::string text;
    };

    class ARMenuItem {
    public:
        ARMenuItem();

        virtual ~ARMenuItem();

        void setType(int32_t t) { type = t; }

        int32_t getType() { return type; }

        void setResID(int32_t t) { resID = t; }

        int32_t getResID() { return resID; }

        void setForm(const std::string &s) { form = s; }

        std::string &getForm() { return form; }

    private:
        int32_t type;
        int32_t resID;
        std::string form;
    };

    class ARSystemLayoutItem : public ARMenuItem {
    public:
        ARSystemLayoutItem();

        virtual ~ARSystemLayoutItem();

        void setSystemLayout(std::shared_ptr<SystemLayout> s) { systemLayout = s; }

        std::shared_ptr<SystemLayout> getSystemLayout() { return systemLayout; }

    private:
        std::shared_ptr<SystemLayout> systemLayout;
    };


    class ARContentItem {
    public:
        ARContentItem();

        virtual ~ARContentItem();

        void setType(int32_t &t) { type = t; }

        int32_t getType() { return type; }

        void setContentID(const std::string &s) { contentID = s; }

        std::string &getContentID() { return contentID; }

        void setMenuTitle(const std::string &s) { menuTitle = s; }

        std::string &getMenuTitle() { return menuTitle; }

        void setRealFileName(const std::string &s) { realFileName = s; }

        std::string &getRealFileName() { return realFileName; }

        void setCurrentIndex(int32_t t) { currentIndex = t; }

        int32_t getCurrentIndex() { return currentIndex; }

        void setLocalPathList(std::vector<std::string> &l) { this->localPathList = l; }

        std::vector<std::string> &getLocalPathList() { return localPathList; }

        std::string getCurrentLocalPath() { return localPathList[currentIndex]; }

    private:
        int32_t type;
        std::string contentID;
        std::string menuTitle;
        std::string realFileName;
        int32_t currentIndex;
        std::vector<std::string> localPathList;
    };

    class ARVideoItem : public ARContentItem {
    public:
        ARVideoItem();

        virtual ~ARVideoItem();

        void setVideoPlayer(std::shared_ptr<gameplay::VideoPlayer> s) { videoPlayer = s; }

        std::shared_ptr<gameplay::VideoPlayer> getVideoPlayer() { return videoPlayer; }

    private:
        std::shared_ptr<gameplay::VideoPlayer> videoPlayer;
    };

    class ARContentList : public ARMenuItem {
    public:
        ARContentList();

        virtual ~ARContentList();

        void setCurrentItemIndex(int32_t t) { currentItemIndex = t; }

        int32_t getCurrentItemIndex() { return currentItemIndex; }

        void setItems(std::vector<std::shared_ptr<ARContentItem> > &l) { this->items = l; }

        std::vector<std::shared_ptr<ARContentItem> > &getItems() { return items; }

        std::shared_ptr<ARContentItem> getCurrentItem() { return items[currentItemIndex]; }

    private:
        std::vector<std::shared_ptr<ARContentItem> > items;
        int32_t currentItemIndex;
    };

    class ARComplexViewer {
    public:
        ARComplexViewer();

        virtual ~ARComplexViewer();

        void facilityName(const std::string &s) { _facilityName = s;}

        std::string facilityName() { return _facilityName; }

        void setPos(const gameplay::Vector3 &p) { pos = p; }

        gameplay::Vector3 &getPos() { return pos; }

        void setScale(const gameplay::Vector3 &s) { scale = s; }

        gameplay::Vector3 &getScale() { return scale; }

        void setRotation(const gameplay::Vector3 &r) { rotation = r; }

        gameplay::Vector3 &getRotation() { return rotation; }

        void setAlpha(float a) { alpha = a; }

        float getAlpha() { return alpha; }

        void setCurrentMenuType(int32_t t) { currentMenuType = t; }

        int32_t getCurrentMenuType() { return currentMenuType; }

        void setMenuLayoutEnable(bool e) { menuLayoutEnable = e; }

        bool getMenuLayoutEnable() { return menuLayoutEnable; }

        void setMenus(std::vector<std::shared_ptr<ARContextMenu> > &menus) { this->menus = menus; }

        std::vector<std::shared_ptr<ARContextMenu> > &getMenus() { return menus; }

        void setItems(std::vector<std::shared_ptr<ARMenuItem> > &items) { this->items = items; }

        std::vector<std::shared_ptr<ARMenuItem> > &getItems() { return items; }

    private:
        std::string _facilityName;
        gameplay::Vector3 pos;
        gameplay::Vector3 scale;
        gameplay::Vector3 rotation;
        float alpha;
        int32_t currentMenuType;
        bool menuLayoutEnable;
        std::vector<std::shared_ptr<ARContextMenu> > menus;
        std::vector<std::shared_ptr<ARMenuItem> > items;
    };
}