#include "scene/primitives/object_builders/cubeParts.hpp"

void CubeRender::Init()
{
    PushToVerts({-0.5f * m_scale, -0.5f * m_scale,  0.5f * m_scale,   -0.5f * m_scale, -0.5f * m_scale,  0.5f * m_scale,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f});
    PushToVerts({ 0.5f * m_scale, -0.5f * m_scale,  0.5f * m_scale,    0.5f * m_scale, -0.5f * m_scale,  0.5f * m_scale,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f});
    PushToVerts({ 0.5f * m_scale, -0.5f * m_scale, -0.5f * m_scale,    0.5f * m_scale, -0.5f * m_scale, -0.5f * m_scale,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f});
    PushToVerts({-0.5f * m_scale, -0.5f * m_scale, -0.5f * m_scale,   -0.5f * m_scale, -0.5f * m_scale, -0.5f * m_scale,   0.0f, 1.0f, 1.0f,   0.0f, 0.0f});
    PushToVerts({-0.5f * m_scale,  0.5f * m_scale,  0.5f * m_scale,   -0.5f * m_scale,  0.5f * m_scale,  0.5f * m_scale,   1.0f, 0.0f, 1.0f,   0.0f, 1.0f});
    PushToVerts({ 0.5f * m_scale,  0.5f * m_scale,  0.5f * m_scale,    0.5f * m_scale,  0.5f * m_scale,  0.5f * m_scale,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f});
    PushToVerts({ 0.5f * m_scale,  0.5f * m_scale, -0.5f * m_scale,    0.5f * m_scale,  0.5f * m_scale, -0.5f * m_scale,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f});
    PushToVerts({-0.5f * m_scale,  0.5f * m_scale, -0.5f * m_scale,   -0.5f * m_scale,  0.5f * m_scale, -0.5f * m_scale,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f});

    PushToInds({0, 1, 4});
    PushToInds({1, 5, 4});
    PushToInds({1, 2, 5});
    PushToInds({2, 6, 5});
    PushToInds({2, 3, 6});
    PushToInds({3, 7, 6});
    PushToInds({3, 0, 7});
    PushToInds({0, 4, 7});
    PushToInds({2, 1, 0});
    PushToInds({0, 3, 2});
    PushToInds({4, 5, 6});
    PushToInds({4, 6, 7});

    m_has_normals = true;
    m_has_color_texture = true;
}

void CubeRender::Update(const double_t& delta_time)
{

}