#version 450 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 baseColor; // Base color to mix with albedo texture
uniform sampler2D albedoMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D normalMap;

uniform float metallicValue; // Uniform float value for metallic
uniform float roughnessValue; // Uniform float value for roughness

// Directional light
struct DirectionalLight {
    vec3 direction;
    vec3 color;
};
uniform DirectionalLight dirLight;

// Point lights
struct PointLight {
    vec3 position;
    vec3 color;
    float constant;
    float linear;
    float quadratic;
};
#define MAX_POINT_LIGHTS 4
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform int numPointLights;

uniform vec3 viewPos;

const float PI = 3.14159265359;

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir, float roughness, vec3 albedo, float metallic);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float roughness, vec3 albedo, float metallic);

float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);

void main() {
    // Check if normalMap texture is provided, otherwise use default normal
    vec3 norm = texture(normalMap, TexCoords).rgb * 2.0 - 1.0;
    norm = normalize(norm);
    if (norm == vec3(0.0)) {
        norm = normalize(Normal);
    }

    vec3 viewDir = normalize(viewPos - FragPos);

    // Check if albedoMap texture is provided, otherwise use baseColor
    vec3 albedo = mix(baseColor, texture(albedoMap, TexCoords).rgb, texture(albedoMap, TexCoords).a);
    if (albedo == vec3(0.0)) {
        albedo = baseColor;
    }

    // Check if metallicMap texture is provided, otherwise use metallicValue
    float metallic = metallicValue * texture(metallicMap, TexCoords).r;
    if (metallic == 0.0) {
        metallic = metallicValue;
    }

    // Check if roughnessMap texture is provided, otherwise use roughnessValue
    float roughness = roughnessValue * texture(roughnessMap, TexCoords).r;
    if (roughness == 0.0) {
        roughness = roughnessValue;
    }

    float ao = 1.0;

    // Reflectance at normal incidence; if di-electric (like plastic) use F0 of 0.04 and if metal, use the albedo color
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    // Directional light contribution
    vec3 Lo = CalcDirectionalLight(dirLight, norm, viewDir, roughness, albedo, metallic);

    // Point light contributions
    for(int i = 0; i < numPointLights; i++) {
        Lo += CalcPointLight(pointLights[i], norm, FragPos, viewDir, roughness, albedo, metallic);
    }

    // Apply ambient occlusion
    vec3 ambient = vec3(0.03) * albedo * ao;

    vec3 color = ambient + Lo;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // Gamma correction
    color = pow(color, vec3(1.0 / 2.2));

    FragColor = vec4(color, 1.0);
}

vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir, float roughness, vec3 albedo, float metallic) {
    vec3 lightDir = normalize(-light.direction);
    vec3 halfDir = normalize(lightDir + viewDir);
    vec3 radiance = light.color;

    // Cook-Torrance BRDF
    float NDF = DistributionGGX(normal, halfDir, roughness);
    float G   = GeometrySmith(normal, viewDir, lightDir, roughness);
    vec3 F    = fresnelSchlick(max(dot(halfDir, viewDir), 0.0), mix(vec3(0.04), albedo, metallic));

    vec3 nominator    = NDF * G * F;
    float denominator = 4.0 * max(dot(normal, viewDir), 0.0) * max(dot(normal, lightDir), 0.0) + 0.001; // avoid divide by zero
    vec3 specular = nominator / denominator;

    // kS is equal to Fresnel
    vec3 kS = F;
    // for energy conservation, the diffuse and specular light can't be above 1.0 (unless the surface emits light)
    vec3 kD = vec3(1.0) - kS;
    // multiply kD by the inverse metalness such that only non-metals (dielectrics) have diffuse lighting, or a linear blend if partly metal (pure metals have no diffuse light).
    kD *= 1.0 - metallic;

    // scale light by NdotL
    float NdotL = max(dot(normal, lightDir), 0.0);

    // add to outgoing radiance Lo
    vec3 Lo = (kD * albedo / PI + specular) * radiance * NdotL;

    return Lo;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float roughness, vec3 albedo, float metallic) {
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 halfDir = normalize(lightDir + viewDir);
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    vec3 radiance = light.color * attenuation;

    // Cook-Torrance BRDF
    float NDF = DistributionGGX(normal, halfDir, roughness);
    float G   = GeometrySmith(normal, viewDir, lightDir, roughness);
    vec3 F    = fresnelSchlick(max(dot(halfDir, viewDir), 0.0), mix(vec3(0.04), albedo, metallic));

    vec3 nominator    = NDF * G * F;
    float denominator = 4.0 * max(dot(normal, viewDir), 0.0) * max(dot(normal, lightDir), 0.0) + 0.001; // avoid divide by zero
    vec3 specular = nominator / denominator;

    // kS is equal to Fresnel
    vec3 kS = F;
    // for energy conservation, the diffuse and specular light can't be above 1.0 (unless the surface emits light)
    vec3 kD = vec3(1.0) - kS;
    // multiply kD by the inverse metalness such that only non-metals (dielectrics) have diffuse lighting, or a linear blend if partly metal (pure metals have no diffuse light).
    kD *= 1.0 - metallic;

    // scale light by NdotL
    float NdotL = max(dot(normal, lightDir), 0.0);

    // add to outgoing radiance Lo
    vec3 Lo = (kD * albedo / PI + specular) * radiance * NdotL;

    return Lo;
}

float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}
