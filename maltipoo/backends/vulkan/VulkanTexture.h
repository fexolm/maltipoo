#pragma once

#include "maltipoo/GPU.h"

#include <vulkan/vulkan.h>

struct VulkanTexture : GPUTexture
{
    virtual const VkImageView &GetImageView() const = 0;
};