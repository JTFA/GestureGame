
#include "gameplay.h"
#include <LogUtils.h>
#include <utility/ImageHelper.h>

#include <QuadNode.h>

using namespace gameplay;

QuadNode::QuadNode(const char *id, QuadNodeType nodeType) : Node(id), materialParameter(nullptr)
, sampler(nullptr) {
	initModel(1.0f);
	createDefaultMaterial(false, false);
	this->nodeType = nodeType;
	isDepthWrite = true;
}

QuadNode::QuadNode(const char *id, QuadNodeType nodeType, float alpha) : Node(id), materialParameter(nullptr)
, sampler(nullptr) {
	initModel(1.0f);
	createDefaultMaterial(false, alpha);
	this->nodeType = nodeType;
	isDepthWrite = true;
}

QuadNode::QuadNode(const char *id, QuadNodeType nodeType, float alpha, bool isDepthWrite) : Node(id)
, materialParameter(nullptr), sampler(nullptr) {
	initModel(1.0f);
	createDefaultMaterial(false, alpha);
	this->nodeType = nodeType;
	this->isDepthWrite = isDepthWrite;
}

QuadNode::~QuadNode()
{
	LOGD("QuadNode Destructor");
}

QuadNodeType QuadNode::getType() {
	return nodeType;
}

void QuadNode::initModel(float size) {
	vertexCoordinate.push_back(Vector3(-size / 2, -size / 2, 0));
	vertexCoordinate.push_back(Vector3(-size / 2, size / 2, 0));
	vertexCoordinate.push_back(Vector3(size / 2, -size / 2, 0));
	vertexCoordinate.push_back(Vector3(size / 2, size / 2, 0));

	Mesh *mesh = Mesh::createQuad(vertexCoordinate[0], vertexCoordinate[1],
								  vertexCoordinate[2], vertexCoordinate[3]);

	mesh->setBoundingSphere(BoundingSphere(Vector3::zero(), size));

	Model *model = Model::create(mesh);
	setDrawable(model);
	SAFE_RELEASE(model);
	SAFE_RELEASE(mesh);
}

void QuadNode::createDefaultMaterial(bool mipmap, bool modulateAlpha) {
	Model * model = dynamic_cast<Model *>(getDrawable());
	if (!modulateAlpha) {
		material = model->setMaterial("res/shaders/textured.vert", "res/shaders/textured.frag");
		material->setParameterAutoBinding("u_worldViewProjectionMatrix",
										  "WORLD_VIEW_PROJECTION_MATRIX");
	} else {
		material = model->setMaterial("res/shaders/textured.vert", "res/shaders/textured.frag",
									  "MODULATE_ALPHA");
		material->setParameterAutoBinding("u_worldViewProjectionMatrix",
										  "WORLD_VIEW_PROJECTION_MATRIX");
		MaterialParameter* _materialAlpha = material->getParameter("u_modulateAlpha");
		_materialAlpha->setValue(0.8f);
	}

	material->getStateBlock()->setCullFace(false);
	material->getStateBlock()->setDepthTest(false);
	material->getStateBlock()->setDepthWrite(true);
}

void QuadNode::createDefaultMaterial(bool mipmap, float alpha) {
	Model * model = dynamic_cast<Model *>(getDrawable());
	material = model->setMaterial("res/shaders/textured.vert", "res/shaders/textured.frag",
								  "MODULATE_ALPHA");
	material->setParameterAutoBinding("u_worldViewProjectionMatrix",
									  "WORLD_VIEW_PROJECTION_MATRIX");
	MaterialParameter* _materialAlpha = material->getParameter("u_modulateAlpha");
	_materialAlpha->setValue(alpha);

	material->getStateBlock()->setCullFace(false);
	material->getStateBlock()->setDepthTest(false);
	material->getStateBlock()->setDepthWrite(true);
}

void QuadNode::setTexture(const char * texturePath, bool mipmap) {
    // Load the texture from file.
    if (texturePath != nullptr) {
        ImageHelper helper;
        if (helper.checkSupportedExt(texturePath)) {
            sampler = helper.createSampler(texturePath, mipmap);
            material->getParameter("u_diffuseTexture")->setValue(sampler);
        } else {
            sampler = material->getParameter("u_diffuseTexture")->setValue(texturePath, mipmap);
        }
        if (mipmap) {
            sampler->setFilterMode(Texture::LINEAR_MIPMAP_LINEAR, Texture::LINEAR);
        } else {
            sampler->setFilterMode(Texture::LINEAR, Texture::LINEAR);
        }
        sampler->setWrapMode(Texture::CLAMP, Texture::CLAMP);
    }

    material->getStateBlock()->setCullFace(true);
    material->getStateBlock()->setDepthTest(true);
    material->getStateBlock()->setDepthWrite(isDepthWrite);

    material->getStateBlock()->setBlend(true);
    material->getStateBlock()->setBlendSrc(RenderState::BLEND_SRC_ALPHA);
    material->getStateBlock()->setBlendDst(RenderState::BLEND_ONE_MINUS_SRC_ALPHA);


    // TODO : Jack. Need to test when changing texture
    /*
    Texture * newTexture = Texture::create(gameplay::Texture::RGB888, videoWidth, videoHeight, 0, false, Texture::TEXTURE_2D);
    Texture::Sampler* sampler = Texture::Sampler::create(videoTexture);
    sampler->setFilterMode(Texture::LINEAR, Texture::LINEAR);
    sampler->setWrapMode(Texture::CLAMP, Texture::CLAMP);

    // TODO : Jack. For front camera ar
    //videoMaterial->getStateBlock()->setCullFace(true);
    material->getStateBlock()->setDepthTest(true);
    material->getStateBlock()->setDepthWrite(true);
    material->getParameter("u_diffuseTexture")->setValue(sampler);
     */
}

Material *QuadNode::getMaterial() {
	return material;
}

Texture::Sampler *QuadNode::getSampler() {
    return sampler;
}