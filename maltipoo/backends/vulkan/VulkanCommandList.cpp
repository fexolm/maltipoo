#include "VulkanCommandList.h"
#include "Shared.h"
#include "VulkanBuffer.h"

VulkanCommandList::VulkanCommandList(VulkanDeviceRef device, VulkanCommandPoolRef commandPool, VkCommandBuffer commandBuffer, class VulkanGPU *gpu)
    : device(device), commandPool(commandPool), commandBuffer(commandBuffer), gpu(gpu) {
}

VulkanCommandList::~VulkanCommandList() {
    vkFreeCommandBuffers(device->Device(), commandPool->CommandPool(), 1, &commandBuffer);
}

void VulkanCommandList::Begin() {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.pInheritanceInfo = nullptr;
    beginInfo.flags = 0;
    beginInfo.pNext = nullptr;
    VULKAN_GPU_SAFE_CALL(vkBeginCommandBuffer(commandBuffer, &beginInfo));
}

void VulkanCommandList::End() {
    VULKAN_GPU_SAFE_CALL(vkEndCommandBuffer(commandBuffer));
}

void VulkanCommandList::BeginRenderPass(GPUFutureRef<GPUTexture> renderTarget) {
    VulkanFutureRef<GPUTexture> future = std::static_pointer_cast<VulkanFuture<GPUTexture>>(renderTarget);
    VulkanTexture *vulkanRenderTarget = (**future).get();

    VulkanRenderPassRef renderPass = gpu->CreateRenderPass();
    VkFramebuffer framebuffer = gpu->CreateFramebuffer(renderPass, *vulkanRenderTarget);

    VkRenderPassBeginInfo renderPassBeginInfo;
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.renderPass = renderPass->RenderPass();
    renderPassBeginInfo.framebuffer = framebuffer;
    renderPassBeginInfo.renderArea.offset = VkOffset2D{0, 0};
    renderPassBeginInfo.renderArea.extent = gpu->GetExtent();

    std::array<VkClearValue, 2> clearValues;
    clearValues[0].color = {{0.f, 0.f, 0.f, 0.f}};
    clearValues[1].depthStencil = {1.0f, 0};
    renderPassBeginInfo.clearValueCount = clearValues.size();
    renderPassBeginInfo.pClearValues = clearValues.data();
    renderPassBeginInfo.pNext = nullptr;

    ownedResources.emplace_back(future);
    waitSemaphores.emplace_back(future->Semaphore());

    vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void VulkanCommandList::DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset,
                                    uint32_t firstInstance) {
    for (int i = 0; i < currentDescriptorSets.size(); i++) {
        BindDescriptorSet(currentPipeline, currentDescriptorSets[i], i);
    }
    vkCmdDrawIndexed(commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

void VulkanCommandList::BindPipeline(const GPUGraphicsPipelineRef &pipeline) {
    for (int i = 0; i < currentDescriptorSets.size(); i++) {
        BindDescriptorSet(currentPipeline, currentDescriptorSets[i], i);
    }

    currentDescriptorSets.clear();

    VulkanGraphicsPipeline *vkPipeline = static_cast<VulkanGraphicsPipeline *>(pipeline.get());

    currentPipeline = pipeline;

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipeline->PipelineHandle());

    for (int i = 0; i < vkPipeline->DescriptorLayouts().size(); i++) {
        VulkanDescriptorSetRef descriptorSet = gpu->CreateDescriptorSet(pipeline, i);
        currentDescriptorSets.push_back(descriptorSet);
        ownedResources.push_back(descriptorSet);
    }
}

void VulkanCommandList::SetViewport(float offsetX, float offsetY, float width, float height) {
    VkViewport viewport;
    viewport.x = offsetX;
    viewport.y = offsetY;
    viewport.width = width;
    viewport.height = height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
}

void VulkanCommandList::SetScissors(int32_t offsetX, int32_t offsetY, uint32_t width, uint32_t height) {
    VkRect2D scissor;
    scissor.offset = {offsetX, offsetY};
    scissor.extent = {width, height};

    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

void VulkanCommandList::EndRenderPass() {
    vkCmdEndRenderPass(commandBuffer);
}

void VulkanCommandList::BindVertexBuffer(const GPUBufferRef &buf, int binding) {
    VulkanBuffer *buffer = static_cast<VulkanBuffer *>(buf.get());
    VkBuffer buffers[] = {buffer->Buffer()};
    VkDeviceSize offsets[] = {0};

    vkCmdBindVertexBuffers(commandBuffer, binding, 1, buffers, offsets);
    ownedResources.emplace_back(buf);
}

void VulkanCommandList::BindIndexBuffer(const GPUBufferRef &buf) {
    VulkanBuffer *buffer = static_cast<VulkanBuffer *>(buf.get());

    vkCmdBindIndexBuffer(commandBuffer, buffer->Buffer(), 0, VK_INDEX_TYPE_UINT16);

    ownedResources.emplace_back(buf);
}

void VulkanCommandList::BindUniformBuffer(const std::string &name, const GPUBufferRef &buffer, int size) {
    VulkanGraphicsPipeline *vulkanPipeline = static_cast<VulkanGraphicsPipeline *>(currentPipeline.get());
    DescriptorSetBindingPoint bindingPoint = vulkanPipeline->params[name];
    VulkanDescriptorSetRef descriptorSet = currentDescriptorSets[bindingPoint.set];
    descriptorSet->Update(bindingPoint.binding, buffer, size);
}

void VulkanCommandList::BindTexture(const std::string &name, const GPUTextureRef &texture, const GPUSamplerRef &sampler) {
    VulkanGraphicsPipeline *vulkanPipeline = static_cast<VulkanGraphicsPipeline *>(currentPipeline.get());
    DescriptorSetBindingPoint bindingPoint = vulkanPipeline->params[name];
    VulkanDescriptorSetRef descriptorSet = currentDescriptorSets[bindingPoint.set];

    VulkanImage *vulkanImage = static_cast<VulkanImage *>(texture.get());
    VulkanSampler *vulkanSampler = static_cast<VulkanSampler *>(sampler.get());
    descriptorSet->Update(bindingPoint.binding, *vulkanImage, *vulkanSampler);
}

void VulkanCommandList::BindDescriptorSet(const GPUGraphicsPipelineRef &pipeline,
                                          const VulkanDescriptorSetRef &descriptorSet, int binding) {
    VulkanGraphicsPipeline *vulkanPipeline = static_cast<VulkanGraphicsPipeline *>(pipeline.get());

    VulkanDescriptorSet *set = static_cast<VulkanDescriptorSet *>(descriptorSet.get());

    VkDescriptorSet descriptorSets[] = {set->DescriptorSetHandle()};

    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanPipeline->LayoutHandle(), binding, 1, descriptorSets, 0, nullptr);

    ownedResources.emplace_back(descriptorSet);
}