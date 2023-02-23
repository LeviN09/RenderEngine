#ifndef CUBE_CLASS_HPP
#define CUBE_CLASS_HPP

#include "renderObject.hpp"

class Cube : public RenderObject {
    float scale;

    public:
        Cube(float _scale): RenderObject(), scale{_scale} {
            InitCube();
            Configure();
        }

    private:
        void InitCube();

        void Update() override;
};

#endif