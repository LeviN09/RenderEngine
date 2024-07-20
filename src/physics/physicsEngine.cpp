#include <algorithm>
#include <cmath>
#include <glm/geometric.hpp>

#include "physics/physicsEngine.hpp"
#include "physics/physicsObject.hpp"
#include "scene/primitives/object_builders/sphereParts.hpp"

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

void PhysicsEngine::Update()
{
    GravityUpdate();
    CollisionUpdate();

    for (const auto& obj : m_objects)
    {
        //std::cout << "veloc " << obj->velocity.x << " " << obj->velocity.y << " " << obj->velocity.z << std::endl;
        //std::cout << "gravy " << obj->gravity.x << " " << obj->gravity.y << " " << obj->gravity.z << std::endl;
        
        if(!obj->IsColliding())
        {
            obj->SetAcc(obj->SumAcceleration());
            obj->Push(obj->GetAcc());
            obj->Translate(obj->GetVelocity());
        }

        //(obj->GetRenderObject()).SetPos(obj.get().position);
    }
}

void PhysicsEngine::CollisionUpdate()
{
    for (auto& obj : m_objects)
    {
        if (!obj->HasCollision()) { continue; }

        obj->SetNormalAcc(glm::vec3(0.0f));

        for (auto& other : m_objects)
        {
            if (obj.get() == other.get()) { continue; }
            if (!other->HasCollision()) { continue; }
            CalcCollision((SphereBody&)*obj, (SphereBody&)*other);
        }
    }
}

void PhysicsEngine::CalcCollision(SphereBody& a, const SphereBody& b)
{
    glm::vec3 diffV = (a.GetPosition() + a.GetVelocity()) - (b.GetPosition() + b.GetVelocity());
    float diffMag = glm::length(diffV);

    float diff = (a.GetRadius() + b.GetRadius()) - diffMag;
    glm::vec3 push = b.GetMass() * glm::normalize(diffV);

    if (diff > 0.0f)
    {
        //std::cout << "push " << diffV.x << " " << diffV.y << " " << diffV.z << " " << a->isColliding << std::endl;

        //a->position += push;
        //b->position -= push;
        a.SetNormalAcc(a.GetNormalAcc() + push / 10000.0f);
        //b->normalAcc -= push / 10000.0f;
        //a->velocity = glm::vec3(0.0f);
        //b->velocity = glm::vec3(0.0f);
        //a->isColliding = true;
        //std::cout << "collide" << std::endl;
    }
}

void PhysicsEngine::GravityUpdate()
{
    static const float gravConstant{1 / 10000.0f};

    for (auto& obj : m_objects)
    {
        if (!obj->HasGravity())
        {
            continue;
        }

        obj->SetGravityAcc(glm::vec3(0.0f));
    }

    for (auto& obj : m_objects)
    {
        if (!obj->HasGravity())
        {
            continue;
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