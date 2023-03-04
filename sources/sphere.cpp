#include "../headers/sphere.hpp"
#include <initializer_list>
#include <vector>

void Sphere::InitSphere() {
    const float deltaRowAngle = M_PI / rows;
    const float deltaColumnAngle = 2 * M_PI / cols;

    PushToVerts({0.0f, radius, 0.0f,  0.0f, radius, 0.0f,  1.0f, 1.0f, 1.0f});
    for (int i = 0; i < rows - 1; i++) {
        float miniRadius = (float)sin((i + 1) * deltaRowAngle) * radius;
        for (int j = 0; j < cols; j++) {
            PushToVerts({
                (float)cos(j * deltaColumnAngle) * miniRadius,
                (float)cos((i + 1) * deltaRowAngle) * radius ,
                (float)sin(j * deltaColumnAngle) * miniRadius,
                (float)cos(j * deltaColumnAngle) * miniRadius,
                (float)cos((i + 1) * deltaRowAngle) * radius ,
                (float)sin(j * deltaColumnAngle) * miniRadius,
                i * 1.0f / rows, j * 1.0f / cols, 1.0f
            });
        }
    }
    PushToVerts({0.0f, -radius, 0.0f, 0.0f, -radius, 0.0f,  1.0f, 1.0f, 1.0f});

    for (int i = 0; i < cols - 1; i++) {
        PushToInds({0, i + 2, i + 1});
    }
    PushToInds({0, 1, cols});
    for (int i = 0; i < rows - 2; i++) {
        for (int j = 0; j < cols - 1; j++) {
            PushToInds({i * cols + j + 1, (i + 1) * cols + j + 1, (i + 1) * cols + j + 2});
            PushToInds({i * cols + j + 1, (i + 1) * cols + j + 2, i * cols + j + 2});
        }
        PushToInds({i * cols + 1, (i + 1) * cols + 1, (i + 1) * cols});
        PushToInds({(i + 1) * cols + 1, (i + 2) * cols, (i + 1) * cols});
    }
    for (int i = 0; i < cols - 1; i++) {
        PushToInds({(rows - 2) * cols + i + 1, (rows - 1) * cols + 1, (rows - 2) * cols + i + 2});
    }
    PushToInds({(rows - 1) * cols, (rows - 1) * cols + 1, (rows - 2) * cols + 1});

    hasNormals = true;
    hasColorTexture = false;
}

void Sphere::Update() {
    //Rotate(glm::radians(1.0f), glm::vec3(1.0f));
}