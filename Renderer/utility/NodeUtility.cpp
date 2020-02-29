//
// Created by kt-hyunjun on 2019-12-14.
//

#include <cmath>

#include <LogUtils.h>
#include <utility/NodeUtility.h>

using namespace armakers;

void NodeUtility::transformLookAt(gameplay::Node* target, const gameplay::Vector3& position,
        gameplay::Vector3* t, gameplay::Quaternion* q) {
    if (!t || !q) return;
    gameplay::Vector3 localTranslation = target->getTranslation();
    gameplay::Quaternion localQuaternion = target->getRotation();
    gameplay::Vector3 localRotate;
    localQuaternion.computeEuler(&localRotate.x, &localRotate.y, &localRotate.z);

    //position
    localQuaternion.rotatePoint(position, t);

    //rotation
    gameplay::Vector3 rotate;
    gameplay::Vector3::cross(*t, localTranslation, &rotate);
    rotate.add(localRotate);
    gameplay::Quaternion::createFromEuler(rotate.x, rotate.y, rotate.z, q);
}


