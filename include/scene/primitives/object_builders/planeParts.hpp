#ifndef PLANEPARTS_CLASS_HPP
#define PLANEPARTS_CLASS_HPP

#include "render/renderObject.hpp"

class PlaneRender : public RenderObject
{
    public:
        PlaneRender(const std::string& uid, const glm::mat4& model_mat):
            RenderObject(uid, model_mat)
        {
            Init();
            Configure();
        }

        void Init() override;
        void Update() override {}
};

#endif