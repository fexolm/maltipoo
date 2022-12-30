#pragma once

#include "maltipoo/GPU.h"
#include "Shared.h"
#include "VulkanCommandList.h"
#include "VulkanCommandPool.h"
#include "VulkanDescriptorPool.h"
#include "VulkanDescriptorSet.h"
#include "VulkanDevice.h"
#include "VulkanImage.h"
#include "VulkanRenderPass.h"
#include "VulkanSwapchain.h"
#include "VulkanSampler.h"
#include "VulkanShader.h"
#include "VulkanFuture.h"

#include <SDL2/SDL.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vulkan/vulkan.h>

const int MAX_FRAMES_IN_FLIGHT = 2;

class VulkanGPU : public GPUDriver
{

public:
	VulkanGPU(SDL_Window *window, int width, int height);

	virtual ~VulkanGPU() override;

	virtual GPUShaderRef CreateShader(const std::vector<uint32_t> &code, ShaderType type) override;

	virtual GPUBufferRef CreateBuffer(size_t size, const BufferInfo &info) override;

	virtual GPUGraphicsPipelineRef CreateGraphicsPipeline(const GraphicsPipelineCreateInfo &info) override;

	virtual GPUCommandListRef CreateCommandList() override;

	virtual GPUFutureRef<> Submit(GPUCommandListRef &commandList) override;

	virtual void WaitIdle() override;

	virtual void SubmitAndWaitIdle(GPUCommandListRef &commandList) override;

	virtual GPUFutureRef<GPUTexture> AquireFramebufferImage() override;

	virtual GPUTextureRef CreateTexture(uint32_t width, uint32_t height) override;

	virtual GPUSamplerRef CreateSampler() override;

	virtual void CopyBufferToTexture(const GPUBufferRef &buf, GPUTextureRef &texture, uint32_t width, uint32_t height) override;

	virtual void BeginFrame() override;

	virtual void Present(GPUFutureRef<> renderFinished) override;

	VulkanRenderPassRef CreateRenderPass();

	VkFramebuffer CreateFramebuffer(VulkanRenderPassRef renderPass, const VulkanTexture &renderTarget);

	VkExtent2D GetExtent();

	VulkanDescriptorSetRef CreateDescriptorSet(const GPUGraphicsPipelineRef &pipeline, int num);

private:
	VulkanInstanceRef instance;
	VulkanDeviceRef device;

	VkSurfaceKHR surface;

	VkSurfaceFormatKHR surfaceFormat;
	VkPresentModeKHR presentMode;
	VkSurfaceCapabilitiesKHR caps;
	VkExtent2D extent;

	VulkanSwapchainRef swapchain;
	std::vector<VulkanSwapchainImageViewRef> swapchainImageViews;

	VulkanCommandPoolRef commandPool;

	VulkanDescriptorPoolRef descriptorPool;

	VkFence inFlightFences[MAX_FRAMES_IN_FLIGHT];

	std::unordered_map<int, VkFramebuffer> frameBuffersCache;
	std::unordered_map<int, VulkanRenderPassRef> renderPassCache;

	uint32_t currentSwapchainImgIdx;
	uint32_t currentFrame;

	VulkanImageRef depthImage;

	std::unordered_multimap<int, GPUResourceRef> inFlightResources;
};