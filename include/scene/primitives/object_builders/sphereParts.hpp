#ifndef SPHEREPARTS_CLASS_HPP
#define SPHEREPARTS_CLASS_HPP

#include "physics/physicsObject.hpp"
#include "render/renderObject.hpp"

class SphereRender : public RenderObject
{
    public:
        SphereRender(const std::string& uid, const glm::mat4& model_mat, float_t radius, uint64_t rows, uint64_t cols):
            RenderObject(uid, model_mat), m_radius{ radius }, m_rows{ rows }, m_cols{ cols }
        {
            Init();
            Configure();
        }

    protected:
        float_t m_radius{ 0.0f };
        uint64_t m_rows{ 0 }, m_cols{ 0 };

    private:
        void Init() override;
        void Update(const double_t& delta_time) override;
};

class SphereBody : public PhysicsObject
{
    public:
        SphereBody(const std::string& uid, glm::mat4& model_mat, float radius, float mass):
            PhysicsObject(uid, model_mat, mass), m_radius(radius)
        {}

        float_t GetRadius() const;

        void CalcCollision(PhysicsObject& other) override;
        void CalcCollision(SphereBody& other) override;

    private:
        float_t m_radius;
};

#endif