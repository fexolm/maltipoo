#pragma once

#include "VulkanCommandPool.h"
#include "VulkanDescriptorSet.h"
#include "VulkanDevice.h"
#include "VulkanGraphicsPipeline.h"

#include <vulkan/vulkan.h>

struct VulkanCommandList : public GPUCommandList
{
	explicit VulkanCommandList(VulkanDeviceRef device, VulkanCommandPoolRef commandPool, VkCommandBuffer commandBuffer, class VulkanGPU *gpu);

	virtual ~VulkanCommandList() override;

	virtual void Begin() override;

	virtual void End() override;

	virtual void BindPipeline(const GPUGraphicsPipelineRef &pipeline) override;

	virtual void BeginRenderPass(const GPUTextureRef &renderTarget, GPUFutureRef waitFuture) override;

	virtual void DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset,
							 uint32_t firstInstance) override;

	virtual void SetViewport(float offsetX, float offsetY, float width, float height) override;

	virtual void SetScissors(int32_t offsetX, int32_t offsetY, uint32_t width, uint32_t height) override;

	virtual void EndRenderPass() override;

	virtual void BindVertexBuffer(const GPUBufferRef &buf, int binding) override;

	virtual void BindIndexBuffer(const GPUBufferRef &buf) override;

	virtual void BindUniformBuffer(const std::string &name, const GPUBufferRef &buffer, int size) override;

	virtual void BindTexture(const std::string &name, const GPUTextureRef &texture, const GPUSamplerRef &sampler);

	void BindDescriptorSet(const GPUGraphicsPipelineRef &pipeline,
						   const VulkanDescriptorSetRef &descriptorSet, int binding);

	class VulkanGPU *gpu;

	VulkanDeviceRef device;
	VulkanCommandPoolRef commandPool;
	VkCommandBuffer commandBuffer;

	std::vector<GPUResourceRef> ownedResources;

	std::vector<VulkanDescriptorSetRef> currentDescriptorSets;

	GPUGraphicsPipelineRef currentPipeline;

	std::vector<VkSemaphore> waitSemaphores;
};

using VulkanCommandListRef = std::shared_ptr<VulkanCommandList>;