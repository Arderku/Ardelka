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

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform DirectionalLight dirLight;
uniform PointLight pointLights[4];
