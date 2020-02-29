package com.kt.renderobject;

public class ARContentList extends ARMenuItem {
    private ARContentItem[] items;
    private int currentItemIndex;

    public ARContentList(ARContextMenu.ARContextMenuType type, int resID, String form, ARContentItem[] items) {
        super(type, resID, form);
        this.items = items;
        currentItemIndex = 0;
    }
}
