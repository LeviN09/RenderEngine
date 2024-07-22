#ifndef PLANEPARTS_CLASS_HPP
#define PLANEPARTS_CLASS_HPP

#include "render/renderObject.hpp"

class PlaneRender : public RenderObject
{
    public:
        PlaneRender(const std::string& uid, const glm::mat4& model_mat, const float_t& scale):
            RenderObject(uid, model_mat), m_scale{ scale }
        {
            Init();
            Configure();
        }

        void Init() override;
        void Update(const double_t& delta_time) override {}

    private:
        float_t m_scale;
};

#endif