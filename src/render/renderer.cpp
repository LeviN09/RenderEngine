#include <algorithm>
#include <memory>
#include <optional>
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

void Renderer::Render()
{
    for (const auto& obj : m_objects)
    {
        obj->SetLight(m_lights[0]);
        obj->Render(90.0f, 0.1f, 150.0f);
    }
}

void Renderer::Update(const double_t& xpos, const double_t& ypos)
{
    m_curr_cam->Inputs(m_window, xpos, ypos);
    for (const auto& obj : m_objects)
    {
        obj->Update();
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

Light& Renderer::GetLight() const
{
    return *m_lights[0];
}

void Renderer::AddCamera(const std::string& uid)
{
    m_cameras.push_back(std::make_shared<Camera>(uid, m_width, m_height, glm::vec3(1.0f)));
}

void Renderer::DeleteCamera(const std::string& uid)
{
    std::erase_if(m_cameras, [&](std::shared_ptr<Camera>& item){ return item->GetUid() == uid; });
}

void Renderer::AddObject(std::unique_ptr<RenderObject> object, const std::string& vert_shader, const std::string& frag_shader)
{
    object->AddShader(vert_shader, frag_shader);
    object->SetCamera(m_curr_cam);
    m_objects.push_back(std::move(object));
}

void Renderer::AddObject(std::unique_ptr<RenderObject> object)
{
    AddObject(std::move(object), "../shaders/default.vert", "../shaders/default.frag");
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
}