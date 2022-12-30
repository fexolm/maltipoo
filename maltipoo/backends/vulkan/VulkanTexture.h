#pragma once

#include "maltipoo/GPU.h"

#include <memory>
#include <vulkan/vulkan.h>

struct VulkanTexture : GPUTexture {
    virtual const VkImageView &GetImageView() const = 0;
};

using VulkanTextureRef = std::shared_ptr<VulkanTexture>;