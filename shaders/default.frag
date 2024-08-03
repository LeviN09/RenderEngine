#version 330 core
out vec4 FragColor;

in vec3 color;
in vec2 texCoord;
in vec3 normal;
in vec3 worldPos;

//uniform vec2 windowSize;
uniform sampler2D tex0;
uniform int hasColorTexture;
//uniform sampler2D tex1;

uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 lightPos;

uniform vec3 dirLightColor;
uniform vec3 dirLightIntensity;
uniform vec3 dirLightDir;

vec3 getDiffuse(vec3 norm, vec3 lightDir)
{
    float diff = max(dot(norm, lightDir), 0.0f);
    float dirLightDiff = max(dot(norm, dirLightDir), 0.0f);
    return diff * lightColor + dirLightDiff * dirLightColor;
}

vec3 getSpecular(vec3 norm, vec3 lightDir)
{
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - worldPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    return specularStrength * spec * lightColor;  
}

void main()
{
    vec3 ambient = vec3(0.1f, 0.1f, 0.1f);

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - worldPos);

    vec3 result = (ambient + getDiffuse(norm, lightDir) + getSpecular(norm, lightDir)) * color;
    if (hasColorTexture == 1)
    {
        FragColor = texture(tex0, texCoord) * vec4(result, 1.0f);
    }
    else
    {
        FragColor = vec4(result, 1.0);
    }
}
