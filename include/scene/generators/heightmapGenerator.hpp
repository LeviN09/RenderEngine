#pragma once

#include "physics/physicsEngine.hpp"
#include "render/camera.hpp"
#include "render/renderer.hpp"
#include "scene/primitives/heightmap.hpp"
#include <cmath>
#include <cstdint>
#include <functional>
#include <glm/ext/vector_float2.hpp>
#include <memory>
#include <thread>
#include <vector>

struct MapObject
{
    std::unique_ptr<HeightmapObject> object;
    glm::ivec2 tile_coord;
};

class HeightmapGenerator
{
    public:
        HeightmapGenerator(Renderer& renderer, PhysicsEngine& engine, const Camera& camera, const std::function<float_t(float_t, float_t)>& sampler, const glm::ivec2& size, bool use_lod):
            m_renderer{ renderer }, m_engine{ engine }, m_camera{ camera }, m_size{ size }, m_sampler{ sampler }, m_use_lod{ use_lod }
        {}
        ~HeightmapGenerator()
        {}

        void StartUpdate();
        void EndUpdate();

    private:
        void UpdateGen();
        void UpdateLod();

        const std::string HashCoord(int64_t x, int64_t y);
        std::vector<glm::ivec2> SpiralIterator(const glm::ivec2& center);
        const uint64_t ExpectedResolution(const float_t& distance);

        std::function<float_t(float_t, float_t)> m_sampler;
        glm::ivec2 m_size;
        float_t m_tile_size{ 100.0f };
        uint64_t m_resolution{ 50 };

        float_t m_far_clip{ 1300.0f };

        std::vector<MapObject> m_objects;

        Renderer& m_renderer;
        PhysicsEngine& m_engine;
        const Camera& m_camera;

        uint64_t m_counter{ 0 };

        bool m_is_running{ false };
        bool m_use_lod{ false };
        std::thread m_update_thread;
        std::thread m_lod_thread;
        std::mutex m_lock;
};