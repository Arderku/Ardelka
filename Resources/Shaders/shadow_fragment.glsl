#version 450 core

out vec4 FragColor;

uniform vec3 shadowColor;  // The color of the shadow (for debug or visual effect)
uniform float shadowBias;  // Bias to prevent shadow acne
uniform sampler2D shadowMap;  // The shadow map texture
in vec4 shadowCoords;   // Shadow coordinates passed from the vertex shader

void main() {
    // Normalize the depth for proper comparison in light space (divide by w for perspective divide)
    float depth = shadowCoords.z / shadowCoords.w;
    float shadow = 1.0;  // Default to not in shadow (fully lit)

    // Sample the shadow map (get the depth value at the current shadow coordinates)
    float shadowMapDepth = texture(shadowMap, shadowCoords.xy).r;

    // Compare depths (with bias) to decide if it's in shadow or not
    if (depth - shadowBias > shadowMapDepth) {
        shadow = 0.0;  // In shadow
    }

    // Set the output color to be the shadow color, modulated by the shadow factor
    FragColor = vec4(shadow * shadowColor, 0.1);
}
