#pragma once

#include "maltipoo/GPU.h"
#include "VulkanDescriptorPool.h"

#include <vulkan/vulkan.h>

class VulkanDescriptorSet : public GPUResource
{
public:
	VulkanDescriptorSet(VulkanDeviceRef device,
						VulkanDescriptorPoolRef descriptorPool,
						const VkDescriptorSetLayout *layouts);

	VkDescriptorSet DescriptorSetHandle() const;

	virtual ~VulkanDescriptorSet() override;

	void Update(int binding, const GPUBufferRef &buf, int size);

	void Update(int binding, class VulkanImage &image,
				class VulkanSampler &sampler);

private:
	VulkanDeviceRef device;
	VkDescriptorSet descriptorSet;
	VulkanDescriptorPoolRef descriptorPool;

	GPUBufferRef bufferRef;
};

using VulkanDescriptorSetRef = std::shared_ptr<VulkanDescriptorSet>;