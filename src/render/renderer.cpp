#include <algorithm>
#include <memory>
#include <optional>
#include <stdexcept>
#include <vector>

#include "render/renderer.hpp"
#include "render/gpu_interface/shaderClass.hpp"
#include "render/light.hpp"
#include "render/renderObject.hpp"
#include "scene/idTag.hpp"

#include <GL/gl.h>

std::optional<std::reference_wrapper<IdTag>> Renderer::SearchObject(const std::string& uid) const
{
    const auto& search = std::find_if(m_objects.begin(), m_objects.end(), [&](const std::unique_ptr<RenderObject>& item){ return item->GetUid() == uid; });
    if (search != m_objects.end())
    {
        return *search->get();
    }

    const auto& search2 = std::find_if(m_cameras.begin(), m_cameras.end(), [&](const std::shared_ptr<Camera>& item){ return item->GetUid() == uid; });
    if (search2 != m_cameras.end())
    {
        return *search2->get();
    }

    const auto& search3 = std::find_if(m_lights.begin(), m_lights.end(), [&](const std::shared_ptr<Light>& item){ return item->GetUid() == uid; });
    if (search3 != m_lights.end())
    {
        return *search3->get();
    }

    return std::nullopt;
}

void Renderer::Render(const double_t& delta_time)
{
    DirectionalLight* light = dynamic_cast<DirectionalLight*>(m_lights[0].get());

    light->RenderShadowMap();

    m_lock.lock();
    for (const auto& obj : m_objects)
    {
        if (!obj->GetVisible())
        {
            continue;
        }

        obj->Render(delta_time, 90.0f, 0.1f, m_far_clip, light->GetShadowShader());
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, m_width, m_height);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (const auto& obj : m_objects)
    {
        if (!obj->GetVisible())
        {
            continue;
        }
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, light->GetShadowMap());
        obj->Render(delta_time, 90.0f, 0.1f, m_far_clip);
    }
    m_lock.unlock();
}

void Renderer::Update(const double_t& delta_time, const double_t& xpos, const double_t& ypos)
{
    m_curr_cam->Inputs(m_window, xpos, ypos);
    m_lock.lock();
    for (const auto& obj : m_objects)
    {
        obj->Update(delta_time);
    }
    m_lock.unlock();
}

void Renderer::UpdateQueues()
{
    m_queue_lock.lock();
    if (!m_to_remove_queue.empty())
    {
        m_lock.lock();
        const auto& search = std::find_if(m_objects.begin(), m_objects.end(), [&](const std::unique_ptr<RenderObject>& item){ return item->GetUid() == m_to_remove_queue.front(); });

        if (search != m_objects.end())
        {
            m_objects.erase(search);
        }
        m_to_remove_queue.pop();
        m_lock.unlock();
    }

    if (!m_object_queue.empty())
    {
        m_lock.lock();
        ConfigureObject(m_object_queue.front());
        m_objects.push_back(std::move(m_object_queue.front()));
        m_object_queue.pop();
        m_lock.unlock();
    }
    m_queue_lock.unlock(); 
}

void Renderer::AddCurrCamera(const std::string& uid)
{
    m_cameras.push_back(std::make_shared<Camera>(uid, m_width, m_height, glm::vec3(0.0f, 0.0f, 10.0f)));
    m_curr_cam = m_cameras.back();
}

Camera& Renderer::GetCurrCam() const
{
    return *m_curr_cam;
}

Light& Renderer::GetLight(const std::string& uid) const
{
    const auto& search = std::find_if(m_lights.begin(), m_lights.end(), [&](const std::shared_ptr<Light>& item){ return item->GetUid() == uid; });

    if (search == m_lights.end())
    {
        throw std::invalid_argument("No light [" + uid + "] found.");
    }

    return *search->get();
}

void Renderer::AddCamera(const std::string& uid)
{
    m_cameras.push_back(std::make_shared<Camera>(uid, m_width, m_height, glm::vec3(1.0f)));
}

void Renderer::DeleteCamera(const std::string& uid)
{
    std::erase_if(m_cameras, [&](std::shared_ptr<Camera>& item){ return item->GetUid() == uid; });
}

void Renderer::ConfigureObject(const std::unique_ptr<RenderObject>& object)
{
    object->Configure();
    object->AddShader();
    object->ApplyTexture();
    object->SetCamera(m_curr_cam);

    for (const auto& light : m_lights)
    {
        object->AddLight(light);
    }
}

void Renderer::AddObject(std::unique_ptr<RenderObject> object, const ShaderType& type)
{
    m_queue_lock.lock();
    object->SetShaderType(type);
    m_object_queue.push(std::move(object));
    m_queue_lock.unlock();
}

void Renderer::AddObject(std::unique_ptr<RenderObject> object)
{
    AddObject(std::move(object), ShaderType::Default);
}

void Renderer::RemoveObject(const std::string& uid)
{
    m_queue_lock.lock();
    m_to_remove_queue.push(uid);
    m_queue_lock.unlock();
}

void Renderer::SetCurrCam(const std::shared_ptr<Camera>& cam)
{
    m_curr_cam = cam;

    m_lock.lock();
    for (auto& obj : m_objects)
    {
        obj->SetCamera(m_curr_cam);
    }
    m_lock.unlock();
}

void Renderer::AddLight(const std::shared_ptr<Light>& light)
{
    m_lights.push_back(light);

    m_lock.lock();
    for (const auto& obj : m_objects)
    {
        obj->AddLight(m_lights.back());
    }
    m_lock.unlock();
}

void Renderer::RemoveLight(const std::string& uid)
{
    const auto& search = std::find_if(m_lights.begin(), m_lights.end(), [&](const std::shared_ptr<Light>& item){ return item->GetUid() == uid; });

    if (search == m_lights.end())
    {
        return;
    }

    m_lock.lock();
    for (const auto& obj : m_objects)
    {
        obj->RemoveLight(*search);
    }
    m_lock.unlock();

    m_lights.erase(search);
}