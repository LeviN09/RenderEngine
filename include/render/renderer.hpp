#pragma once

#include <cstdint>
#include <memory>
#include <mutex>
#include <queue>
#include <vector>

#include "render/camera.hpp"
#include "render/gpu_interface/shaderClass.hpp"
#include "render/light.hpp"
#include "render/renderObject.hpp"
#include "scene/iTaggedObjectManager.hpp"

#include <GLFW/glfw3.h>


template<typename T, typename Container=std::deque<T> >
class iterable_queue : public std::queue<T,Container>
{
public:
    typedef typename Container::iterator iterator;
    typedef typename Container::const_iterator const_iterator;

    iterator begin() { return this->c.begin(); }
    iterator end() { return this->c.end(); }
    const_iterator begin() const { return this->c.begin(); }
    const_iterator end() const { return this->c.end(); }
};

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
        void RemoveLight(const std::string& uid);
        void AddUpdatedObject(std::unique_ptr<RenderObject> object, const ShaderType& type);
        void AddObject(std::unique_ptr<RenderObject> object, const ShaderType& type);
        void AddObject(std::unique_ptr<RenderObject> object);
        void RemoveObject(const std::string& uid);
        Camera& GetCurrCam() const;
        void SetCurrCam(const std::shared_ptr<Camera>& cam);
        Light& GetLight(const std::string& uid) const;

        void Render(const double_t& delta_time);
        void Update(const double_t& delta_time, const double_t& xpos, const double_t& ypos);
        void UpdateQueues();

    private:
        void ConfigureObject(const std::unique_ptr<RenderObject>& object);

        std::shared_ptr<Camera> m_curr_cam;
        GLFWwindow& m_window;
        int64_t m_width, m_height;
        float_t m_far_clip{ 5000.0f };

        std::vector<std::unique_ptr<RenderObject>> m_objects;
        std::vector<std::shared_ptr<Camera>> m_cameras;
        std::vector<std::shared_ptr<Light>> m_lights;

        iterable_queue<std::unique_ptr<RenderObject>> m_object_queue;
        iterable_queue<std::unique_ptr<RenderObject>> m_updates_queue;
        std::queue<std::string> m_to_remove_queue;

        std::mutex m_lock;
        std::mutex m_queue_lock;
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
    for (const auto& item : m_object_queue)
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
    for (const auto& item : m_updates_queue)
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