#pragma once

#include <QuadNode.h>

namespace gameplay {
	class ImageNode : public QuadNode {
	public:
		ImageNode(const char *id, const char *filePath, bool mipmap);

		ImageNode(const char *id, const char *filePath, bool mipmap, bool modulateAlpha);

		ImageNode(const char *id, const char *filePath, bool mipmap, float alpha);

		ImageNode(const char *id, const char *filePath, bool mipmap, float alpha, bool isDepthWrite);

		~ImageNode();
	};
}


