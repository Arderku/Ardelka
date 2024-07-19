#include "Mesh.h"
#include <iostream>
#include <vector>

// Constructor that initializes the mesh with vertices and indices
Mesh::Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices)
        : m_Vertices(vertices), m_Indices(indices) {
    SetupMesh();
}

// Destructor that cleans up the allocated resources
Mesh::~Mesh() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
}

// Binds the vertex array object
void Mesh::Bind() const {
    glBindVertexArray(m_VAO);
}

// Unbinds the vertex array object
void Mesh::Unbind() const {
    glBindVertexArray(0);
}

// Sets up the vertex array object, vertex buffer, and element buffer
void Mesh::SetupMesh() {
    // Generate and bind the vertex array object
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);

    // Bind and set the vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(float), m_Vertices.data(), GL_STATIC_DRAW);

    // Bind and set the element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), m_Indices.data(), GL_STATIC_DRAW);

    // Set vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // Normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // TexCoords
    glEnableVertexAttribArray(2);

    // Unbind the vertex array object
    glBindVertexArray(0);
}

// Returns the number of indices
unsigned int Mesh::GetIndexCount() const {
    return static_cast<unsigned int>(m_Indices.size());
}
