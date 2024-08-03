#include <algorithm>
#include <memory>
#include <optional>
#include <stdexcept>
#include <vector>

#include "render/renderer.hpp"
#include "scene/idTag.hpp"

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
    for (const auto& obj : m_objects)
    {
        if (!obj->GetVisible())
        {
            continue;
        }

        obj->Render(delta_time, 90.0f, 0.1f, 500.0f);
    }
}

void Renderer::Update(const double_t& delta_time, const double_t& xpos, const double_t& ypos)
{
    m_curr_cam->Inputs(m_window, xpos, ypos);
    for (const auto& obj : m_objects)
    {
        obj->Update(delta_time);
    }
}

void Renderer::AddCurrCamera(const std::string& uid)
{
    m_cameras.push_back(std::make_shared<Camera>(uid, m_width, m_height, glm::vec3(0.0f, 0.0f, 20.0f)));
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

void Renderer::AddObject(std::unique_ptr<RenderObject> object, const ShaderType& type)
{
    object->AddShader(type);
    object->SetCamera(m_curr_cam);

    for (const auto& light : m_lights)
    {
        object->AddLight(light);
    }

    m_objects.push_back(std::move(object));
}

void Renderer::AddObject(std::unique_ptr<RenderObject> object)
{
    AddObject(std::move(object), ShaderType::Default);
}

void Renderer::SetCurrCam(const std::shared_ptr<Camera>& cam)
{
    m_curr_cam = cam;

    for (auto& obj : m_objects)
    {
        obj->SetCamera(m_curr_cam);
    }
}

void Renderer::AddLight(const std::shared_ptr<Light>& light)
{
    m_lights.push_back(light);

    for (const auto& obj : m_objects)
    {
        obj->AddLight(m_lights.back());
    }
}

void Renderer::RemoveLight(const std::string& uid)
{
    const auto& search = std::find_if(m_lights.begin(), m_lights.end(), [&](const std::shared_ptr<Light>& item){ return item->GetUid() == uid; });

    if (search == m_lights.end())
    {
        return;
    }

    for (const auto& obj : m_objects)
    {
        obj->RemoveLight(*search);
    }

    m_lights.erase(search);
}