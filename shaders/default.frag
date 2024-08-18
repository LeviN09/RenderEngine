#version 460 core
out vec4 FragColor;

in vec3 color;
in vec2 texCoord;
in vec3 normal;
in vec3 worldPos;
in vec3 modelViewPos;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

//uniform vec2 windowSize;
uniform sampler2D shadowTex;
uniform sampler2D tex0;
uniform int hasColorTexture;
//uniform sampler2D tex1;

uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform float lightIntensity;

uniform vec3 dirLightColor;
uniform float dirLightIntensity;
uniform vec3 dirLightDir;

vec3 getDiffuse(vec3 norm, vec3 lightDir)
{
    float diff = max(dot(norm, lightDir), 0.0f);
    float dirLightDiff = max(dot(norm, dirLightDir), 0.0f);
    return clamp(lightIntensity * diff * lightColor + dirLightIntensity * dirLightDiff * dirLightColor, 0.0f, 1.0f);
}

vec3 getSpecular(vec3 norm, vec3 lightDir)
{
    float specularStrength = 0.5f;

    vec3 viewDir = normalize(viewPos - worldPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    vec3 dirReflectDir = reflect(-dirLightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    float dirSpec = pow(max(dot(viewDir, dirReflectDir), 0.0f), 32);
    return specularStrength * spec * lightColor + specularStrength * dirSpec * dirLightColor;  
}

float ShadowCalculation(vec3 norm, vec4 fragPosLightSpace)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5f + 0.5f;

    float closestDepth = texture(shadowTex, projCoords.xy).r; 
    float currentDepth = projCoords.z;

    float bias = max(-0.005f * (1.0f - dot(norm, dirLightDir)), 0.0005f); 

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowTex, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowTex, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0f : 0.0f;        
        }    
    }
    shadow /= 9.0f;

    if(projCoords.z > 1.0f)
    {
        shadow = 0.0f;
    }

    return shadow;
}

float fogFar = 1300.0f;
float fogNear = 900.0f;

void main()
{
    vec3 ambient = vec3(0.2f);

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - worldPos);

    float dist = length(modelViewPos);
    float fogFactor = (fogFar - dist) / (fogFar - fogNear);
    fogFactor = clamp(fogFactor, 0.0f, 1.0f);

    float shadow = ShadowCalculation(norm, fs_in.FragPosLightSpace); 
    vec4 result = vec4((ambient + (1.0f - shadow) * (getDiffuse(norm, lightDir) + getSpecular(norm, lightDir))) * color, 1.0f);

    result = mix(vec4(0.0f, 0.0f, 1.0f, 0.0f), result, fogFactor);

    if (hasColorTexture == 1)
    {
        FragColor = texture(tex0, texCoord) * result;
    }
    else
    {
        FragColor = result;
    }
}
