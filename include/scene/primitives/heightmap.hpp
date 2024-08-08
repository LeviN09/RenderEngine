#ifndef HEIGHTMAP_CLASS_HPP
#define HEIGHTMAP_CLASS_HPP

#include "scene/sceneObject.hpp"
#include <cmath>
#include <cstdint>
#include <functional>

class HeightmapObject : public SceneObject
{
    public:
        HeightmapObject(Renderer& renderer, PhysicsEngine& physics_engine, const std::string& uid, const glm::vec3& pos, float_t scale, const uint64_t& resolution, const std::function<float_t(float_t, float_t)>& sampler):
            SceneObject(renderer, physics_engine, uid, pos), m_scale{ scale }, m_resolution{ resolution }, m_sampler{ sampler }
        {}
        HeightmapObject(Renderer& renderer, PhysicsEngine& physics_engine, const std::string& uid, glm::mat4& model_mat, float_t scale, const uint64_t& resolution, const std::function<float_t(float_t, float_t)>& sampler):
            SceneObject(renderer, physics_engine, uid, model_mat), m_scale{ scale }, m_resolution{ resolution }, m_sampler{ sampler }
        {}
        ~HeightmapObject() {}

        void AddRenderObject(const ShaderType& type) override;
        void AddRenderObject() override;
        void AddPhysicsObject() override;

    private:
        float_t m_scale{ 1.0f };
        uint64_t m_resolution{ 0 };
        std::function<float_t(float_t, float_t)> m_sampler;
};

#endif