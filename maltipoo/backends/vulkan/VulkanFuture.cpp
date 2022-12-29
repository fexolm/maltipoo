#include "VulkanFuture.h"

VulkanFuture::VulkanFuture(VulkanDeviceRef device) : device(device)
{
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    semaphoreInfo.flags = 0;
    semaphoreInfo.pNext = nullptr;

    vkCreateSemaphore(device->Device(), &semaphoreInfo, nullptr, &semaphore);
}

const VkSemaphore &VulkanFuture::Semaphore() const
{
    return semaphore;
}

VulkanFuture::~VulkanFuture()
{
    vkDestroySemaphore(device->Device(), semaphore, nullptr);
}