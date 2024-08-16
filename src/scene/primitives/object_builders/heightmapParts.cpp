#include "scene/primitives/object_builders/cubeParts.hpp"
#include "scene/primitives/object_builders/heightmapParts.hpp"
#include "scene/primitives/object_builders/planeParts.hpp"
#include "scene/primitives/object_builders/sphereParts.hpp"
#include <cmath>
#include <cstdint>
#include <glm/geometric.hpp>

void HeightmapRender::Init()
{
    if (m_is_shared)
    {
        InitShared();
    }
    else
    {
        InitSeparate();
    }
}

void HeightmapRender::InitShared()
{
    const uint64_t verts = 4; 
    uint64_t corrected_res = m_resolution + 2;
    float_t tile_size = m_scale / static_cast<float_t>(corrected_res);
    float_t displacement = m_scale / 2.0f;

    float_t percent = 1.0f / static_cast<float_t>(corrected_res);

    auto to_tile = [&](uint64_t var){ return static_cast<float_t>(var) * tile_size - displacement; };

    for (uint64_t i = 0; i < corrected_res; ++i)
    {
        for (uint64_t j = 0; j < corrected_res; ++j)
        {
            glm::vec3 top_l{to_tile(i    ), m_sampler(to_tile(i    ) + m_displacement.x, to_tile(j    ) + m_displacement.y), to_tile(j    )};
            glm::vec3 bot_l{to_tile(i    ), m_sampler(to_tile(i    ) + m_displacement.x, to_tile(j + 1) + m_displacement.y), to_tile(j + 1)};
            glm::vec3 top_r{to_tile(i + 1), m_sampler(to_tile(i + 1) + m_displacement.x, to_tile(j    ) + m_displacement.y), to_tile(j    )};
            glm::vec3 bot_r{to_tile(i + 1), m_sampler(to_tile(i + 1) + m_displacement.x, to_tile(j + 1) + m_displacement.y), to_tile(j + 1)};

            glm::vec3 top_l_norm = GetNormal(to_tile(i    ) + m_displacement.x, to_tile(j    ) + m_displacement.y);
            glm::vec3 bot_l_norm = GetNormal(to_tile(i    ) + m_displacement.x, to_tile(j + 1) + m_displacement.y);
            glm::vec3 top_r_norm = GetNormal(to_tile(i + 1) + m_displacement.x, to_tile(j    ) + m_displacement.y);
            glm::vec3 bot_r_norm = GetNormal(to_tile(i + 1) + m_displacement.x, to_tile(j + 1) + m_displacement.y);

            PushToVerts({top_l.x, top_l.y, top_l.z,    top_l_norm.x, top_l_norm.y, top_l_norm.z,    top_l.y / 50.0f + 0.5f, top_l.y / 50.0f - 0.3f, -top_l.y / 50.0f - 1.0f,    i       * percent, j       * percent});
            PushToVerts({bot_l.x, bot_l.y, bot_l.z,    bot_l_norm.x, bot_l_norm.y, bot_l_norm.z,    bot_l.y / 50.0f + 0.5f, bot_l.y / 50.0f - 0.3f, -bot_l.y / 50.0f - 1.0f,    i       * percent, (j + 1) * percent});
            PushToVerts({bot_r.x, bot_r.y, bot_r.z,    bot_r_norm.x, bot_r_norm.y, bot_r_norm.z,    bot_r.y / 50.0f + 0.5f, bot_r.y / 50.0f - 0.3f, -bot_r.y / 50.0f - 1.0f,    (i + 1) * percent, (j + 1) * percent});
            PushToVerts({top_r.x, top_r.y, top_r.z,    top_r_norm.x, top_r_norm.y, top_r_norm.z,    top_r.y / 50.0f + 0.5f, top_r.y / 50.0f - 0.3f, -top_r.y / 50.0f - 1.0f,    (i + 1) * percent, j       * percent});

            PushToInds({verts * (i * corrected_res + j) + 0, verts * (i * corrected_res + j) + 1, verts * (i * corrected_res + j) + 2});
            PushToInds({verts * (i * corrected_res + j) + 0, verts * (i * corrected_res + j) + 2, verts * (i * corrected_res + j) + 3});
        }
    }

    m_is_concave = true;
    m_has_normals = true;
    m_has_color_texture = true;
}

