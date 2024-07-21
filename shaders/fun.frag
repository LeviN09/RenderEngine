#version 330 core
out vec4 FragColor;

in vec3 color;
in vec2 texCoord;
in vec3 normal;
in vec3 worldPos;
in vec2 screenCenter;

uniform vec2 windowSize;
uniform sampler2D tex0;
uniform int hasColorTexture;
uniform float deltaTime;
//uniform sampler2D tex1;

uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 lightPos;

void main()
{
    vec2 uv = ((gl_FragCoord.xy - screenCenter) * 2.0f - windowSize.xy) / windowSize.y;

    float d = length(uv);

    d = sin(d*8.0f + deltaTime * 10.0f) / 8.0f;
    d = abs(d);

    d = smoothstep(0.0f, 0.1f, d);

    FragColor = vec4(d, d, d, 1.0f);
}
