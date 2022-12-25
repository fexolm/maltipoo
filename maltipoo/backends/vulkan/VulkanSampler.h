#pragma once

#include "VulkanDevice.h"
#include "maltipoo/GPU.h"

#include <vulkan/vulkan.h>

struct VulkanSampler : GPUSampler
{
	VulkanDeviceRef device;
	VkSampler sampler;

	VulkanSampler(VulkanDeviceRef device, VkSampler sampler);

	virtual ~VulkanSampler() override;
};