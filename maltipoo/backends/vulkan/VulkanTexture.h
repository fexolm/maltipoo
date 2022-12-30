#pragma once

#include "maltipoo/GPU.h"

#include <vulkan/vulkan.h>
#include <memory>

struct VulkanTexture : GPUTexture
{
    virtual const VkImageView &GetImageView() const = 0;
};

using VulkanTextureRef = std::shared_ptr<VulkanTexture>;