void HeightmapRender::InitSeparate()
{
    const uint64_t verts = 6; 
    uint64_t corrected_res = m_resolution + 2;
    float_t tile_size = m_scale / static_cast<float_t>(corrected_res);
    float_t displacement = m_scale / 2.0f;

    float_t percent = 1.0f / static_cast<float_t>(corrected_res);

    auto to_tile = [&](uint64_t var){ return static_cast<float_t>(var) * tile_size - displacement; };

    for (uint64_t i = 0; i < corrected_res; ++i)
    {
        for (uint64_t j = 0; j < corrected_res; ++j)
        {
            glm::vec3 top_l{to_tile(i    ), m_sampler(to_tile(i    ) + m_displacement.x, to_tile(j    ) + m_displacement.y), to_tile(j    )};
            glm::vec3 bot_l{to_tile(i    ), m_sampler(to_tile(i    ) + m_displacement.x, to_tile(j + 1) + m_displacement.y), to_tile(j + 1)};
            glm::vec3 top_r{to_tile(i + 1), m_sampler(to_tile(i + 1) + m_displacement.x, to_tile(j    ) + m_displacement.y), to_tile(j    )};
            glm::vec3 bot_r{to_tile(i + 1), m_sampler(to_tile(i + 1) + m_displacement.x, to_tile(j + 1) + m_displacement.y), to_tile(j + 1)};

            glm::vec3 top_l_norm = GetNormal(to_tile(i    ) + m_displacement.x, to_tile(j    ) + m_displacement.y);
            glm::vec3 bot_l_norm = GetNormal(to_tile(i    ) + m_displacement.x, to_tile(j + 1) + m_displacement.y);
            glm::vec3 top_r_norm = GetNormal(to_tile(i + 1) + m_displacement.x, to_tile(j    ) + m_displacement.y);
            glm::vec3 bot_r_norm = GetNormal(to_tile(i + 1) + m_displacement.x, to_tile(j + 1) + m_displacement.y);

            PushToVerts({top_l.x, top_l.y, top_l.z,    top_l_norm.x, top_l_norm.y, top_l_norm.z,    i       * percent, j       * percent, 0.0f,    i       * percent, j       * percent});
            PushToVerts({bot_l.x, bot_l.y, bot_l.z,    bot_l_norm.x, bot_l_norm.y, bot_l_norm.z,    i       * percent, (j + 1) * percent, 0.0f,    i       * percent, (j + 1) * percent});
            PushToVerts({top_r.x, top_r.y, top_r.z,    top_r_norm.x, top_r_norm.y, top_r_norm.z,    (i + 1) * percent, j       * percent, 0.0f,    (i + 1) * percent, j       * percent});

            PushToVerts({top_r.x, top_r.y, top_r.z,    top_r_norm.x, top_r_norm.y, top_r_norm.z,    (i + 1) * percent, j       * percent, 0.0f,    (i + 1) * percent, j       * percent});
            PushToVerts({bot_l.x, bot_l.y, bot_l.z,    bot_l_norm.x, bot_l_norm.y, bot_l_norm.z,    i       * percent, (j + 1) * percent, 0.0f,    i       * percent, (j + 1) * percent});
            PushToVerts({bot_r.x, bot_r.y, bot_r.z,    bot_r_norm.x, bot_r_norm.y, bot_r_norm.z,    (i + 1) * percent, (j + 1) * percent, 0.0f,    (i + 1) * percent, (j + 1) * percent});

            PushToInds({verts * (i * corrected_res + j) + 0, verts * (i * corrected_res + j) + 1, verts * (i * corrected_res + j) + 2});
            PushToInds({verts * (i * corrected_res + j) + 3, verts * (i * corrected_res + j) + 4, verts * (i * corrected_res + j) + 5});
        }
    }

    m_is_concave = true;
    m_has_normals = true;
    m_has_color_texture = true;
}

glm::vec3 HeightmapRender::GetNormal(float_t x, float_t y)
{
    float_t epsilon = 0.0001f;

    float_t h0 = m_sampler(x - epsilon, y - epsilon);
    float_t h1 = m_sampler(x + epsilon, y - epsilon);
    float_t h2 = m_sampler(x - epsilon, y + epsilon);
    float_t h3 = m_sampler(x + epsilon, y + epsilon);

    float_t dhdx = (h1 - h0) / (epsilon * 2.0f);
    float_t dhdy = (h2 - h0) / (epsilon * 2.0f);

    glm::vec3 normal = glm::normalize(glm::vec3(-dhdx, 1.0f, -dhdy));

    return normal;
}

const uint64_t& HeightmapRender::GetResolution()
{
    return m_resolution;
}

void HeightmapRender::ChangeResolution(const uint64_t& resolution)
{
    m_resolution = resolution;
    Init();
    Configure();
}

const bool HeightmapBody::IsCollidingWith(const PhysicsObject& other) const
{
    return false;
}

const bool HeightmapBody::IsCollidingWith(const SphereBody& other) const
{
    return false;
}

const bool HeightmapBody::IsCollidingWith(const PlaneBody& other) const
{
    return false;
}

const bool HeightmapBody::IsCollidingWith(const CubeBody& other) const
{
    return false;
}

void HeightmapBody::CalcCollision(PhysicsObject& other)
{

}

void HeightmapBody::CalcCollision(SphereBody& other)
{

}

void HeightmapBody::CalcCollision(CubeBody& other)
{

}