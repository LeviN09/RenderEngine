#version 330 core
out vec4 FragColor;

in vec3 color;
in vec2 texCoord;
in vec3 normal;
in vec3 worldPos;
in vec2 screenCenter;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

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
    // Transform the sphere center to view space
    vec3 centerViewSpace = (view * model * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
    
    // Transform the current fragment to view space
    vec3 fragViewSpace = (view * vec4(gl_FragCoord.xyz, 1.0)).xyz;
    
    // Calculate the vector from the center to the fragment in view space
    vec3 centerToFrag = fragViewSpace - centerViewSpace;
    
    // Project this vector onto the sphere's surface
    vec3 surfacePoint = normalize(centerToFrag);
    
    // Calculate the angle between the surface normal and the vector to the "visual center"
    float cosTheta = dot(normalize(normal), surfacePoint);

    float r = sin(cosTheta*8.0f + deltaTime * 3.0f) / 2.0f + 0.5f;
    float g = sin(cosTheta*10.0f + deltaTime * 5.0f) / 2.0f + 0.5f;
    float b = sin(cosTheta*3.0f + deltaTime * 8.0f) / 2.0f + 0.5f;

    FragColor = vec4(r, g, b, 1.0f);
}
