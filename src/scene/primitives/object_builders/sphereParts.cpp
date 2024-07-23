#include <cstdint>

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

    direction /= distance;

    float_t overlap = totalRadius - distance;

    float_t normalForceMagnitude = m_spring_constant * overlap;
    glm::vec3 normalForce = direction * normalForceMagnitude;

    glm::vec3 acceleration1 = normalForce / m_mass;
    m_normal_acc -= acceleration1;
}