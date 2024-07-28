#include "scene/primitives/object_builders/cubeParts.hpp"
#include "scene/primitives/object_builders/planeParts.hpp"
#include "scene/primitives/object_builders/sphereParts.hpp"

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

const bool PlaneBody::IsCollidingWith(const PhysicsObject& other) const
{
    return other.IsCollidingWith(*this);
}

const bool PlaneBody::IsCollidingWith(const SphereBody& other) const
{
    return false;
}

const bool PlaneBody::IsCollidingWith(const PlaneBody& other) const
{
    return false;
}

const bool PlaneBody::IsCollidingWith(const CubeBody& other) const
{
    return false;
}

void PlaneBody::CalcCollision(PhysicsObject& other)
{

}

void PlaneBody::CalcCollision(SphereBody& other)
{

}

void PlaneBody::CalcCollision(CubeBody& other)
{

}