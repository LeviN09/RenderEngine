#version 330 core
out vec4 FragColor;

in vec3 color;
in vec2 texCoord;

//uniform vec2 windowSize;
//uniform float switcher;
uniform sampler2D tex0;
uniform int hasColorTexture;
//uniform sampler2D tex1;

/*float dist(vec2 coord) {
    return sqrt(pow(coord.x - gl_FragCoord.x, 2) + pow(coord.y - gl_FragCoord.y, 2));
}*/

void main() {
    //FragColor = vec4(color, 1.0f);
    if (hasColorTexture == 1) {
        FragColor = texture(tex0, texCoord) * vec4(color, 1.0f);
    }
    else {
        FragColor = vec4(color, 1.0f);
    }
}
