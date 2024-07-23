#include <glm/ext/matrix_transform.hpp>

#include "physics/physicsObject.hpp"

void PhysicsObject::SetCollision(bool has_collision)
{
    m_has_collision = has_collision;
}

bool PhysicsObject::HasCollision() const
{
    return m_has_collision;
}

void PhysicsObject::SetColliding(bool is_colliding)
{
    m_is_colliding = is_colliding;
}

const bool PhysicsObject::IsColliding() const
{
    return m_is_colliding;
}

void PhysicsObject::SetGravity(bool has_gravity)
{
    m_has_gravity = has_gravity;
}

bool PhysicsObject::HasGravity() const
{
    return m_has_gravity;
}

void PhysicsObject::SetUniversalGravity(bool has_universal_gravity)
{
    m_has_universal_gravity = has_universal_gravity;
}

bool PhysicsObject::HasUniversalGravity() const
{
    return m_has_universal_gravity;
}

const glm::vec3 PhysicsObject::SumAcceleration() const
{
    return m_normal_acc + m_gravity_acc;
}

void PhysicsObject::Push(const glm::vec3& push)
{
    m_velocity += push;
}

void PhysicsObject::SetPos(const glm::vec3& to_pos)
{
    glm::vec3 pos = glm::vec3(m_model_mat[3]);

    m_model_mat = glm::translate(m_model_mat, to_pos - pos);
}

void PhysicsObject::Translate(const glm::vec3& translation)
{
    m_model_mat = glm::translate(m_model_mat, translation);
}

void PhysicsObject::SetVelocity(const glm::vec3& vel)
{
    m_const_velocity = vel;
}

void PhysicsObject::AddVelocity(const glm::vec3& vel)
{
    m_const_velocity += vel;
}

const glm::vec3 PhysicsObject::GetPosition() const
{
    return glm::vec3(m_model_mat[3][0], m_model_mat[3][1], m_model_mat[3][2]);
}

const glm::vec3 PhysicsObject::GetConstVelocity() const
{
    return m_const_velocity;
}

const glm::vec3 PhysicsObject::GetVelocity() const
{
    return m_velocity;
}

const glm::vec3 PhysicsObject::GetAngularVelocity() const
{
    return m_angular_velocity;
}

const glm::vec3 PhysicsObject::GetGravityAcc() const
{
    return m_gravity_acc;
}

const glm::vec3 PhysicsObject::GetNormalAcc() const
{
    return m_normal_acc;
}

const glm::vec3 PhysicsObject::GetAcc() const
{
    return m_acceleration;
}

void PhysicsObject::SetAcc(const glm::vec3& acceleration)
{
    m_acceleration = acceleration;
}

void PhysicsObject::SetNormalAcc(const glm::vec3& acc)
{
    m_normal_acc = acc;
}

void PhysicsObject::SetGravityAcc(const glm::vec3& acc)
{
    m_gravity_acc = acc;
}

const float_t PhysicsObject::GetMass() const
{
    return m_mass;
}

void PhysicsObject::Update(const double_t& delta_time)
{
    m_acceleration = SumAcceleration();
    m_velocity += m_acceleration * (float_t)delta_time;
    Translate(m_velocity * (float_t)delta_time);
}