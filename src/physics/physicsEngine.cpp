#include <algorithm>
#include <cmath>
#include <glm/geometric.hpp>

#include "physics/physicsEngine.hpp"
#include "physics/physicsObject.hpp"

void PhysicsEngine::AddObject(std::unique_ptr<PhysicsObject> object)
{
    m_objects.push_back(std::move(object));
}

std::optional<std::reference_wrapper<IdTag>> PhysicsEngine::SearchObject(const std::string& uid) const
{
    const auto& search = std::find_if(m_objects.begin(), m_objects.end(), [&](const std::unique_ptr<PhysicsObject>& item){ return item->GetUid() == uid; });
    if (search != m_objects.end())
    {
        return *search->get();
    }

    return std::nullopt;
}

void PhysicsEngine::Update(const double_t& delta_time)
{
    GravityUpdate();
    CollisionUpdate(delta_time);

    for (const auto& obj : m_objects)
    {
        obj->Update(delta_time);
    }
}

void PhysicsEngine::CollisionUpdate(const double_t& delta_time)
{
    for (auto& obj : m_objects)
    {
        if (!obj->HasCollision()) { continue; }

        obj->SetNormalAcc(glm::vec3(0.0f));

        for (auto& other : m_objects)
        {
            if (obj.get() == other.get()) { continue; }
            if (!other->HasCollision()) { continue; }
            obj->CalcCollision(*other);
        }
    }
}

void PhysicsEngine::GravityUpdate()
{
    static const float gravConstant{1 / 10000.0f};

    for (auto& obj : m_objects)
    {
        obj->SetGravityAcc(glm::vec3(0.0f));

        if (!obj->HasGravity() && !obj->HasUniversalGravity())
        {
            continue;
        }

        if (obj->HasUniversalGravity())
        {
            obj->SetGravityAcc(obj->GetGravityAcc() + glm::vec3(0.0f, -9.8f, 0.0f));
        }

        for (auto& other : m_objects)
        {
            if (obj.get() == other.get())
            {
                continue;
            }

            if (!other->HasGravity())
            {
                continue;
            }

            glm::vec3 diffV = obj->GetPosition() - other->GetPosition();
            float diffMag = glm::length(diffV);
            float mag = gravConstant * (other->GetMass()) / (diffMag * diffMag);

            glm::vec3 push = glm::normalize(diffV) * mag;

            obj->SetGravityAcc(obj->GetGravityAcc() - push);
        }
        //std::cout << "gravyc " << obj->gravity.x << " " << obj->gravity.y << " " << obj->gravity.z << std::endl;
    }
}