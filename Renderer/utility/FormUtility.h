//
// Created by kt-hyunjun on 2019-12-03.
//

#pragma once

#include "gameplay.h"
#include <FormNode.h>

//form size 1000 equals 3d object scale 1
#define FORM_SCALE 0.001F

class FormUtility {
public:
    static void translate2Center(gameplay::FormNode *node, bool isRotate = true);
    static void move2Center(gameplay::FormNode *node, bool isRotate = true);
};