package com.kt.renderobject;

public class ARComplexViewer {
    public static final String MENU_EVENT_NAME = ARComplexViewer.class.getSimpleName();
    public static final String ITEM_EVENT_NAME = ARComplexViewer.class.getSimpleName() + "::ITEM";
    public static final String PAGE_EVENT_NAME = ARComplexViewer.class.getSimpleName() + "::PAGE";
    public static final String VIDEO_EVENT_NAME = ARComplexViewer.class.getSimpleName() + "::VIDEO";

    private String facilityName;
    private float[] pos;
    private float[] scale;
    private float[] rotation;
    private float alpha;
    private int currentMenuType;
    private boolean menuLayoutEnable;
    private ARContextMenu[] menus;
    private ARMenuItem[] items;

    public ARComplexViewer(ARContextMenu[] menus, ARMenuItem[] items) {
        this.menus = menus;
        this.items = items;
        menuLayoutEnable = true;
        alpha = 1.0F;
        currentMenuType = menus.length > 0 ? menus[0].getType() : ARContextMenu.ARContextMenuType.UNKNOWN_MENU.getType();
    }

    public void setFacilityName(String facilityName) {
        this.facilityName = facilityName;
    }

    public void setAlpha(float alpha) {
        this.alpha = alpha;
    }

    public void setCurrentMenuType(ARContextMenu.ARContextMenuType type) {
        this.currentMenuType = type.getType();
    }

    public void setMenuLayoutEnable(boolean enable) {
        this.menuLayoutEnable = enable;
    }

    public void setTransform(float[] pos, float[] rotation, float[] scale) {
        this.pos = pos;
        this.rotation = rotation;
        this.scale = scale;
    }
}
