#version 450 core

#include "common.glsl"
#include "lighting.glsl"

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec3 Tangent;
in vec3 Bitangent;

out vec4 FragColor;

uniform sampler2D albedoMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D normalMap;
uniform sampler2D shadowMap;

uniform vec3 viewPos;
uniform vec3 shadowColor;

uniform mat4 lightSpaceMatrix;

vec3 GetNormalFromMap() {
    vec3 tangentNormal = texture(normalMap, TexCoords).xyz * 2.0 - 1.0;

    vec3 Q1 = dFdx(FragPos);
    vec3 Q2 = dFdy(FragPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);

    vec3 N = normalize(Normal);
    vec3 T = normalize(Q1 * st2.t - Q2 * st1.t);
    vec3 B = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

float ShadowCalculation(vec4 fragPosLightSpace) {
    // Perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // Get closest depth value from light's perspective
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    // Calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(vec3(lightSpaceMatrix * vec4(-u_DirectionalLight.direction, 0.0)));
    float bias = max(0.005 * (1.0 - dot(normal, lightDir)), 0.005);

    // Percentage-Closer Filtering (PCF)
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    // If the fragment is outside the light's frustum, do not apply shadow.
    if (projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

void main() {
    // Sample material properties from textures.
    vec3 albedoTex = texture(albedoMap, TexCoords).rgb;
    float metallicTex = texture(metallicMap, TexCoords).r;
    float roughnessTex = texture(roughnessMap, TexCoords).r;

    // Compute the normal using normal mapping.
    vec3 normal = GetNormalFromMap();
    vec3 norm = normalize(normal);

    // Compute view direction.
    vec3 viewDir = normalize(viewPos - FragPos);

    // Calculate shadow factor.
    vec4 fragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
    float shadow = ShadowCalculation(fragPosLightSpace);

    // Start with an ambient term.
    vec3 ambient = u_AmbientLight.color * u_AmbientLight.intensity;
    vec3 lighting = ambient;

    // Calculate directional light contribution.
    // (Pass material parameters to the function.)
    vec3 directionalLight = (1.0 - shadow) * CalculateDirectionalLight(u_DirectionalLight, norm, viewDir, albedoTex, metallicTex, roughnessTex);
    lighting += directionalLight;

    // Add contributions from point lights.
    for (int i = 0; i < 4; ++i) {
        lighting += (1.0 - shadow) * CalculatePointLight(pointLights[i], norm, FragPos, viewDir, albedoTex, metallicTex, roughnessTex);
    }

    // Optionally mix the computed lighting with a shadow color.
    vec3 shadowedColor = mix(lighting, shadowColor, shadow);
    // Multiply by the albedo.
    vec3 finalColor = albedoTex * shadowedColor;

    FragColor = vec4(finalColor, 1.0);
}
