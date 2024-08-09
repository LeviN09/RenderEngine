#pragma once

#include <memory>
#include <unordered_set>

#include "physics/physicsEngine.hpp"
#include "render/renderer.hpp"
#include "scene/sceneObject.hpp"

class SceneOrchestrator
{
    public:
        SceneOrchestrator(Renderer& renderer, PhysicsEngine& physics_engine):
            m_renderer(renderer), m_physics_engine(physics_engine)
        {}

        void AddPrimitive(const SceneObject& object);

    private:
        Renderer& m_renderer;
        PhysicsEngine& m_physics_engine;

        std::unordered_set<std::unique_ptr<SceneObject>> m_objects;
};