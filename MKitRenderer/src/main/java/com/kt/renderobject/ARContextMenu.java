package com.kt.renderobject;

public class ARContextMenu {
    public enum ARContextMenuType {
        UNKNOWN_MENU(-1),
        AR_CALL_MENU(0),
        DATA_VIEWER_MENU(1),
        FACILITY_WORK_MENU(2),
        FACILITY_MEMO_MENU(3),
        CLOSE_MENU(4);

        private int type;
        ARContextMenuType(int type) {
            this.type = type;
        }
        public int getType() {
            return type;
        }
    }

    private int type;
    private String bgImagePath;
    private String text;

    public ARContextMenu(ARContextMenuType type, String bgImagePath, String text) {
        this.type = type.getType();
        this.bgImagePath = bgImagePath;
        this.text = text;
    }

    public int getType() {
        return type;
    }

    public String getBgImagePath() {
        return bgImagePath;
    }

    public String getText() {
        return text;
    }
}
