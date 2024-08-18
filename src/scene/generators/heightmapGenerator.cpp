#include <algorithm>
#include <chrono>
#include <cstdint>
#include <glm/ext/vector_int2.hpp>
#include <glm/geometric.hpp>
#include <iostream>
#include <memory>
#include <mutex>
#include <ostream>
#include <sstream>
#include <thread>
#include <vector>

#include "scene/generators/heightmapGenerator.hpp"
#include "render/gpu_interface/shaderClass.hpp"
#include "scene/primitives/heightmap.hpp"

void HeightmapGenerator::StartUpdate()
{
    m_is_running = true;
    m_update_thread = std::thread([&](){
        while(m_is_running)
        {
            UpdateGen();
        }
    });

    if (!m_use_lod)
    {
        return;
    }

    m_lod_thread = std::thread([&](){
        while(m_is_running)
        {
            UpdateLod();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    });
}

void HeightmapGenerator::EndUpdate()
{
    m_is_running = false;
    m_update_thread.join();
    if (m_use_lod)
    {
        m_lod_thread.join();
    }
}

void HeightmapGenerator::UpdateLod()
{
    glm::vec3 true_cam_pos{ m_camera.GetPosition() };
    glm::vec2 cam_pos{true_cam_pos.x, true_cam_pos.z};

    m_lock.lock();
    for (auto iter = m_objects.begin(); iter != m_objects.end(); ++iter)
    {
        if (!m_is_running)
        {
            m_lock.unlock();
            return;
        }

        glm::vec2 obj_pos{iter->object->GetPos().x, iter->object->GetPos().z};
        float_t len = glm::length(obj_pos - cam_pos);

        uint64_t expected_res = ExpectedResolution(len);
        if (expected_res != iter->object->GetResolution())
        {
            iter->object->ChangeResolution(expected_res);
        }
    }
    m_lock.unlock();
}

void HeightmapGenerator::UpdateGen()
{
    glm::vec3 true_cam_pos{ m_camera.GetPosition() };
    glm::vec2 cam_pos{true_cam_pos.x, true_cam_pos.z};

    m_lock.lock();
    for (auto iter = m_objects.rbegin(); iter != m_objects.rend();)
    {
        if (!m_is_running)
        {
            m_lock.unlock();
            return;
        }

        glm::vec2 obj_pos{iter->object->GetPos().x, iter->object->GetPos().z};
        float_t len = glm::length(obj_pos - cam_pos);
        if (len > m_far_clip)
        {
            iter = std::reverse_iterator<std::vector<MapObject>::iterator>(m_objects.erase((iter + 1).base()));
        }
        else
        {
            ++iter;
        }
    } 
    m_lock.unlock();

    int64_t cam_tile_x = true_cam_pos.x / m_tile_size;
    int64_t cam_tile_y = true_cam_pos.z / m_tile_size;
    glm::ivec2 cam_coord{ cam_tile_x, cam_tile_y };

    std::vector<glm::ivec2> tile_coords = SpiralIterator(cam_coord);

    for (const auto& tile_coord : tile_coords)
    {
        if (!m_is_running)
        {
            return;
        }

        glm::vec2 tile_pos{tile_coord};
        tile_pos *= m_tile_size;
        float_t len = glm::length(tile_pos - cam_pos);
        if (len < m_far_clip)
        {
            m_lock.lock();
            bool exists =
                std::find_if(m_objects.begin(), m_objects.end(), [&](auto& item){ return item.tile_coord == tile_coord; }) != m_objects.end();

            if (!exists)
            {
                uint64_t expected_res{m_resolution};
                
                if (m_use_lod)
                {
                    expected_res = ExpectedResolution(len);
                }

                m_objects.push_back(MapObject{std::make_unique<HeightmapObject>(m_renderer, m_engine, HashCoord(tile_pos.x, tile_pos.y),
                    glm::vec3(tile_pos.x, 0.0f, tile_pos.y),
                    m_tile_size, expected_res, m_sampler,
                    glm::vec2(tile_pos.x, tile_pos.y)), tile_coord});
                m_objects.back().object->AddRenderObject(ShaderType::Cellshade);
            }
            m_lock.unlock();
        }
    }
}

const std::string HeightmapGenerator::HashCoord(int64_t x, int64_t y)
{
    std::stringstream ss;
    ss << m_counter << "map_chunk" << (( y << 16 ) ^ x);
    m_counter++;
    return ss.str();
}

std::vector<glm::ivec2> HeightmapGenerator::SpiralIterator(const glm::ivec2& center)
{
    std::vector<glm::ivec2> result;
    int64_t x = 0, y = 0;
    int64_t dx = 0, dy = -1;
    int64_t max_steps = 4 * std::max(m_size.x, m_size.y) * std::max(m_size.x, m_size.y);

    for (int64_t i = 0; i < max_steps; ++i)
    {
        if ((-m_size.x <= x && x < m_size.x) && (-m_size.y <= y && y < m_size.y))
        {
            result.push_back({center.x + x, center.y + y});
        }

        if (x == y || (x < 0 && x == -y) || (x > 0 && x == 1 - y))
        {
            int64_t temp = dx;
            dx = -dy;
            dy = temp;
        }

        x += dx;
        y += dy;

        if (x > m_size.x && y > m_size.y)
        {
            break;
        }
    }

    return result;
}

const uint64_t HeightmapGenerator::ExpectedResolution(const float_t& distance)
{
    uint64_t val = (uint64_t(100 * std::exp(-distance/1000.0f)) / 25) * 25;
    return val;
}