#ifndef PHYSICS_OBJECT_HPP
#define PHYSICS_OBJECT_HPP

#include <cmath>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/vec3.hpp>

#include "scene/idTag.hpp"

class PhysicsObject : public IdTag
{
    public:
        PhysicsObject(const std::string& uid, glm::mat4& model_mat, float_t mass):
            IdTag(uid), m_model_mat(model_mat), m_mass(mass) {}

        const glm::vec3 SumAcceleration() const;
        void Push(const glm::vec3& push);
        void Translate(const glm::vec3& translation);
        void SetVelocity(const glm::vec3& vel);
        void AddVelocity(const glm::vec3& vel);
        void SetCollision(bool has_collision);
        void SetColliding(bool is_colliding);
        void SetGravity(bool has_gravity);
        void SetAcc(const glm::vec3& acceleration);
        void SetNormalAcc(const glm::vec3& acc);
        void SetGravityAcc(const glm::vec3& acc);
        bool HasCollision() const;
        bool HasGravity() const;
        const glm::vec3 GetPosition() const;
        const glm::vec3 GetConstVelocity() const;
        const glm::vec3 GetVelocity() const;
        const glm::vec3 GetGravityAcc() const;
        const glm::vec3 GetNormalAcc() const;
        const glm::vec3 GetAcc() const;
        const float_t GetMass() const;
        const bool IsColliding() const;

    private:
    
        glm::mat4& m_model_mat;

        glm::vec3 m_const_velocity{ 0.0f };
        glm::vec3 m_velocity{ 0.0f };
        glm::vec3 m_gravity_acc{ 0.0f };
        glm::vec3 m_normal_acc{ 0.0f };
        glm::vec3 m_acceleration{ 0.0f };
        float_t m_mass;
        bool m_has_collision{ true }, m_has_gravity{ false };
        bool m_is_colliding;
};

#endif