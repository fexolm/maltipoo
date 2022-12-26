find_package(Vulkan REQUIRED)
find_package(SDL2 CONFIG REQUIRED)

add_library(
    VulkanGPU 

    VulkanGPU.cpp 
    VulkanBuffer.cpp 
    VulkanDescriptorSet.cpp 
    VulkanGraphicsPipeline.cpp 
    VulkanDescriptorPool.cpp 
    VulkanDevice.cpp 
    VulkanRenderPass.cpp 
    VulkanInstance.cpp
    VulkanImage.cpp
    VulkanSampler.cpp
    VulkanShader.cpp
    VulkanCommandPool.cpp
    VulkanCommandList.cpp
    VulkanSwapchain.cpp
    GPU.cpp
    )

target_include_directories(VulkanGPU PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(VulkanGPU Vulkan::Vulkan SDL2::SDL2 spirv-cross-cpp)