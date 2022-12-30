#pragma once

#include "VulkanDevice.h"
#include "VulkanGPU.h"
#include "VulkanShader.h"
#include "maltipoo/GPU.h"

#include <string>
#include <unordered_map>
#include <vector>
#include <vulkan/vulkan.h>

const int FRAMES_IN_FLIGHT = 3;

class VulkanGraphicsPipeline : public GPUGraphicsPipeline {
  public:
    VulkanGraphicsPipeline(VulkanDeviceRef device, VkExtent2D extent, VkFormat surfaceFormat, VkFormat depthFormat,
                           const GraphicsPipelineCreateInfo &info);

    virtual ~VulkanGraphicsPipeline();

    const std::vector<VkDescriptorSetLayout> &DescriptorLayouts() const;

    VkPipeline PipelineHandle() const;

    VkPipelineLayout LayoutHandle() const;

    std::unordered_map<std::string, DescriptorSetBindingPoint> params;

  private:
    VulkanDeviceRef device;

    VkPipelineLayout layoutHandle;
    VkPipeline pipelineHandle;

    std::vector<VkDescriptorSetLayout> descriptorSetLayouts;
    std::vector<GPUShaderRef> shaders;
};
