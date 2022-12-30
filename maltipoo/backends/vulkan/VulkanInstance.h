#pragma once

#include <memory>
#include <vector>
#include <vulkan/vulkan.hpp>

class VulkanInstance {
  public:
    VulkanInstance(const std::vector<const char *> &enabledExtensions);

    virtual ~VulkanInstance();

    const VkInstance &Instance();

  private:
    VkInstance instance;
};

using VulkanInstanceRef = std::shared_ptr<VulkanInstance>;