#pragma once

#include "gameplay.h"

namespace gameplay {
	enum QuadNodeType {
		VIDEO,
		IMAGE,
		TEXT
	};

	class QuadNode : public Node {
	public:

		QuadNode(const char *id, QuadNodeType nodeType);

		QuadNode(const char *id, QuadNodeType nodeType, float alpha);

        QuadNode(const char *id, QuadNodeType nodeType, float alpha, bool isDepthWrite);

		virtual ~QuadNode();

		void setTexture(const char *texturePath, bool mipmap);

		Material *getMaterial();
        Texture::Sampler *getSampler();
		QuadNodeType getType();

	protected:
		void initModel(float size);

		void createDefaultMaterial(bool mipmap = true, bool modulateAlpha = false);

		void createDefaultMaterial(bool mipmap = true, float alpha = 1.0f);

		Material *material;
		MaterialParameter *materialParameter;
        Texture::Sampler *sampler;

		std::vector<Vector3> vertexCoordinate;
		QuadNodeType nodeType;
		bool isDepthWrite;
	};
}

