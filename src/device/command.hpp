#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <vulkan/vulkan.h>

#include "queue.hpp"
#include "logical_device.hpp"
#include "swapchain.hpp"
#include "pipeline.hpp"
#include "framebuffer.hpp"
#include "renderpass.hpp"

namespace ivy {

    class CommandPool {

        public:
            CommandPool(LogicalDevice& ld, QueueManager& qm_, SwapChain& swapChain, uint32_t n);
            ~CommandPool();
            void startRecording(LogicalDevice &logicalDevice, FrameBufferCollection& frameBufferCollection,
                                SwapChain& swapChain, RenderPass& renderPass, Pipeline& pipeline);
            std::vector<VkCommandBuffer>& getCommandBuffersHandle() { return commandBuffers; }

        private:
            VkCommandPool vkcpool;
            // Be aware that VkDevice is itself a pointer
            // VkDevice vkdev;
            std::vector<VkCommandBuffer> commandBuffers;
            LogicalDevice &logicalDevice;
            VkCommandPool& getVkCPool() { return vkcpool; }

        protected:
    };
}

#endif