#pragma once

#include <glm/glm.hpp>  // Assuming you are using GLM for vector and matrix operations

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
    glm::vec2 texCoord;
};
