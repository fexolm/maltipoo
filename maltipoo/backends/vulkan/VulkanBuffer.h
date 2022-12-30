#pragma once

#include "VulkanDevice.h"
#include "maltipoo/GPU.h"

#include <memory>
#include <vulkan/vulkan.h>

class VulkanBuffer : public GPUBuffer {
  public:
    VulkanBuffer(VulkanDeviceRef device, VkPhysicalDevice physicalDevice,
                 uint32_t size, const BufferInfo &info);

    virtual void Write(void *data, uint32_t size) override;

    VkBuffer Buffer() const;

    VkDeviceMemory Memory() const;

    virtual ~VulkanBuffer() override;

  private:
    VulkanDeviceRef device;
    VkBuffer buffer;
    VkDeviceMemory alloc;
};