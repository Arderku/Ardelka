const float PI = 3.14159265359;

uniform vec3 albedo;       // Material albedo (in linear space)
uniform float metallic = 1;    // Metallic value [0,1]
uniform float roughness;   // Roughness value [0,1]

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir,
                                 vec3 materialAlbedo, float materialMetallic, float materialRoughness)
{
    // For a directional light, the light direction is the inverse of the light’s direction.
    vec3 L = normalize(-light.direction);
    vec3 H = normalize(viewDir + L);

    // Precompute the dot products
    float NdotL = max(dot(normal, L), 0.0);
    float NdotV = max(dot(normal, viewDir), 0.0);
    float NdotH = max(dot(normal, H), 0.0);

    float VdotH = max(dot(viewDir, H), 0.0);

    // Compute base reflectivity F0 using the metallic workflow:
    // For non-metals, F0 is typically around 0.04; for metals, it becomes the albedo.
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, materialAlbedo, materialMetallic);

    // --- Normal Distribution Function (GGX) ---
    float a = materialRoughness * materialRoughness;
    if(a < 0.01) a = 0.01;
    float a2 = a * a;
    float denom_NDF = (NdotH * NdotH * (a2 - 1.0) + 1.0);
    float D = a2 / (PI * denom_NDF * denom_NDF);

    // --- Geometry function (Smith using Schlick-GGX) ---
    float k = (materialRoughness + 1.0) * (materialRoughness + 1.0) / 8.0;
    float G_SmithV = NdotV / (NdotV * (1.0 - k) + k);
    float G_SmithL = NdotL / (NdotL * (1.0 - k) + k);
    float G = G_SmithV * G_SmithL;

    // --- Fresnel term (Schlick's approximation) ---
    vec3 F = F0 + (1.0 - F0) * pow(1.0 - VdotH, 5.0);

    // --- Specular term ---
    vec3 specular = (D * G * F) / (4.0 * NdotV * NdotL + 0.001);

    // --- Diffuse term (Lambertian) ---
    // The diffuse contribution is reduced by metallic; metals have little diffuse reflection.
    vec3 kS = F;
    vec3 kD = (vec3(1.0) - kS) * (1.0 - materialMetallic);
    vec3 diffuse = kD * materialAlbedo / PI;

    // Return the final directional light contribution.
    return (diffuse + specular) * light.color * NdotL;
}


vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir,
                           vec3 materialAlbedo, float materialMetallic, float materialRoughness)
{
    // Compute the light direction from the fragment position.
    vec3 L = normalize(light.position - fragPos);
    vec3 H = normalize(viewDir + L);

    float NdotL = max(dot(normal, L), 0.0);
    float NdotV = max(dot(normal, viewDir), 0.0);
    float NdotH = max(dot(normal, H), 0.0);
    float VdotH = max(dot(viewDir, H), 0.0);

    // Compute base reflectivity F0.
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, materialAlbedo, materialMetallic);

    // --- Normal Distribution Function (GGX) ---
    float a = materialRoughness * materialRoughness;
    float a2 = a * a;
    float denom_NDF = (NdotH * NdotH * (a2 - 1.0) + 1.0);
    float D = a2 / (PI * denom_NDF * denom_NDF);

    // --- Geometry function (Smith using Schlick-GGX) ---
    float k = (materialRoughness + 1.0) * (materialRoughness + 1.0) / 8.0;
    float G_SmithV = NdotV / (NdotV * (1.0 - k) + k);
    float G_SmithL = NdotL / (NdotL * (1.0 - k) + k);
    float G = G_SmithV * G_SmithL;

    // --- Fresnel term (Schlick's approximation) ---
    vec3 F = F0 + (1.0 - F0) * pow(1.0 - VdotH, 5.0);

    // --- Specular term ---
    vec3 specular = (D * G * F) / (4.0 * NdotV * NdotL + 0.001);

    // --- Diffuse term (Lambertian) ---
    vec3 kS = F;
    vec3 kD = (vec3(1.0) - kS) * (1.0 - materialMetallic);
    vec3 diffuse = kD * materialAlbedo / PI;

    // --- Attenuation ---
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // Return the final point light contribution.
    return (diffuse + specular) * light.color * NdotL * attenuation;
}
