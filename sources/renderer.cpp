#include "../headers/renderer.hpp"
#include <iostream>
#include <memory>

void Renderer::Render() {
    for (auto&& obj : objects) {
        if (obj != nullptr) {
            obj->SetLight(lights[0]);
            obj->Render(90.0f, 0.1f, 150.0f);
        }
    }
}

void Renderer::Update() {
    currCam->Inputs(window);
    for (auto&& obj : objects) {
        if (obj != nullptr)
            obj->Update();
    }
}

Camera* Renderer::AddCurrCamera() {
    Camera* cam = new Camera(width, height, glm::vec3(0.0f, 0.0f, 20.0f));
    currCam = cam;
    cameras.push_back(cam);
    return currCam;
}

Camera* Renderer::AddCamera() {
    Camera* cam = new Camera(width, height, glm::vec3(1.0f));
    cameras.push_back(cam);
    return cam;
}

void Renderer::AddObject(RenderObject* object) {
    objects.push_back(object);
    object->SetCamera(currCam);
    object->AddShader();
}

void Renderer::AddObject(PhysicsObject* object) {
    RenderObject* renderObj = object->GetRenderObject();
    AddObject(renderObj);
}

void Renderer::SetCurrCam(Camera* cam) {
    currCam = cam;

    for (auto& obj : objects) {
        obj->SetCamera(currCam);
    }
}

Light* Renderer::AddLight(Light* light) {
    lights.push_back(light);
    return light;
}