#version 460 core
out vec4 FragColor;
  
in vec3 color;
in vec2 texCoord;
in vec3 normal;
in vec3 worldPos;

uniform sampler2D shadowMap;
uniform sampler2D tex0;

void main()
{             
    float depthValue = texture(shadowMap, texCoord).r;
    float texVal = texture(tex0, texCoord).r;
    FragColor = vec4(vec3(depthValue) * texVal, 1.0);
}