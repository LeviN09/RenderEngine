#include <cstdint>
#include <glm/ext/vector_int2.hpp>
#include <glm/geometric.hpp>
#include <iostream>
#include <memory>
#include <ostream>
#include <sstream>
#include <vector>

#include "scene/generators/heightmapGenerator.hpp"
#include "render/gpu_interface/shaderClass.hpp"
#include "scene/primitives/heightmap.hpp"

void HeightmapGenerator::AllGen()
{
    glm::vec2 displacement = m_tile_size * glm::vec2{ m_size - 1 } / 2.0f;

    for (uint64_t i = 0; i < m_size.x; ++i)
    {
        for (uint64_t j = 0; j < m_size.y; ++j)
        {
            std::stringstream uid{"asd"};
            uid << i * m_size.x + j;

            float_t x_pos = i * m_tile_size - displacement.x;
            float_t z_pos = j * m_tile_size - displacement.y;

            glm::ivec2 tile_coord = glm::ivec2(x_pos / m_tile_size, z_pos / m_tile_size);

            m_objects.push_back(MapObject{std::make_unique<HeightmapObject>(m_renderer, m_engine, uid.str(),
                glm::vec3(x_pos, 0.0f, z_pos),
                m_tile_size, m_resolution, m_sampler,
                glm::vec2(x_pos, z_pos)), tile_coord});
            m_objects.back().object->AddRenderObject();
        }
    }
}

void HeightmapGenerator::Update()
{
    glm::vec3 true_cam_pos{ m_camera.GetPosition() };

    std::vector<MapObject>::iterator iter{m_objects.begin()};
    glm::vec2 cam_pos{true_cam_pos.x, true_cam_pos.z};
    while (iter != m_objects.end())
    {
        glm::vec2 obj_pos{iter->object->GetPos().x, iter->object->GetPos().z};
        float_t len = glm::length(obj_pos - cam_pos);
        if (len > m_far_clip * 2.0f)
        {
            iter = m_objects.erase(iter);
        }
        else
        {
/*             auto& obj = m_renderer.GetObject<HeightmapRender>("r_" + iter->object->GetUid());
            uint64_t expected_res = glm::min(len / 10.0f, 100.0f);
            if (expected_res != obj.GetResolution())
            {
                obj.ChangeResolution(expected_res);
                std::cout << "brb" << obj.GetResolution() << std::endl;
            } */

            ++iter;
        }
    }

    int64_t cam_tile_x = true_cam_pos.x / m_tile_size;
    int64_t cam_tile_y = true_cam_pos.z / m_tile_size;
    glm::ivec2 cam_coord{ cam_tile_x, cam_tile_y };

    for (int64_t i = -m_size.x; i < m_size.x; ++i)
    {
        for (int64_t j = -m_size.y; j < m_size.y; ++j)
        {
            glm::ivec2 tile_coord{ cam_coord - m_size / 2 };
            tile_coord.x += i + m_size.x / 2;
            tile_coord.y += j + m_size.y / 2;

            if (glm::length(glm::vec2(cam_coord) - glm::vec2(tile_coord)) < m_far_clip)
            {
                bool exists{ false };
                for (auto& item : m_objects)
                {
                    if (item.tile_coord == tile_coord)
                    {
                        exists = true;
                        break;
                    }
                }

                if (!exists)
                {
                    glm::vec2 tile_pos{tile_coord};
                    tile_pos *= m_tile_size;
                    m_objects.push_back(MapObject{std::make_unique<HeightmapObject>(m_renderer, m_engine, HashCoord(tile_pos.x, tile_pos.y),
                        glm::vec3(tile_pos.x, 0.0f, tile_pos.y),
                        m_tile_size, m_resolution, m_sampler,
                        glm::vec2(tile_pos.x, tile_pos.y)), tile_coord});
                    m_objects.back().object->AddRenderObject(ShaderType::Cellshade);
                }
            }
        }
    }
}

const std::string HeightmapGenerator::HashCoord(int64_t x, int64_t y)
{
    std::stringstream ss;
    ss << counter << "map_chunk" << (( y << 16 ) ^ x);
    counter++;
    return ss.str();
}