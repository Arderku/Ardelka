#version 450 core

out vec4 FragColor;
uniform vec3 shadowColor;
uniform float shadowBias;

void main() {
    float depth = gl_FragCoord.z + shadowBias;
    FragColor = vec4(shadowColor, 1.0);  // For debugging, set alpha to 1.0
}