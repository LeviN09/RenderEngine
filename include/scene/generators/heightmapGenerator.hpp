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

struct MapObject
{
    std::unique_ptr<HeightmapObject> object;
    glm::ivec2 tile_coord;
};

class HeightmapGenerator
{
    public:
        HeightmapGenerator(Renderer& renderer, PhysicsEngine& engine, const Camera& camera, const std::function<float_t(float_t, float_t)>& sampler, const glm::ivec2& size):
            m_renderer{ renderer }, m_engine{ engine }, m_camera{ camera }, m_size{ size }, m_sampler{ sampler }
        {
            //AllGen();
        }

        void Update();

    private:
        void AllGen();
        const std::string HashCoord(int64_t x, int64_t y);

        std::function<float_t(float_t, float_t)> m_sampler;
        glm::ivec2 m_size;
        float_t m_tile_size{ 20.0f };
        uint64_t m_resolution{ 20 };

        float_t m_far_clip{ 50.0f };

        std::vector<MapObject> m_objects;

        Renderer& m_renderer;
        PhysicsEngine& m_engine;
        const Camera& m_camera;

        uint64_t counter{ 0 };
};