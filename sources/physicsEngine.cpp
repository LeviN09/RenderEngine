#include "../headers/physicsEngine.hpp"

#include <cmath>
#include <glm/geometric.hpp>

void PhysicsEngine::AddObject(PhysicsObject *object) {
    objects.push_back(object);
    renderer->AddObject(object);
}

void PhysicsEngine::Update() {
    GravityUpdate();
    CollisionUpdate();

    for (auto& obj : objects) {
        std::cout << "veloc " << obj->velocity.x << " " << obj->velocity.y << " " << obj->velocity.z << std::endl;
        //std::cout << "gravy " << obj->gravity.x << " " << obj->gravity.y << " " << obj->gravity.z << std::endl;
        
        if(!obj->isColliding) {
            obj->acceleration = obj->SumAcceleration();
            obj->velocity += obj->acceleration;
            obj->position += obj->velocity;
        }
        (obj->GetRenderObject())->SetPos(obj->position);
    }
}

void PhysicsEngine::CollisionUpdate() {
    for (auto& obj : objects) {
        if (!obj->GetCollision()) { continue; }

        obj->normalAcc = glm::vec3(0.0f);
        for (auto& other : objects) {
            if (obj == other) { continue; }
            if (!other->GetCollision()) { continue; }
            CalcCollision((SphereBody*)obj, (SphereBody*)other);
        }
    }
}

void PhysicsEngine::CalcCollision(SphereBody* a, SphereBody* b) {
    glm::vec3 diffV = (a->position + a->velocity) - (b->position + b->velocity);
    float diffMag = glm::length(diffV);

    float diff = (a->radius + b->radius) - diffMag;
    glm::vec3 push = b->mass * glm::normalize(diffV);
    if (diff > 0.0f) {
        //std::cout << "push " << diffV.x << " " << diffV.y << " " << diffV.z << " " << a->isColliding << std::endl;

        //a->position += push;
        //b->position -= push;
        a->normalAcc += push / 10000.0f;
        //b->normalAcc -= push / 10000.0f;
        //a->velocity = glm::vec3(0.0f);
        //b->velocity = glm::vec3(0.0f);
        //a->isColliding = true;
        std::cout << "collide" << std::endl;
    }
}

void PhysicsEngine::GravityUpdate() {
    float gravConstant{1 / 10000.0f};

    for (auto& obj : objects) {
        if (!obj->GetGravity()) { continue; }

        obj->gravityAcc = glm::vec3(0.0f);
    }

    for (auto& obj : objects) {
        if (!obj->GetGravity()) { continue; }

        for (auto& other : objects) {
            if (obj == other) { continue; }
            if (!other->GetGravity()) { continue; }

            glm::vec3 diffV = obj->position - other->position;
            float diffMag = glm::length(diffV);
            float mag = gravConstant * (other->mass) / (diffMag * diffMag);

            glm::vec3 push = glm::normalize(diffV) * mag;


            obj->gravityAcc -= push;
        }
        //std::cout << "gravyc " << obj->gravity.x << " " << obj->gravity.y << " " << obj->gravity.z << std::endl;
    }
}