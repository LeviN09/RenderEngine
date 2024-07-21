#ifndef RENDERER_CLASS_HPP
#define RENDERER_CLASS_HPP

#include <cstdint>
#include <memory>
#include <vector>

#include "render/camera.hpp"
#include "render/gpu_interface/shaderClass.hpp"
#include "render/light.hpp"
#include "render/renderObject.hpp"
#include "scene/iTaggedObjectManager.hpp"

#include <GLFW/glfw3.h>

class Renderer : public ITaggedObjectManager
{
    public:
        Renderer(GLFWwindow& window, const int64_t& width, const int64_t& height):
            m_window{window}, m_width{width}, m_height{height}
        {}

        std::optional<std::reference_wrapper<IdTag>> SearchObject(const std::string& uid) const override;

        template <typename T>
        T& GetObject(const std::string& uid);

        void AddCurrCamera(const std::string& uid);
        void AddCamera(const std::string& uid);
        void DeleteCamera(const std::string& uid);
        void AddLight(const std::shared_ptr<Light>& light);
        void AddObject(std::unique_ptr<RenderObject> object, const ShaderType& type);
        void AddObject(std::unique_ptr<RenderObject> object);
        Camera& GetCurrCam() const;
        void SetCurrCam(const std::shared_ptr<Camera>& cam);
        Light& GetLight() const;

        void Render(const double_t& delta_time);
        void Update(const double_t& delta_time, const double_t& xpos, const double_t& ypos);

    private:
        std::shared_ptr<Camera> m_curr_cam;
        GLFWwindow& m_window;
        int64_t m_width, m_height;

        std::vector<std::unique_ptr<RenderObject>> m_objects;
        std::vector<std::shared_ptr<Camera>> m_cameras;
        std::vector<std::shared_ptr<Light>> m_lights;
};

template<typename T>
T& Renderer::GetObject(const std::string& uid)
{
    for (const auto& item : m_objects)
    {
        if (item->GetUid() == uid)
        {
            T* castedItem = dynamic_cast<T*>(item.get());
            if (castedItem)
            {
                return *castedItem;
            }
            throw std::runtime_error("Item [" + uid + "] found but cast failed");
        }
    }
    throw std::runtime_error("Item [" + uid + "] not found");
}

#endif