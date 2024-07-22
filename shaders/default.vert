#version 330 core
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

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    worldPos = vec3(model * vec4(aPos, 1.0f));
    //normal = aNorm;
    normal = mat3(transpose(inverse(model))) * aNorm;
    color = aColor;
    texCoord = aTex;
}
