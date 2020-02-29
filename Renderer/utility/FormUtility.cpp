//
// Created by kt-hyunjun on 2019-12-03.
//

#include <utility/FormUtility.h>

void FormUtility::translate2Center(gameplay::FormNode *node, bool isRotate) {
    gameplay::Rectangle rect = node->getFormBounds();
    node->setScale(gameplay::Vector3(FORM_SCALE, FORM_SCALE, FORM_SCALE));
    gameplay::Quaternion quaternion;
    if (isRotate) {
        gameplay::Quaternion::createFromEuler(MATH_DEG_TO_RAD(0.F),
                                              MATH_DEG_TO_RAD(180.F),
                                              MATH_DEG_TO_RAD(0.F), &quaternion);
    }

    gameplay::Vector3 translation;
    float moveX = rect.width * 0.5F * FORM_SCALE;
    float moveY = rect.height * 0.5F * FORM_SCALE;

    translation.x = -moveX;
    translation.y = isRotate ? moveY : -moveY;
    translation.z = 0;

    node->setTranslation(translation);
    node->setRotation(quaternion);
}

void FormUtility::move2Center(gameplay::FormNode *node, bool isRotate) {
    gameplay::Rectangle rect = node->getFormBounds();
    gameplay::Vector3 translation;
    float moveX = rect.width * 0.5F * FORM_SCALE;
    float moveY = rect.height * 0.5F * FORM_SCALE;

    translation.x = -moveX;
    translation.y = isRotate ? moveY : -moveY;
    translation.z = 0;

    node->setTranslation(translation);
}
