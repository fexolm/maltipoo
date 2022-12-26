#include "VulkanSwapchain.h"
#include "Shared.h"

VulkanSwapchain::VulkanSwapchain(VulkanDeviceRef device, VkSurfaceKHR surface, VkSurfaceFormatKHR surfaceFormat, VkPresentModeKHR presentMode, VkSurfaceCapabilitiesKHR caps, VkExtent2D extent)
    : device(device)
{
    uint32_t imageCount = caps.minImageCount + 1;

    VkSwapchainCreateInfoKHR swapchainCreateInfo{};
    swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainCreateInfo.surface = surface;
    swapchainCreateInfo.minImageCount = imageCount;
    swapchainCreateInfo.imageFormat = surfaceFormat.format;
    swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
    swapchainCreateInfo.imageExtent = extent;
    swapchainCreateInfo.imageArrayLayers = 1;
    swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    uint32_t queueFamilyIndices[] = {device->GraphicsFamily(), device->PresentFamily()};
    if (device->GraphicsFamily() == device->PresentFamily())
    {
        swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swapchainCreateInfo.queueFamilyIndexCount = 0;
        swapchainCreateInfo.pQueueFamilyIndices = nullptr;
    }
    else
    {
        swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapchainCreateInfo.queueFamilyIndexCount = 2;
        swapchainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
    }

    swapchainCreateInfo.preTransform = caps.currentTransform;
    swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchainCreateInfo.presentMode = presentMode;
    swapchainCreateInfo.clipped = true;
    swapchainCreateInfo.oldSwapchain = nullptr;
    swapchainCreateInfo.flags = 0;
    swapchainCreateInfo.pNext = nullptr;

    VULKAN_GPU_SAFE_CALL(vkCreateSwapchainKHR(device->Device(), &swapchainCreateInfo, nullptr, &swapchain));
}

const VkSwapchainKHR &VulkanSwapchain::Swapchain() const
{
    return swapchain;
}

VulkanSwapchain::~VulkanSwapchain()
{
    vkDestroySwapchainKHR(device->Device(), swapchain, nullptr);
}

VulkanSwapchainImageView::VulkanSwapchainImageView(VulkanDeviceRef device, VulkanSwapchainRef swapchain, VkImage swapchainImage, VkSurfaceFormatKHR surfaceFormat, VkExtent2D extent)
    : device(device), swapchain(swapchain)
{
    VkImageViewCreateInfo imageViewCreateInfo{};
    imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    imageViewCreateInfo.image = swapchainImage;
    imageViewCreateInfo.format = surfaceFormat.format;
    imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
    imageViewCreateInfo.subresourceRange.levelCount = 1;
    imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
    imageViewCreateInfo.subresourceRange.layerCount = 1;
    imageViewCreateInfo.flags = 0;
    imageViewCreateInfo.pNext = nullptr;

    VULKAN_GPU_SAFE_CALL(vkCreateImageView(device->Device(), &imageViewCreateInfo, nullptr, &imageView));
}

const VkImageView &VulkanSwapchainImageView::GetImageView() const
{
    return imageView;
}

VulkanSwapchainImageView::~VulkanSwapchainImageView()
{
    vkDestroyImageView(device->Device(), imageView, nullptr);
}
