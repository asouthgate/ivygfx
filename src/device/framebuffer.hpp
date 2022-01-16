#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include <vector>
#include <vulkan/vulkan.h>

#include "swapchain.hpp"
#include "logical_device.hpp"
#include "renderpass.hpp"


namespace ivy {

    class FrameBufferCollection {

        public:
            FrameBufferCollection(LogicalDevice& ld, SwapChain& sc, RenderPass& rp);
            ~FrameBufferCollection();
            
            std::vector<VkFramebuffer>& getFrameBuffersHandle() { return framebuffers; }

        private:
            std::vector<VkFramebuffer> framebuffers;
            LogicalDevice& logicalDevice;
    };

}


#endif