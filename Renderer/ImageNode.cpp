#define LOG_NDEBUG 0

#ifdef LOG_TAG
#undef LOG_TAG
#define LOG_TAG "ImageNode"
#endif

#include <LogUtils.h>
#include <ImageNode.h>

using namespace gameplay;

ImageNode::ImageNode(const char *id, const char *filePath, bool mipmap) :
        QuadNode(id, QuadNodeType::IMAGE) {
    setTexture(filePath, mipmap);
//    LOGD("ImageNode %p", this);

}

ImageNode::ImageNode(const char *id, const char *filePath, bool mipmap, bool modulateAlpha) :
		QuadNode(id, QuadNodeType::IMAGE, modulateAlpha) {
//	, filePath, translation, rotation, scale)
	setTexture(filePath, mipmap);
//	LOGD("ImageNode %p", this);
}

ImageNode::ImageNode(const char *id, const char *filePath, bool mipmap, float alpha) :
        QuadNode(id, QuadNodeType::IMAGE, alpha) {
//	, filePath, translation, rotation, scale)
    setTexture(filePath, mipmap);
//	LOGD("ImageNode %p : %s : alpha %f", this, filePath, filePath, alpha);
}

ImageNode::ImageNode(const char *id, const char *filePath, bool mipmap, float alpha, bool isDepthWrite) :
        QuadNode(id, QuadNodeType::IMAGE, alpha, isDepthWrite) {
//	, filePath, translation, rotation, scale)
    setTexture(filePath, mipmap);
//	LOGD("ImageNode %p : %s : alpha %f", this, filePath, filePath, alpha);
}

ImageNode::~ImageNode() {
	LOGD("ImageNode::~ImageNode destroy %p", this);
}