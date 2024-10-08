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
out vec3 modelViewPos;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} vs_out;

//uniform float scale;

uniform mat4 lightProjection;

void main()
{
    vec4 modelPos = model * vec4(aPos, 1.0f);

    vs_out.FragPos = vec3(modelPos);
    vs_out.Normal = transpose(inverse(mat3(model))) * aNorm;
    vs_out.TexCoords = aTex;
    vs_out.FragPosLightSpace = lightProjection * vec4(vs_out.FragPos, 1.0);

    gl_Position = projection * view * modelPos;
    worldPos = vec3(modelPos);
    modelViewPos = vec3(view * modelPos);
    //normal = aNorm;
    normal = transpose(inverse(mat3(model))) * aNorm;
    color = aColor;
    texCoord = aTex;
}
