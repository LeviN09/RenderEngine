#include <cstdint>
#include <glm/geometric.hpp>
#include <glm/gtc/quaternion.hpp>

#include "scene/primitives/object_builders/cubeParts.hpp"
#include "scene/primitives/object_builders/planeParts.hpp"
#include "scene/primitives/object_builders/sphereParts.hpp"

void SphereRender::Init()
{
    const float deltaRowAngle = M_PI / m_rows;
    const float deltaColumnAngle = 2 * M_PI / m_cols;

    PushToVerts({0.0f, m_radius, 0.0f,  0.0f, m_radius, 0.0f,  1.0f, 1.0f, 1.0f});
    for (int i = 0; i < m_rows - 1; i++)
    {
        float mini_radius = (float)sin((i + 1) * deltaRowAngle) * m_radius;
        for (int j = 0; j < m_cols; j++)
        {
            PushToVerts({
                (float)cos(j * deltaColumnAngle) * mini_radius,
                (float)cos((i + 1) * deltaRowAngle) * m_radius,
                (float)sin(j * deltaColumnAngle) * mini_radius,
                (float)cos(j * deltaColumnAngle) * mini_radius,
                (float)cos((i + 1) * deltaRowAngle) * m_radius,
                (float)sin(j * deltaColumnAngle) * mini_radius,
                i * 1.0f / m_rows, j * 1.0f / m_cols, 1.0f
            });
        }
    }
    PushToVerts({0.0f, -m_radius, 0.0f, 0.0f, -m_radius, 0.0f,  1.0f, 1.0f, 1.0f});

    for (uint64_t i = 0; i < m_cols - 1; i++)
    {
        PushToInds({0, i + 2, i + 1});
    }
    PushToInds({0, 1, m_cols});
    for (int i = 0; i < m_rows - 2; i++)
    {
        for (int j = 0; j < m_cols - 1; j++)
        {
            PushToInds({i * m_cols + j + 1, (i + 1) * m_cols + j + 1, (i + 1) * m_cols + j + 2});
            PushToInds({i * m_cols + j + 1, (i + 1) * m_cols + j + 2, i * m_cols + j + 2});
        }
        PushToInds({i * m_cols + 1, (i + 1) * m_cols + 1, (i + 1) * m_cols});
        PushToInds({(i + 1) * m_cols + 1, (i + 2) * m_cols, (i + 1) * m_cols});
    }
    for (int i = 0; i < m_cols - 1; i++)
    {
        PushToInds({(m_rows - 2) * m_cols + i + 1, (m_rows - 1) * m_cols + 1, (m_rows - 2) * m_cols + i + 2});
    }
    PushToInds({(m_rows - 1) * m_cols, (m_rows - 1) * m_cols + 1, (m_rows - 2) * m_cols + 1});

    m_has_normals = true;
    m_has_color_texture = false;
}

void SphereRender::Update(const double_t& delta_time)
{

}

float_t SphereBody::GetRadius() const
{
    return m_radius;
}

const bool SphereBody::IsCollidingWith(const PhysicsObject& other) const
{
    return other.IsCollidingWith(*this);
}

const bool SphereBody::IsCollidingWith(const SphereBody& other) const
{
    glm::vec3 direction = other.GetPosition() - GetPosition();
    float_t distance = length(direction);
    float_t totalRadius = m_radius + other.m_radius;

    return distance < totalRadius;
}

const bool SphereBody::IsCollidingWith(const PlaneBody& other) const
{
    return false;
}

const bool SphereBody::IsCollidingWith(const CubeBody& other) const
{
    glm::vec3 closest_point;
    
    for (int i = 0; i < 3; i++)
    {
        float coordinate = GetPosition()[i];
        if (coordinate < other.GetPosition()[i] - other.GetHalfExtent()[i])
        {
            closest_point[i] = other.GetPosition()[i] - other.GetHalfExtent()[i];
        }
        else if (coordinate > other.GetPosition()[i] + other.GetHalfExtent()[i])
        {
            closest_point[i] = other.GetPosition()[i] + other.GetHalfExtent()[i];
        }
        else
        {
            closest_point[i] = coordinate;
        }
    }
    
    glm::vec3 difference = closest_point - GetPosition();
    float distanceSquared = glm::length2(difference);
    
    return distanceSquared <= (m_radius * m_radius);
}

