#pragma once

#include "maltipoo/GPU.h"

#include <vulkan/vulkan.h>

struct VulkanRenderTarget : public GPURenderTarget
{
	VkImageView swapchainImg;
	uint32_t imageIdx;
};