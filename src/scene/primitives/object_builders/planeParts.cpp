#include "scene/primitives/object_builders/planeParts.hpp"

void PlaneRender::Init()
{
    PushToVerts({-0.5f * m_scale, -0.5f * m_scale, 0.0f,     0.0f, 0.0f, -1.0f,   1.0f, 0.0f, 1.0f,    0.0f, 0.0f});
    PushToVerts({ 0.5f * m_scale, -0.5f * m_scale, 0.0f,     0.0f, 0.0f, -1.0f,   1.0f, 1.0f, 0.0f,    1.0f, 0.0f});
    PushToVerts({ 0.5f * m_scale,  0.5f * m_scale, 0.0f,     0.0f, 0.0f, -1.0f,   0.0f, 1.0f, 1.0f,    1.0f, 1.0f});
    PushToVerts({-0.5f * m_scale,  0.5f * m_scale, 0.0f,     0.0f, 0.0f, -1.0f,   1.0f, 1.0f, 1.0f,    0.0f, 1.0f});

    PushToInds({0, 1, 2});
    PushToInds({0, 2, 3});

    m_has_normals = true;
    m_has_color_texture = true;
}