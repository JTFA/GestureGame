#pragma once
#include "gameplay.h"

#include <math.h>
#include <cstring>

class Utilities {
public:
    static int getHashCode(const char* keyValue);
	static const char* commify(int value);
	static gameplay::Vector3 getTranslate(float width, float height, gameplay::Vector3 originalPosition);
	static gameplay::Vector3 getScale(gameplay::Rectangle rectangle, gameplay::Vector3 scale);
	static gameplay::Vector3 getScale2(gameplay::Rectangle rectangle, gameplay::Vector3 scale);
	static gameplay::Vector3 getTranslate4Form(float width, float height, gameplay::Vector3 originalPosition);
	static gameplay::Vector3 getScale4Form(gameplay::Rectangle rectangle, gameplay::Vector3 scale, float distance);
	static gameplay::Vector3 getScale4Form2(gameplay::Rectangle rectangle, gameplay::Vector3 scale, float distance);
	static float dotProduct(gameplay::Quaternion &q1, gameplay::Quaternion &q2);
};


