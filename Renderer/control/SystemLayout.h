//
// Created by kt-hyunjun on 2019-10-22.
//

#pragma once

#include <string>
#include <memory>
#include "gameplay.h"

namespace armakers {
    class SystemLayout {
    public:
        static std::shared_ptr<SystemLayout> create(std::string name, int w, int h, int layoutId);

        SystemLayout(std::string name, int w, int h, int layoutId) {
            this->name = name;
            this->layoutId = layoutId;
            width = w;
            height = h;
        }

        virtual ~SystemLayout() {}

        virtual void createObject(int width, int height, int textureId, int layoutId) = 0;

        virtual bool isDrawable(gameplay::ImageControl *imageControl, gameplay::Form *form) = 0;

        virtual void update() = 0;

        virtual void destroyObject() = 0;

        const char *getName() { return name.c_str(); }

        void setSize(int w, int h) {
            width = w;
            height = h;
        }

        virtual void onPressEvent(int contactIndex, int x, int y) = 0;

        virtual void onMoveEvent(int contactIndex, int x, int y) = 0;

        virtual void onReleaseEvent(int contactIndex, int x, int y) = 0;

    protected:
        std::string name;
        int layoutId;
        int width;
        int height;
    };
}