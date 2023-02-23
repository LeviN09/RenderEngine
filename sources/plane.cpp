#include "../headers/plane.hpp"

void Plane::InitPlane() {
    PushToVerts({-10.0f, -10.0f, 10.0f,   1.0f, 0.0f, 1.0f,    0.0f, 0.0f});
    PushToVerts({ 10.0f, -10.0f, 10.0f,   1.0f, 1.0f, 0.0f,    1.0f, 0.0f});
    PushToVerts({ 10.0f,  10.0f, 10.0f,   0.0f, 1.0f, 1.0f,    1.0f, 1.0f});
    PushToVerts({-10.0f,  10.0f, 10.0f,   1.0f, 1.0f, 1.0f,    0.0f, 1.0f});

    PushToInds({0, 1, 2});
    PushToInds({0, 2, 3});

    hasNormals = false;
    hasColorTexture = true;
}