void SphereBody::CalcCollision(PhysicsObject& other)
{
    other.CalcCollision(*this);
}

void SphereBody::CalcCollision(SphereBody& other)
{
    glm::vec3 direction = other.GetPosition() - GetPosition();
    float_t distance = length(direction);
    float_t totalRadius = m_radius + other.m_radius;

    if (distance >= totalRadius)
    {
        return;
    }

/*
    glm::vec3 collision_point = GetPosition() + direction / 2.0f;
    glm::vec3 normal = glm::normalize(direction);

    glm::vec3 ra = collision_point - GetPosition();
    glm::vec3 rb = collision_point - other.GetPosition();
    glm::vec3 relativeVelocity = (other.m_velocity + glm::cross(other.m_angular_velocity, rb)) - 
                               (m_velocity + glm::cross(m_angular_velocity, ra));

    float elasticity = 0.8f; // Coefficient of restitution
    float j = -(1 + elasticity) * glm::dot(relativeVelocity, normal);
    j /= glm::dot(normal, normal * (1/m_mass + 1/other.m_mass)) + 
         glm::dot(normal, glm::cross(m_inverse_inertia_tensor * glm::cross(ra, normal), ra) + 
                     glm::cross(other.m_inverse_inertia_tensor * glm::cross(rb, normal), rb));

    glm::vec3 impulse = j * normal;
    m_velocity -= impulse / m_mass;
    other.m_velocity += impulse / other.m_mass;

    m_angular_velocity -= m_inverse_inertia_tensor * glm::cross(ra, impulse);
    other.m_angular_velocity += other.m_inverse_inertia_tensor * glm::cross(rb, impulse);
*/

    direction /= distance;

    float_t overlap = totalRadius - distance;

    float_t normalForceMagnitude = m_spring_constant * overlap;
    glm::vec3 normalForce = direction * normalForceMagnitude;

    glm::vec3 acceleration1 = normalForce / m_mass;
    m_normal_acc -= acceleration1;
}

void SphereBody::CalcCollision(CubeBody& other)
{
    glm::vec3 normal;
    float penetrationDepth;

    glm::vec3 closest_point;

    for (int i = 0; i < 3; i++)
    {
        float coordinate = GetPosition()[i];
        closest_point[i] = glm::clamp(coordinate, 
                                     other.GetPosition()[i] - other.GetHalfExtent()[i], 
                                     other.GetPosition()[i] + other.GetHalfExtent()[i]);
    }

    glm::vec3 difference = GetPosition() - closest_point;
    float distanceSquared = glm::length2(difference);

    if (distanceSquared > m_radius * m_radius)
    {
        return;
    }

    float distance = std::sqrt(distanceSquared);
    normal = distance > 0 ? difference / distance : glm::vec3(1, 0, 0);
    penetrationDepth = m_radius - distance;

    glm::vec3 relativeVelocity = m_velocity - other.GetVelocity();

    float velocityAlongNormal = glm::dot(relativeVelocity, normal);

    if (velocityAlongNormal > 0)
    {
        return;
    }

    float restitution = 0.5f;

    float impulseScalar = -(1 + restitution) * velocityAlongNormal;
    impulseScalar /= 1/m_mass + 1/other.GetMass();

    glm::vec3 impulse = normal * impulseScalar;
    m_velocity += impulse / m_mass;

    if (m_velocity.x < m_error_margin)
    {
        m_velocity.x = 0.0f;
    }
    if (m_velocity.y < m_error_margin)
    {
        m_velocity.y = 0.0f;
    }
    if (m_velocity.z < m_error_margin)
    {
        m_velocity.z = 0.0f;
    }

    float percent = 0.8f;
    float slop = 0.01f;
    glm::vec3 correction = std::max(penetrationDepth - slop, 0.0f) / (1/m_mass + 1/other.GetMass()) * percent * normal;
    Translate(correction * (1/m_mass));
}