package com.kt.renderobject;

public class ARContentItem {

    public enum ARContentItemType {
        UNKNOWN_DATA(-1),
        IMAGE_DATA(0),
        DOC_DATA(1),
        VIDEO_DATA(2);

        private int type;
        ARContentItemType(int type) {
            this.type = type;
        }
        public int getType() {
            return type;
        }
    }
    private int type;
    private String contentID;
    private String menuTitle;
    private String realFileName;
    private int currentIndex;
    private String[] localPathList;

    public ARContentItem(ARContentItemType type, String contentID, String menuTitle, String realFileName,
                         int currentIndex, String[] localPathList) {
        this.type = type.getType();
        this.contentID = contentID;
        this.menuTitle = menuTitle;
        this.realFileName = realFileName;
        this.currentIndex = currentIndex;
        this.localPathList = localPathList;
    }

    public int getCurrentIndex() {
        return currentIndex;
    }

    public void setCurrentIndex(int currentIndex) {
        this.currentIndex = currentIndex;
    }

    public static ARContentItemType parseType(String type) {
        if ("01".equals(type)) {
            return ARContentItemType.IMAGE_DATA;
        } else if ("02".equals(type)) {
            return ARContentItemType.VIDEO_DATA;
        } else if ("03".equals(type)) {
            return ARContentItemType.DOC_DATA;
        } else {
            return ARContentItemType.UNKNOWN_DATA;
        }
    }
}
