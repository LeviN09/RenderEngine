#pragma once

#include <functional>
#include <glm/ext/vector_float3.hpp>
#include <memory>
#include <vector>

#include "physics/physicsObject.hpp"
#include "scene/iTaggedObjectManager.hpp"

class PhysicsEngine : public ITaggedObjectManager
{
    public:
        PhysicsEngine() = default;
        ~PhysicsEngine() = default;

        std::optional<std::reference_wrapper<IdTag>> SearchObject(const std::string& uid) const override;

        template <typename T>
        T& GetObject(const std::string& uid);

        void AddObject(std::unique_ptr<PhysicsObject> object);
        void RemoveObject(const std::string& uid);
        void Update(const double_t& delta_time);

    private:
        void CollisionUpdate(const double_t& delta_time);
        void GravityUpdate();

        std::vector<std::unique_ptr<PhysicsObject>> m_objects;
};

template <typename T>
T& PhysicsEngine::GetObject(const std::string& uid)
{
    for (const auto& item : m_objects)
    {
        if (item->GetUid() == uid)
        {
            T* castedItem = dynamic_cast<T*>(item.get());
            if (castedItem)
            {
                return *castedItem;
            }
            throw std::runtime_error("Item [" + uid + "] found but cast failed");
        }
    }
    throw std::runtime_error("Item [" + uid + "] not found");
}