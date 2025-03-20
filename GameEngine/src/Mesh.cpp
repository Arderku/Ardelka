#include "Mesh.h"
#include <iostream>
#include <vector>
#include <GL/glew.h>

Mesh::Mesh(const std::vector<float>& vertices, const std::vector<unsigned int>& indices)
        : m_Vertices(vertices), m_Indices(indices) {
    PrintMeshData(); // Print mesh data for debugging
    SetupMesh();
}

Mesh::~Mesh() {
    if (m_VAO) {
        glDeleteVertexArrays(1, &m_VAO);
    }
    if (m_VBO) {
        glDeleteBuffers(1, &m_VBO);
    }
    if (m_EBO) {
        glDeleteBuffers(1, &m_EBO);
    }
}

void Mesh::Bind() const {
    glBindVertexArray(m_VAO);
}

void Mesh::Unbind() const {
    glBindVertexArray(0);
}

void Mesh::SetupMesh() {
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(float), m_Vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int), m_Indices.data(), GL_STATIC_DRAW);

    // Define vertex attributes
    constexpr int stride = 8 * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0); // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float))); // Normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float))); // TexCoords
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void Mesh::PrintMeshData() const {
    std::cerr << "Mesh Data:" << std::endl;
   /* std::cerr << "Vertices:" << std::endl;
    for (size_t i = 0; i < m_Vertices.size(); i += 8) {
        std::cerr << "Position: (" << m_Vertices[i] << ", " << m_Vertices[i+1] << ", " << m_Vertices[i+2] << "), ";
        std::cerr << "Normal: (" << m_Vertices[i+3] << ", " << m_Vertices[i+4] << ", " << m_Vertices[i+5] << "), ";
        std::cerr << "TexCoords: (" << m_Vertices[i+6] << ", " << m_Vertices[i+7] << ")" << std::endl;
    }

    std::cerr << "Indices:" << std::endl;
    for (size_t i = 0; i < m_Indices.size(); i += 3) {
        std::cerr << m_Indices[i] << ", " << m_Indices[i+1] << ", " << m_Indices[i+2] << std::endl;
    }*/
}

unsigned int Mesh::GetIndexCount() const {
    return static_cast<unsigned int>(m_Indices.size());
}
