#include <vulkan/vulkan.h>
#include <stdexcept>

#include "command.hpp"
#include "queue.hpp"
#include "device.hpp"

namespace ive {

    // Create a command pool
    //
    // Recall command pools are used to submit commands such as drawing
    CommandPool::CommandPool(LogicalDevice& ld_) : logicalDevice(ld_) {
        QueueFamilyIndices queueFamilyIndices = logicalDevice.getQueueFamilyIndices();

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
    }

    SingleCommandBuffer CommandPool::getSingleCommandBuffer() {
        return SingleCommandBuffer(logicalDevice.getLogicalDeviceHandle(), vkcpool);
    }

    //  Create a single use command buffer in a command pool for a given device.  
    //
    //
    //  Command buffers must start "recording"; a sort of session must begin.
    //    Also do some parameter config. This is designed for "single time"
    //    commands.
    // TODO: Rename "single time"
    //
    //  Returns:
    //    commandBuffer (VkCommandBuffer)
    SingleCommandBuffer::SingleCommandBuffer(const VkDevice &device, const VkCommandPool& commandPool) {
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

    // Submit the command buffer to a given queue
    // TODO: queue.submit(command) or commandl.submit(queue)?
    // Do we tell the command to go in to queue or the queue to accept a command?
    // \__#.#__/
    // Does the data go to the pipe, or the pipe to the data?
    void SingleCommandBuffer::submit(VkQueue& queue, VkCommandPool& commandPool, VkDevice& device) {
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
        else {throw std::runtime_error("Submitting a SingleCommandBuffer more than once is not allowed.");}
    }
}