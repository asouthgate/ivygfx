#include <vulkan/vulkan.h>
#include <stdexcept>

#include "command.hpp"
#include "queue.hpp"

namespace ive {

    // Create a command pool
    //
    // Recall command pools are used to submit commands such as drawing
    CommandPool::CommandPool(VkDevice vkdev_, const QueueFamilyIndices& qfi) : vkdev(vkdev_) {
//        QueueFamilyIndices queueFamilyIndices = findPhysicalQueueFamilies();

        VkCommandPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        // NB here we use the graphics family
        poolInfo.queueFamilyIndex = qfi.graphicsFamily;
        // First bit specifies short lived command buffers allocated from pool
        // Second means command buffers allocated allows resetting to state zero
        poolInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        // As usual, try and boom if not
        if (vkCreateCommandPool(vkdev, &poolInfo, nullptr, &vkcpool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create command pool!");
        }
    }

//  Start command buffer recording.  
    //
    //
    //  Command buffers must start "recording"; a sort of session must begin.
    //    Also do some parameter config. This is designed for "single time"
    //    commands.
    // TODO: Rename "single time"
    //
    //  Returns:
    //    commandBuffer (VkCommandBuffer)
    SingleCommandBuffer::SingleCommandBuffer(VkDevice &device, VkCommandPool& commandPool) {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = commandPool;
        allocInfo.commandBufferCount = 1;

        vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        // Start recording command buffer
        vkBeginCommandBuffer(commandBuffer, &beginInfo);
    }

    // Stop the command buffer recording
    void SingleCommandBuffer::consume(VkQueue& queue, VkCommandPool& commandPool, VkDevice& device) {
        if (!consumed) {
            vkEndCommandBuffer(commandBuffer);

            VkSubmitInfo submitInfo{};
            submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
            submitInfo.commandBufferCount = 1;
            submitInfo.pCommandBuffers = &commandBuffer;

            vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
            vkQueueWaitIdle(queue);

            vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);

            consumed = true;
        }
        else {throw std::runtime_error("Consuming a SingleCommandBuffer more than once is not allowed.");}
    }
}