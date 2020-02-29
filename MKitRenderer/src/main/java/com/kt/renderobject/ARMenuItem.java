package com.kt.renderobject;

public class ARMenuItem {
    private int type;
    private int resID;
    private String form;

    public ARMenuItem(ARContextMenu.ARContextMenuType type, int resID, String form) {
        this.type = type.getType();
        this.resID = resID;
        this.form = form;
    }
}
