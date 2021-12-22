#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <vulkan/vulkan.h>

#include "queue.hpp"

namespace ive {
    // The command buffer object wraps the vulkan API command buffer functionality
    // and hides some implementation details
    class SingleCommandBuffer {
        public:
            SingleCommandBuffer(VkDevice &device, VkCommandPool& commandPool);
            void consume(VkQueue& queue, VkCommandPool& commandPool, VkDevice& device);
        private:
            VkCommandBuffer commandBuffer;
            bool consumed;
        protected:
    };

    class CommandPool {
        public:
            // TODO: temporary; remove this in ive_device eventually
            CommandPool();
            CommandPool(VkDevice vkdev_, const QueueFamilyIndices& qfi);
            ~CommandPool() {
                vkDestroyCommandPool(vkdev, vkcpool, nullptr);
            }
            VkCommandPool& getVkCPool() { return vkcpool; }
        private:
            VkCommandPool vkcpool;
            // Be aware that VkDevice is itself a pointer
            VkDevice vkdev;
        protected:
    };
}

#endif