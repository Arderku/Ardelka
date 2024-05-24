#include "Mesh.h"
#include "Renderer.h"

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
        : m_IndexCount(static_cast<uint32_t>(indices.size())) {
    m_VertexBuffer = Buffer::Create(vertices.size() * sizeof(Vertex), vertices.data());
    m_IndexBuffer = Buffer::Create(indices.size() * sizeof(uint32_t), indices.data());
}

Mesh::~Mesh() {}

void Mesh::Bind() const {
    m_VertexBuffer->Bind();
    m_IndexBuffer->Bind();
}

void Mesh::Unbind() const {
    m_VertexBuffer->Unbind();
    m_IndexBuffer->Unbind();
}

void Mesh::Draw() const {
    // Command to draw the mesh
    // This will depend on the API (e.g., vkCmdDrawIndexed for Vulkan)
}
