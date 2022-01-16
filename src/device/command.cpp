#include <vulkan/vulkan.h>
#include <stdexcept>

#include "command.hpp"
#include "queue.hpp"
#include "logical_device.hpp"
#include "queue.hpp"
#include "framebuffer.hpp"
#include "pipeline.hpp"

namespace ivy {

    // Create a command pool
    //
    // Recall command pools are used to submit commands such as drawing
    CommandPool::CommandPool(LogicalDevice& ld, QueueManager& qm_, SwapChain& swapChain, uint32_t n) : logicalDevice(ld) {
        QueueFamilyIndices queueFamilyIndices = qm_.getQueueFamilyIndices();

        VkCommandPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        // NB here we use the graphics family
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
        // First bit specifies short lived command buffers allocated from pool
        // Second means command buffers allocated allows resetting to state zero
        poolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        // As usual, try and boom if not
        if (vkCreateCommandPool(logicalDevice.getLogicalDeviceHandle(), &poolInfo, 
                                nullptr, &vkcpool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create command pool!");
        }

        commandBuffers.resize(n);

        // allocate

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = vkcpool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();

        if (vkAllocateCommandBuffers(logicalDevice.getLogicalDeviceHandle(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers!");
        }
    }

    CommandPool::~CommandPool() {
        vkDestroyCommandPool(logicalDevice.getLogicalDeviceHandle(), vkcpool, nullptr);
    }


    //MultipleCommandBuffers::MultipleCommandBuffers(const VkDevice &device, const VkCommandPool& commandPool, uint32_t n) {
    void CommandPool::startRecording(LogicalDevice &logicalDevice, FrameBufferCollection& frameBufferCollection, 
                                    SwapChain& swapChain, RenderPass& renderPass, Pipeline& pipeline) {


        std::vector<VkFramebuffer>& framebuffers = frameBufferCollection.getFrameBuffersHandle();

        // start recording
        for (size_t i = 0; i < commandBuffers.size(); i++) {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.flags = 0; // Optional
            beginInfo.pInheritanceInfo = nullptr; // Optional

            if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
                throw std::runtime_error("failed to begin recording command buffer!");
            }

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = renderPass.getRenderPassHandle();
            renderPassInfo.framebuffer = framebuffers[i];
            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = swapChain.getSwapChainExtent();

            VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
            renderPassInfo.clearValueCount = 1;
            renderPassInfo.pClearValues = &clearColor;

            vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getGraphicsPipelineHandle());

            vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

            vkCmdEndRenderPass(commandBuffers[i]);

            if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to record command buffer!");
            }
        }
    }

}