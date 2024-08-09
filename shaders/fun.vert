#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 color;
out vec2 texCoord;
out vec3 normal;
out vec3 worldPos;

//uniform float scale;
uniform float deltaTime;

void main()
{
    vec3 newPos = aPos + sin(aPos * 20.0f + deltaTime) * 0.05f;

    gl_Position = projection * view * model * vec4(newPos, 1.0f);
    worldPos = vec3(model * vec4(newPos, 1.0f));
    normal = mat3(transpose(inverse(model))) * aNorm;
    color = aColor;
    texCoord = aTex;
}
