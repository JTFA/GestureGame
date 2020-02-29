#include "Utilities.h"
#include <Rectangle.h>
#include <string.h>
#include <string>
#include <Base.h>

int Utilities::getHashCode(const char* keyValue)
{
    int hash = 0;
    int count = strlen(keyValue);
    if (count > 0)
    {
        for (int i = 0; i < count; i++)
        {
            hash = 31*hash + keyValue[i];
        }
    }
    return hash;
}

const char* Utilities::commify(int value)
{
	char separator = ',';
	int width = 3;

	char buf[30];
	memset(buf, 0x00, sizeof(buf));
	sprintf(buf, "%d", value);

	std::string strBuf(buf);
	std::string::iterator i = strBuf.end() - width;
	while (i > strBuf.begin())
		i = strBuf.insert(i, separator) - width;

	return strBuf.c_str();
}

gameplay::Vector3 Utilities::getScale(gameplay::Rectangle rectangle, gameplay::Vector3 scale) {
	gameplay::Vector3 formScale;
	formScale.x = scale.x / rectangle.width;
	formScale.y = scale.y / rectangle.height * -1;
	formScale.z = 1;
	return formScale;
}

gameplay::Vector3 Utilities::getScale2(gameplay::Rectangle rectangle, gameplay::Vector3 scale) {
    gameplay::Vector3 formScale;

    float min = std::min(rectangle.width, rectangle.height);

    formScale.x = scale.x / min;
    formScale.y = scale.y / -min;
    formScale.z = 1;
    return formScale;
}

gameplay::Vector3 Utilities::getTranslate(float width, float height, gameplay::Vector3 originalPosition)
{
    float translate = 0.0005f;

    gameplay::Vector3 translation;

    float moveX = width * translate;
    float moveY = height * translate;

    // TODO : Do Something
    translation.x = originalPosition.x - moveX;
    translation.y = originalPosition.y + moveY;
    translation.z = 0;

    return translation;
}

gameplay::Vector3 Utilities::getScale4Form(gameplay::Rectangle rectangle, gameplay::Vector3 scale, float distance) {
    gameplay::Vector3 formScale;
    float min = std::min(distance / rectangle.width, distance / rectangle.height);
    formScale.x = scale.x * min;
    formScale.y = scale.y * min;
    formScale.z = 0.001F;
    return formScale;
}

gameplay::Vector3 Utilities::getScale4Form2(gameplay::Rectangle rectangle, gameplay::Vector3 scale, float distance) {
    gameplay::Vector3 formScale;
    float min = std::min(distance / rectangle.width, distance / rectangle.height);
    formScale.x = scale.x * min;
    formScale.y = scale.y * -min;
    formScale.z = 0.001F;
    return formScale;
}

gameplay::Vector3 Utilities::getTranslate4Form(float width, float height, gameplay::Vector3 originalPosition) {
    const float translate = 0.01F;
    gameplay::Vector3 translation;
    float moveX = width * translate * 0.8F;
    float moveY = height * translate;

    translation.x = originalPosition.x - moveX;
    translation.y = originalPosition.y + moveY;
    translation.z = originalPosition.z;

    return translation;
}

float Utilities::dotProduct(gameplay::Quaternion &q1, gameplay::Quaternion &q2) {
    return (q1.w * q2.w) + (q1.x * q2.x) + (q1.y * q2.y) + (q1.z * q2.z);
}
