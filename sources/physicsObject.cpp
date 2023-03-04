#include "../headers/physicsObject.hpp"

RenderObject* PhysicsObject::GetRenderObject() { return renderObj; }

void PhysicsObject::SetCollision(bool _hasCollision) { hasCollision = _hasCollision; }

bool PhysicsObject::GetCollision() { return hasCollision; }

void PhysicsObject::SetGravity(bool _hasGravity) { hasGravity = _hasGravity; }

bool PhysicsObject::GetGravity() { return hasGravity; }

glm::vec3 PhysicsObject::SumVelocity() {
    glm::vec3 sum(constVelocity);

    if (hasGravity)
        velocity += gravity;

    return sum + velocity;
}

void PhysicsObject::Push(glm::vec3 _push) { velocity += _push; }

void PhysicsObject::SetVelocity(glm::vec3 _vel) { constVelocity = _vel; }

void PhysicsObject::AddVelocity(glm::vec3 _vel) { constVelocity += _vel; }