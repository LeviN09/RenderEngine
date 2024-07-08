#ifndef CUBEPARTS_CLASS_HPP
#define CUBEPARTS_CLASS_HPP

#include "render/renderObject.hpp"

class CubeRender : public RenderObject
{

    public:
        CubeRender(const std::string& uid, const glm::mat4& model_mat, float_t scale):
            RenderObject(uid, model_mat), m_scale{ scale }
        {
            Init();
            Configure();
        }

    private:
        void Init() override;
        void Update() override;

        float_t m_scale;
};

#endif