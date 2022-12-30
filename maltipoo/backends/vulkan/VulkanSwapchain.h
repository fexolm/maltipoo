#pragma once

#include "VulkanDevice.h"
#include "VulkanTexture.h"
#include <memory>
#include <vulkan/vulkan.h>

class VulkanSwapchain {
  public:
    VulkanSwapchain(VulkanDeviceRef device, VkSurfaceKHR surface, VkSurfaceFormatKHR surfaceFormat, VkPresentModeKHR presentMode, VkSurfaceCapabilitiesKHR caps, VkExtent2D extent);

    const VkSwapchainKHR &Swapchain() const;

    ~VulkanSwapchain();

  private:
    VkSwapchainKHR swapchain;

    VulkanDeviceRef device;
};

using VulkanSwapchainRef = std::shared_ptr<VulkanSwapchain>;

class VulkanSwapchainImageView : public VulkanTexture {
  public:
    VulkanSwapchainImageView(VulkanDeviceRef device, VulkanSwapchainRef swapchain, VkImage swapchainImage, VkSurfaceFormatKHR surfaceFormat, VkExtent2D extent);

    virtual const VkImageView &GetImageView() const override;

    virtual ~VulkanSwapchainImageView() override;

  private:
    VkImageView imageView;

    VulkanDeviceRef device;
    VulkanSwapchainRef swapchain;
};

using VulkanSwapchainImageViewRef = std::shared_ptr<VulkanSwapchainImageView>;
