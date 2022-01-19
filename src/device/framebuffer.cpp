#include <boost/log/trivial.hpp>

#include "framebuffer.hpp"

namespace ivy {

    // TODO: as with much of this code, you should program to an interface, not be getting details from the classes

    FrameBufferCollection::FrameBufferCollection(LogicalDevice& ld, SwapChain& sc, RenderPass& rp) : logicalDevice(ld) {

        std::vector<VkImageView>& swapChainImageViews = sc.getSwapChainImageViews();
        VkExtent2D& swapChainExtent = sc.getSwapChainExtent();


        framebuffers.resize(swapChainImageViews.size());
        for (size_t i = 0; i < swapChainImageViews.size(); i++) {
            VkImageView attachments[] = {
                swapChainImageViews[i]
            };

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = rp.getRenderPassHandle();
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = swapChainExtent.width;
            framebufferInfo.height = swapChainExtent.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(ld.getLogicalDeviceHandle(), &framebufferInfo, nullptr, &framebuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create framebuffer!");
            }
        }
    }

    FrameBufferCollection::~FrameBufferCollection() {
        BOOST_LOG_TRIVIAL(debug) << "FrameBufferCollection::destroying my framebuffers";
        for (auto framebuffer : framebuffers) {
            vkDestroyFramebuffer(logicalDevice.getLogicalDeviceHandle(), framebuffer, nullptr);
        }

    }
}