//
// Created by kt-hyunjun on 2019-12-14.
//

#pragma once

#include "gameplay.h"

namespace armakers {

    class NodeUtility {
    public:
        static void transformLookAt(gameplay::Node* n1, const gameplay::Vector3& position,
                /*out*/ gameplay::Vector3* translation,
                /*out*/ gameplay::Quaternion* quaternion);
    };

}