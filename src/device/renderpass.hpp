#ifndef RENDERPASS_HPP
#define RENDERPASS_HPP

#include "logical_device.hpp"
#include "swapchain.hpp"


namespace ivy {

    class RenderPass {

        public:
            RenderPass(LogicalDevice& logicalDevice, SwapChain& swapChain);
            ~RenderPass();

            RenderPass(const RenderPass &) = delete;
            void operator=(const RenderPass &) = delete;
            RenderPass(RenderPass &&) = delete;
            RenderPass &operator=(RenderPass &&) = delete;

            VkRenderPass& getRenderPassHandle();

        private:
            void createRenderPass(LogicalDevice& logicalDevice, SwapChain& swapChain);
            VkRenderPass renderPass;
            LogicalDevice& logicalDevice;
    };

}

#endif