//
// Created by kt-hyunjun on 2019-11-26.
//

#include <model/ARComplexViewer.h>

using namespace armakers;

//ARContextMenu
ARContextMenu::ARContextMenu() {

}

ARContextMenu::~ARContextMenu() {

}
//ARMenuItem
ARMenuItem::ARMenuItem() {

}
ARMenuItem::~ARMenuItem() {

}
//ARSystemLayoutItem
ARSystemLayoutItem::ARSystemLayoutItem() : ARMenuItem() {

}
ARSystemLayoutItem::~ARSystemLayoutItem() {

}
//ARContentItem
ARContentItem::ARContentItem() {
    currentIndex = 0;
}
ARContentItem::~ARContentItem() {

}
//ARVideoItem
ARVideoItem::ARVideoItem() : ARContentItem() {

}
ARVideoItem::~ARVideoItem() {

}
//ARContentList
ARContentList::ARContentList() : ARMenuItem() {
    currentItemIndex = 0;
}
ARContentList::~ARContentList() {

}
//ARComplexViewer
ARComplexViewer::ARComplexViewer() {
    currentMenuType = 0;
}
ARComplexViewer::~ARComplexViewer() {

}