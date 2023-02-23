#ifndef SPHERE_CLASS_HPP
#define SPHERE_CLASS_HPP

#include "renderObject.hpp"
#include <glm/ext/matrix_float4x4.hpp>
#include <initializer_list>

class Sphere : public RenderObject {
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

#endif