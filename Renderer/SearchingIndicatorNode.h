#pragma once

#include "Node.h"
#include <list>

#define POINT_COUNT       4

namespace gameplay {
    class SearchingIndicatorNode : public Node {
    public:

        enum SearchState {
            SEARCHING,
            FINDING,
            FOUND,
            DONE
        };

        static SearchingIndicatorNode *
        create(const char *id, Vector3 translation, Quaternion rotation, Vector3 scale);

        void searching(Vector3 translation, Quaternion rotation, Vector3 scale, float elapsedTime);

        void
        foundTrackable(Vector3 translation, Quaternion rotation, Vector3 scale, float elapsedTime);

        SearchState _state;

    private:
        SearchingIndicatorNode(const char *id, Vector3 translation, Quaternion rotation,
                               Vector3 scale);

        void addQuadModelAndNode(float size);

        Node *makeNodeQuadModel(float size);

        Material *setTextureUnlitMaterial(Model *model, const char *texturePath, bool mipmap = true,
                                          bool isPoint = false);

        float getAnimateProgress(float elapsedTime, float duration);

        Vector3 points[POINT_COUNT] = {
                Vector3(-0.5, -0.5, -0.1), Vector3(0.5, -0.5, -0.1), Vector3(0.5, 0.5, -0.1),
                Vector3(-0.5, 0.5, -0.1)
        };

        Material *_material;
        MaterialParameter *_materialAlpha;
        float _materialAlphaValue;

        float _elapsedTime;
        float _duration;
        float _animationSpeed;
        float _lastDuration;

        std::vector<Node *> pointNodeList;
    };
}
