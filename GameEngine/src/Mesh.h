#pragma once

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Mesh {
public:
    Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);
    ~Mesh();

    void Bind() const;
    void Unbind() const;
    unsigned int GetIndexCount() const;

private:
    void SetupMesh();
    void PrintMeshData() const; // Debug function to print mesh data

    GLuint m_VAO, m_VBO, m_EBO;
    std::vector<float> m_Vertices;
    std::vector<unsigned int> m_Indices;
};
