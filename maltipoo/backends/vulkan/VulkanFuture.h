#pragma once

#include "VulkanDevice.h"
#include "VulkanTexture.h"

#include "maltipoo/GPU.h"
#include <future>
#include <vulkan/vulkan.h>

template <typename T>
struct VulkanTypeTraits;

template <>
struct VulkanTypeTraits<GPUTexture> {
    using VulkanType = VulkanTextureRef;
};

template <>
struct VulkanTypeTraits<void> {
    using VulkanType = void;
};

template <typename T>
class VulkanFuture : public GPUFuture<T> {
    using ValueType = typename VulkanTypeTraits<T>::VulkanType;

  public:
    explicit VulkanFuture(VulkanDeviceRef device)
        : device(device) {
        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        semaphoreInfo.flags = 0;
        semaphoreInfo.pNext = nullptr;

        vkCreateSemaphore(device->Device(), &semaphoreInfo, nullptr, &semaphore);
    }

    VulkanFuture(VulkanDeviceRef device, ValueType &&value)
        : VulkanFuture(device), value(value) {
    }

    const VkSemaphore &Semaphore() const {
        return semaphore;
    }

    virtual ~VulkanFuture() {
        vkDestroySemaphore(device->Device(), semaphore, nullptr);
    }

    const ValueType &operator*() const {
        return value;
    }

    ValueType &operator*() {
        return value;
    }

  private:
    ValueType value;

    VkSemaphore semaphore;

    VulkanDeviceRef device;
};

template <>
class VulkanFuture<void> : public GPUFuture<void> {
  public:
    explicit VulkanFuture(VulkanDeviceRef device)
        : device(device) {
        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        semaphoreInfo.flags = 0;
        semaphoreInfo.pNext = nullptr;

        vkCreateSemaphore(device->Device(), &semaphoreInfo, nullptr, &semaphore);
    }

    const VkSemaphore &Semaphore() const {
        return semaphore;
    }

    virtual ~VulkanFuture() {
        vkDestroySemaphore(device->Device(), semaphore, nullptr);
    }

  private:
    VkSemaphore semaphore;

    VulkanDeviceRef device;
};

template <typename T = void>
using VulkanFutureRef = std::shared_ptr<VulkanFuture<T>>;