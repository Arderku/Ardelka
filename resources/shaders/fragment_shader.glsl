#version 450 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

// Uniforms for base color, texture maps, and material properties
uniform vec3 baseColor;
uniform sampler2D albedoMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D normalMap;

uniform float metallicValue;
uniform float roughnessValue;

// Directional light structure
struct DirectionalLight {
    vec3 direction;
    vec3 color;
};
uniform DirectionalLight dirLight;

// Point light structure
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

// Camera/view position
uniform vec3 viewPos;

const float PI = 3.14159265359;

// Function prototypes
vec3 CalcDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir, float roughness, vec3 albedo, float metallic);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, float roughness, vec3 albedo, float metallic);
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnelSchlick(float cosTheta, vec3 F0);

void main() {
    // Calculate normal vector from normal map or use default normal
    vec3 norm = normalize(texture(normalMap, TexCoords).rgb * 2.0 - 1.0);
    if (norm == vec3(0.0)) {
        norm = normalize(Normal);
    }

    vec3 viewDir = normalize(viewPos - FragPos);

    // Calculate albedo from texture map or use base color
    vec3 albedo = texture(albedoMap, TexCoords).rgb * baseColor;
    if (albedo == vec3(0.0)) {
        albedo = baseColor;
    }

    // Calculate metallic from texture map or use metallic value
    float metallic = texture(metallicMap, TexCoords).r * metallicValue;
    if (metallic == 0.0) {
        metallic = metallicValue;
    }

    // Calculate roughness from texture map or use roughness value
    float roughness = texture(roughnessMap, TexCoords).r * roughnessValue;
    if (roughness == 0.0) {
        roughness = roughnessValue;
    }

    float ao = 1.0; // Ambient occlusion is set to 1.0 for now

    // Calculate reflectance at normal incidence; use F0 of 0.04 for dielectrics and albedo for metals
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    // Calculate lighting
    vec3 Lo = CalcDirectionalLight(dirLight, norm, viewDir, roughness, albedo, metallic);

    for(int i = 0; i < numPointLights; i++) {
        Lo += CalcPointLight(pointLights[i], norm, FragPos, viewDir, roughness, albedo, metallic);
    }

    // Apply ambient occlusion
    vec3 ambient = vec3(0.03) * albedo * ao;

    // Combine results
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
    float denominator = 4.0 * max(dot(normal, viewDir), 0.0) * max(dot(normal, lightDir), 0.0) + 0.001;
    vec3 specular = nominator / denominator;

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;

    float NdotL = max(dot(normal, lightDir), 0.0);
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
    float denominator = 4.0 * max(dot(normal, viewDir), 0.0) * max(dot(normal, lightDir), 0.0) + 0.001;
    vec3 specular = nominator / denominator;

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;

    float NdotL = max(dot(normal, lightDir), 0.0);
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
