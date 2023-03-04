#ifndef SPHERE_CLASS_HPP
#define SPHERE_CLASS_HPP

#include "physicsObject.hpp"
#include "renderObject.hpp"
#include <glm/ext/matrix_float4x4.hpp>
#include <initializer_list>

class Sphere : public RenderObject {
    protected:
        float radius{0.0f};
        int rows{0}, cols{0};

    public:
        Sphere(float _radius, int _rows, int _cols):
        RenderObject(), radius{_radius}, rows{_rows}, cols{_cols} {
            InitSphere();
            Configure();
        }

    private:
        void InitSphere();

        void Update() override;
};

class SphereBody : public PhysicsObject {
    
    public:
        float radius;

        SphereBody(glm::vec3 _position, float _radius, float _mass):
        PhysicsObject(_position, _mass), radius(_radius) {
            renderObj = new Sphere(radius, (int)radius * 5, (int)radius * 5);
            renderObj->Translate(position);
        }

};

#endif