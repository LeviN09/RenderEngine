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
            Update();
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
    });
}

void HeightmapGenerator::EndUpdate()
{
    m_is_running = false;
    m_update_thread.join();
}

void HeightmapGenerator::Update()
{
    glm::vec3 true_cam_pos{ m_camera.GetPosition() };
    glm::vec2 cam_pos{true_cam_pos.x, true_cam_pos.z};

    m_lock.lock();
    std::vector<MapObject>::iterator iter{m_objects.begin()};
    while (iter != m_objects.end())
    {
        glm::vec2 obj_pos{iter->object->GetPos().x, iter->object->GetPos().z};
        float_t len = glm::length(obj_pos - cam_pos);
        if (len > m_far_clip)
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
    m_lock.unlock();

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

            if (glm::length(glm::vec2(cam_coord) - glm::vec2(tile_coord)) * m_tile_size < m_far_clip)
            {
                bool exists{ false };
                m_lock.lock();
                for (auto& item : m_objects)
                {
                    if (item.tile_coord == tile_coord)
                    {
                        exists = true;
                        break;
                    }
                }
                m_lock.unlock();

                if (!m_is_running)
                {
                    return;
                }

                if (!exists)
                {
                    glm::vec2 tile_pos{tile_coord};
                    tile_pos *= m_tile_size;
                    m_lock.lock();
                    m_objects.push_back(MapObject{std::make_unique<HeightmapObject>(m_renderer, m_engine, HashCoord(tile_pos.x, tile_pos.y),
                        glm::vec3(tile_pos.x, 0.0f, tile_pos.y),
                        m_tile_size, m_resolution, m_sampler,
                        glm::vec2(tile_pos.x, tile_pos.y)), tile_coord});
                    m_objects.back().object->AddRenderObject(ShaderType::Cellshade);
                    m_lock.unlock();
                }
            }
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