#pragma once

#include "VulkanDevice.h"

#include "maltipoo/GPU.h"
#include <vulkan/vulkan.h>

class VulkanFuture : public GPUFuture
{
public:
    VulkanFuture(VulkanDeviceRef device);

    const VkSemaphore &Semaphore() const;

    virtual ~VulkanFuture();

private:
    VkSemaphore semaphore;

    VulkanDeviceRef device;
};

using VulkanFutureRef = std::shared_ptr<VulkanFuture>;