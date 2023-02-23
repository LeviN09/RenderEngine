#ifndef PLANE_CLASS_HPP
#define PLANE_CLASS_HPP

#include "renderObject.hpp"

class Plane : public RenderObject {
    public:
        Plane(): RenderObject() {
            InitPlane();
            Configure();
        }

        void InitPlane();

        void Update() override {}
};

#endif