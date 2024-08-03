struct DirectionalLight {
    vec3 direction;
    vec3 color;
};

struct PointLight {
    vec3 position;
    vec3 color;
    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 color;
    float cutoff;
    float outerCutoff;
    float constant;
    float linear;
    float quadratic;
};

struct AmbientLight {
    vec3 color;
    float intensity;
};

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform DirectionalLight u_DirectionalLight;
uniform AmbientLight u_AmbientLight;
uniform SpotLight spotLight;
uniform PointLight pointLights[4];
