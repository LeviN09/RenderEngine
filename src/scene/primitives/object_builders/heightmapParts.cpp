#include "scene/primitives/object_builders/cubeParts.hpp"
#include "scene/primitives/object_builders/heightmapParts.hpp"
#include "scene/primitives/object_builders/planeParts.hpp"
#include "scene/primitives/object_builders/sphereParts.hpp"
#include <cmath>
#include <cstdint>
#include <glm/geometric.hpp>

void HeightmapRender::Init()
{
    const uint64_t verts = 6; 
    uint64_t corrected_res = m_resolution + 2;
    float_t tile_size = m_scale / static_cast<float_t>(corrected_res);
    float_t displacement = m_scale / 2.0f;

    float_t percent = 1.0f / static_cast<float_t>(corrected_res);

    auto to_tile = [&](uint64_t var){ return static_cast<float_t>(var) * tile_size; };

    for (uint64_t i = 0; i < corrected_res; ++i)
    {
        for (uint64_t j = 0; j < corrected_res; ++j)
        {
            glm::vec3 top_l{to_tile(i    ) - displacement, m_sampler(to_tile(i    ) - displacement, to_tile(j    ) - displacement), to_tile(j    ) - displacement};
            glm::vec3 bot_l{to_tile(i    ) - displacement, m_sampler(to_tile(i    ) - displacement, to_tile(j + 1) - displacement), to_tile(j + 1) - displacement};
            glm::vec3 top_r{to_tile(i + 1) - displacement, m_sampler(to_tile(i + 1) - displacement, to_tile(j    ) - displacement), to_tile(j    ) - displacement};
            glm::vec3 bot_r{to_tile(i + 1) - displacement, m_sampler(to_tile(i + 1) - displacement, to_tile(j + 1) - displacement), to_tile(j + 1) - displacement};

            glm::vec3 top_l_norm = GetNormal(to_tile(i    ), to_tile(j    ));
            glm::vec3 bot_l_norm = GetNormal(to_tile(i    ), to_tile(j + 1));
            glm::vec3 top_r_norm = GetNormal(to_tile(i + 1), to_tile(j    ));
            glm::vec3 bot_r_norm = GetNormal(to_tile(i + 1), to_tile(j + 1));

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
    uint64_t corrected_res = m_resolution + 2;
    float_t tile_size = m_scale / static_cast<float_t>(corrected_res);

    glm::vec3 vert_diff = glm::normalize(glm::vec3(-tile_size * 2.0f, m_sampler(x - tile_size, y) - m_sampler(x + tile_size, y), 0.0f));
    glm::vec3 hori_diff = glm::normalize(glm::vec3(0.0f, m_sampler(x, y - tile_size) - m_sampler(x, y + tile_size), -tile_size * 2.0f));

    return glm::normalize(glm::cross(hori_diff, vert_diff));
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