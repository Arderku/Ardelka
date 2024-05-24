#pragma once
#include <Graphics/Mesh.h>
#include <vulkan/vulkan.h>
#include <Graphics/Vertex.h>

class VulkanMesh : public Mesh {
public:
    VulkanMesh(VkDevice device, VkPhysicalDevice physicalDevice);
    ~VulkanMesh();

    void Bind() const override;
    void Unbind() const override;
    void SetVertices(const std::vector<Vertex>& vertices) override;
    void SetIndices(const std::vector<uint32_t>& indices) override;
    uint32_t GetIndexCount() const override { return static_cast<uint32_t>(m_Indices.size()); }

private:
    VkDevice m_Device;
    VkPhysicalDevice m_PhysicalDevice;
    VkBuffer m_VertexBuffer;
    VkDeviceMemory m_VertexBufferMemory;
    VkBuffer m_IndexBuffer;
    VkDeviceMemory m_IndexBufferMemory;
    std::vector<uint32_t> m_Indices;

    void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
    void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
};
