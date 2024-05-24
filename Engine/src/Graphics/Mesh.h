#pragma once

#include <vector>
#include "Vertex.h"
#include "Buffer.h"

class Mesh {
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
    ~Mesh();

    void Bind() const;
    void Unbind() const;
    void Draw() const;

private:
    std::shared_ptr<Buffer> m_VertexBuffer;
    std::shared_ptr<Buffer> m_IndexBuffer;
    uint32_t m_IndexCount;
};
