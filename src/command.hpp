#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <vulkan/vulkan.h>

#include "queue.hpp"
#include "device.hpp"

namespace ive {

    // The command buffer object wraps the vulkan API command buffer functionality
    // and hides some implementation details
    class SingleCommandBuffer {
        public:
            SingleCommandBuffer(const VkDevice &device, const VkCommandPool& commandPool);
            void submit(VkQueue& queue, VkCommandPool& commandPool, VkDevice& device);
            // Again, recall VkCommandBuffer is in itself a ptr... 
            VkCommandBuffer getVkCommandBuffer() {return commandBuffer;}
        private:
            VkCommandBuffer commandBuffer;
            bool consumed;
        protected:
    };

    class CommandPool {

        public:
            // TODO: temporary; remove this in ive_device eventually
//            CommandPool();
//            CommandPool(VkDevice vkdev_, const QueueFamilyIndices& qfi);
            CommandPool(LogicalDevice& ld);
            ~CommandPool() {
                vkDestroyCommandPool(logicalDevice.getLogicalDeviceHandle(), vkcpool, nullptr);
            }

            SingleCommandBuffer getSingleCommandBuffer();
            VkCommandPool& getVkCPool() { return vkcpool; }

        private:
            VkCommandPool vkcpool;
            // Be aware that VkDevice is itself a pointer
            // VkDevice vkdev;
            LogicalDevice &logicalDevice;

        protected:
    };
}

#endif