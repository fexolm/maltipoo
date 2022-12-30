#pragma once

#include "VulkanDevice.h"
#include "maltipoo/GPU.h"
#include <map>
#include <unordered_map>

struct DescriptorSetBindingPoint {
    int set;
    int binding;
};

struct VulkanShader : public GPUShader {
    explicit VulkanShader(VulkanDeviceRef device, VkShaderModule module);

    virtual ~VulkanShader() override;

    VulkanDeviceRef device;
    VkShaderModule module;

    std::map<int, std::vector<VkDescriptorSetLayoutBinding>> bindings;
    std::vector<VkVertexInputAttributeDescription> vertexAttributes;
    std::vector<VkVertexInputBindingDescription> vertexBindings;

    std::unordered_map<std::string, DescriptorSetBindingPoint> params;
};