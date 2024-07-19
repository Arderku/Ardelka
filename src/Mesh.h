#pragma once

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Mesh {
public:
    // Constructor that initializes the mesh with vertices and indices
    Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices);

    // Destructor that cleans up the allocated resources
    ~Mesh();

    // Binds the vertex array object
    void Bind() const;

    // Unbinds the vertex array object
    void Unbind() const;

    // Returns the number of indices
    unsigned int GetIndexCount() const;

private:
    // Sets up the vertex array object, vertex buffer, and element buffer
    void SetupMesh();

    // Vertex Array Object, Vertex Buffer Object, Element Buffer Object
    GLuint m_VAO, m_VBO, m_EBO;

    // Vertex data and index data
    std::vector<float> m_Vertices;
    std::vector<unsigned int> m_Indices;
};

