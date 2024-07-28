#include "render/camera.hpp"
#include "scene/primitives/object_builders/cubeParts.hpp"
#include "scene/primitives/object_builders/planeParts.hpp"
#include "scene/primitives/object_builders/sphereParts.hpp"

void CubeRender::Init()
{
    PushToVerts({-0.5f * m_scale.x, -0.5f * m_scale.y,  0.5f * m_scale.z,   -0.5f * m_scale.x, -0.5f * m_scale.y,  0.5f * m_scale.z,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f});
    PushToVerts({ 0.5f * m_scale.x, -0.5f * m_scale.y,  0.5f * m_scale.z,    0.5f * m_scale.x, -0.5f * m_scale.y,  0.5f * m_scale.z,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f});
    PushToVerts({ 0.5f * m_scale.x, -0.5f * m_scale.y, -0.5f * m_scale.z,    0.5f * m_scale.x, -0.5f * m_scale.y, -0.5f * m_scale.z,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f});
    PushToVerts({-0.5f * m_scale.x, -0.5f * m_scale.y, -0.5f * m_scale.z,   -0.5f * m_scale.x, -0.5f * m_scale.y, -0.5f * m_scale.z,   0.0f, 1.0f, 1.0f,   0.0f, 0.0f});
    PushToVerts({-0.5f * m_scale.x,  0.5f * m_scale.y,  0.5f * m_scale.z,   -0.5f * m_scale.x,  0.5f * m_scale.y,  0.5f * m_scale.z,   1.0f, 0.0f, 1.0f,   0.0f, 1.0f});
    PushToVerts({ 0.5f * m_scale.x,  0.5f * m_scale.y,  0.5f * m_scale.z,    0.5f * m_scale.x,  0.5f * m_scale.y,  0.5f * m_scale.z,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f});
    PushToVerts({ 0.5f * m_scale.x,  0.5f * m_scale.y, -0.5f * m_scale.z,    0.5f * m_scale.x,  0.5f * m_scale.y, -0.5f * m_scale.z,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f});
    PushToVerts({-0.5f * m_scale.x,  0.5f * m_scale.y, -0.5f * m_scale.z,   -0.5f * m_scale.x,  0.5f * m_scale.y, -0.5f * m_scale.z,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f});

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

const glm::vec3& CubeBody::GetHalfExtent() const
{
    return m_half_extent;
}

const bool CubeBody::IsCollidingWith(const PhysicsObject& other) const
{
    return other.IsCollidingWith(*this);
}

const bool CubeBody::IsCollidingWith(const SphereBody& other) const
{
    glm::vec3 closest_point;
    
    for (int i = 0; i < 3; i++)
    {
        float coordinate = other.GetPosition()[i];
        if (coordinate < GetPosition()[i] - m_half_extent[i])
        {
            closest_point[i] = GetPosition()[i] - m_half_extent[i];
        }
        else if (coordinate > GetPosition()[i] + m_half_extent[i])
        {
            closest_point[i] = GetPosition()[i] + m_half_extent[i];
        }
        else
        {
            closest_point[i] = coordinate;
        }
    }
    
    glm::vec3 difference = closest_point - other.GetPosition();
    float distanceSquared = glm::length2(difference);
    
    return distanceSquared <= (other.GetRadius() * other.GetRadius());
}

const bool CubeBody::IsCollidingWith(const PlaneBody& other) const
{
    return false;
}

const bool CubeBody::IsCollidingWith(const CubeBody& other) const
{
    glm::vec3 normal{ 1.0f };
    glm::mat3 axes{ 1.0f };

    float minOverlap = std::numeric_limits<float>::max();
    glm::vec3 minAxis;

    for (uint64_t i = 0; i < axes.length(); ++i)
    {
        float proj1 = glm::dot(m_half_extent, axes[i]);
        float proj2 = glm::dot(other.GetHalfExtent(), axes[i]);

        glm::vec3 centerDiff = other.GetPosition() - GetPosition();
        float distance = std::abs(glm::dot(centerDiff, axes[i]));

        float overlap = proj1 + proj2 - distance;

        if (overlap <= 0)
        {
            return false;
        }

        if (overlap < minOverlap)
        {
            minOverlap = overlap;
            minAxis = axes[i];
        }
    }

    return true;
}

void CubeBody::CalcCollision(PhysicsObject& other)
{
    other.CalcCollision(*this);
}

void CubeBody::CalcCollision(SphereBody& other)
{
    glm::vec3 normal;
    float penetrationDepth;

    glm::vec3 closest_point;

    for (int i = 0; i < 3; i++)
    {
        float coordinate = other.GetPosition()[i];
        closest_point[i] = glm::clamp(coordinate, 
                                     GetPosition()[i] - m_half_extent[i], 
                                     GetPosition()[i] + m_half_extent[i]);
    }

    glm::vec3 difference = other.GetPosition() - closest_point;
    float distanceSquared = glm::length2(difference);

    if (distanceSquared > other.GetRadius() * other.GetRadius())
    {
        return;
    }

    float distance = std::sqrt(distanceSquared);
    normal = distance > 0 ? difference / distance : glm::vec3(1, 0, 0);
    penetrationDepth = other.GetRadius() - distance;

    glm::vec3 relativeVelocity = other.GetVelocity() - m_velocity;

    float velocityAlongNormal = glm::dot(relativeVelocity, normal);

    if (velocityAlongNormal > 0)
    {
        return;
    }

    float restitution = 0.5f;

    float impulseScalar = -(1 + restitution) * velocityAlongNormal;
    impulseScalar /= 1/m_mass + 1/other.GetMass();

    glm::vec3 impulse = normal * impulseScalar;
    m_velocity -= impulse / m_mass;

    float percent = 0.8f;
    float slop = 0.01f;
    glm::vec3 correction = std::max(penetrationDepth - slop, 0.0f) / (1/m_mass + 1/other.GetMass()) * percent * normal;
    Translate(-correction * (1/m_mass));
}

void CubeBody::CalcCollision(CubeBody& other)
{
    glm::vec3 normal{ 0.0f };
    float penetrationDepth{ 0.0f };
    glm::mat3 axes{ 1.0f };

    float minOverlap = std::numeric_limits<float>::max();
    glm::vec3 minAxis;

    for (uint64_t i = 0; i < axes.length(); ++i)
    {
        float proj1 = glm::dot(m_half_extent, axes[i]);
        float proj2 = glm::dot(other.GetHalfExtent(), axes[i]);

        glm::vec3 centerDiff = other.GetPosition() - GetPosition();
        float distance = std::abs(glm::dot(centerDiff, axes[i]));

        float overlap = proj1 + proj2 - distance;

        if (overlap <= 0)
        {
            return;
        }

        if (overlap < minOverlap)
        {
            minOverlap = overlap;
            minAxis = axes[i];
        }
    }

    normal = minAxis;
    if (glm::dot(normal, other.GetPosition() - GetPosition()) < 0)
    {
        normal = -normal;
    }
    penetrationDepth = minOverlap;
    
    glm::vec3 relativeVelocity = other.GetVelocity() - m_velocity;

    float velocityAlongNormal = glm::dot(relativeVelocity, normal);

    if (velocityAlongNormal > 0)
    {
        return;
    }

    float restitution = 0.5f;

    float impulseScalar = -(1 + restitution) * velocityAlongNormal;
    impulseScalar /= 1/m_mass + 1/other.GetMass();

    glm::vec3 impulse = normal * impulseScalar;
    m_velocity -= impulse / m_mass;

    float percent = 0.8f;
    float slop = 0.01f;
    glm::vec3 correction = std::max(penetrationDepth - slop, 0.0f) / (1/m_mass + 1/other.GetMass()) * percent * normal;
    Translate(-correction * (1/m_mass));
}