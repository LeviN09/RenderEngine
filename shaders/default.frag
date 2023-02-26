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

uniform vec3 lightColor;
uniform vec3 lightPos;

void main() {
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - worldPos);
    float diff = max(dot(norm, lightDir), 0.0f);
    //float diff = 1.0f;
    vec3 diffuse = diff * lightColor;
    //vec3 diffuse = lightColor;
    vec3 ambient = vec3(0.1f, 0.1f, 0.1f);

    //FragColor = vec4(color, 1.0f);
    if (hasColorTexture == 1) {
        vec3 result = (ambient + diffuse) * color;
        //FragColor = vec4(result, 1.0);
        //FragColor = vec4(diffuse, 1.0f);
        FragColor = texture(tex0, texCoord) * vec4(result, 1.0f);
    }
    else {
        vec3 result = (ambient + diffuse) * color;
        FragColor = vec4(result, 1.0);
        //FragColor = vec4(color, 1.0f);
    }
}
