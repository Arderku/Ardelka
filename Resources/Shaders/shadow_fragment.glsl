#version 450 core

out vec4 FragColor;
uniform vec3 shadowColor;
uniform float shadowBias;

void main() {
     float depth = gl_FragCoord.z + shadowBias;
        float shadow = (depth > texture(shadowMap, shadowCoords).r) ? 0.5 : 1.0;
        FragColor = vec4(shadow * shadowColor, 0.1);
}