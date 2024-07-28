#ifndef PHYSICS_OBJECT_HPP
#define PHYSICS_OBJECT_HPP

#include <cmath>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/vec3.hpp>

#include "scene/idTag.hpp"

class SphereBody;
class CubeBody;
class PlaneBody;

class PhysicsObject : public IdTag
{
    public:
        const glm::vec3 SumAcceleration() const;
        void Push(const glm::vec3& push);
        void SetPos(const glm::vec3& to_pos);
        void Translate(const glm::vec3& translation);
        void SetVelocity(const glm::vec3& vel);
        void AddVelocity(const glm::vec3& vel);
        void SetCollision(bool has_collision);
        void SetGravity(bool has_gravity);
        void SetUniversalGravity(bool has_universal_gravity);
        void SetAcc(const glm::vec3& acceleration);
        void SetNormalAcc(const glm::vec3& acc);
        void SetGravityAcc(const glm::vec3& acc);

        bool HasCollision() const;
        bool HasGravity() const;
        bool HasUniversalGravity() const;

        const glm::vec3 GetPosition() const;
        const glm::vec3 GetConstVelocity() const;
        const glm::vec3 GetVelocity() const;
        const glm::vec3 GetAngularVelocity() const;
        const glm::vec3 GetGravityAcc() const;
        const glm::vec3 GetNormalAcc() const;
        const glm::vec3 GetAcc() const;
        const float_t GetMass() const;

        const bool IsColliding() const;
        virtual const bool IsCollidingWith(const PhysicsObject& other) const = 0;
        virtual const bool IsCollidingWith(const SphereBody& other) const = 0;
        virtual const bool IsCollidingWith(const PlaneBody& other) const = 0;
        virtual const bool IsCollidingWith(const CubeBody& other) const = 0;

        virtual void CalcCollision(PhysicsObject& other) = 0;
        virtual void CalcCollision(SphereBody& other) = 0;
        virtual void CalcCollision(CubeBody& other) = 0;
        void Update(const double_t& delta_time);

    protected:
        PhysicsObject(const std::string& uid, glm::mat4& model_mat, float_t mass):
            IdTag(uid), m_model_mat(model_mat), m_mass(mass)
        {}

        glm::mat4& m_model_mat;

        glm::mat3 m_inertia_tensor{ 1.0f };
        glm::mat3 m_inverse_inertia_tensor{ 1.0f };

        glm::vec3 m_const_velocity{ 0.0f };
        glm::vec3 m_velocity{ 0.0f };
        glm::vec3 m_angular_velocity{ 0.0f };

        glm::vec3 m_torque{ 0.0f };
        glm::vec3 m_gravity_acc{ 0.0f };
        glm::vec3 m_normal_acc{ 0.0f };
        glm::vec3 m_acceleration{ 0.0f };

        glm::quat orientation{ 0.0f, 0.0f, 0.0f, 0.0f };
        float_t m_mass;
        float_t m_spring_constant { 10000.0f }; 

        bool m_has_collision{ true }, m_has_gravity{ false }, m_has_universal_gravity{ true };
        bool m_is_colliding;
};

#endif