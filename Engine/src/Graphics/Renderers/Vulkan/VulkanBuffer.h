#pragma once

#include <Graphics/Buffer.h>
#include <vulkan/vulkan.h>


class VulkanBuffer : public Buffer {
public:
    VulkanBuffer(VkDevice device, VkPhysicalDevice physicalDevice);
    ~VulkanBuffer();

    void Allocate(size_t size) override;
    void Update(const void* data, size_t size) override;
    void Bind() const override;

private:
    void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
    uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

    VkDevice device;
    VkPhysicalDevice physicalDevice;
    VkBuffer buffer;
    VkDeviceMemory bufferMemory;
    VkDeviceSize bufferSize;
};
