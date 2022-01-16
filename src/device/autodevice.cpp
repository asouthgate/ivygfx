#include <vulkan/vulkan.hpp>

#include "autodevice.hpp"

namespace ivy {
    

    AutoDevice::~AutoDevice() {
        vkDestroySemaphore(logicalDevice.getLogicalDeviceHandle(), renderFinishedSemaphore, nullptr);
        vkDestroySemaphore(logicalDevice.getLogicalDeviceHandle(), imageAvailableSemaphore, nullptr);
    }

    void AutoDevice::main() {
        while (!window.shouldClose()) {
            window.pollEvents();
            drawFrame();
        }

    }

    void AutoDevice::drawFrame() {
        // 1 get image from swap chain
        // 2 execute command buffer with that image as attachment
        // 3 retrn the image to swap chain for presentation
        // ez?

        uint32_t imageIndex;
        swapChain.acquireNextImage(imageIndex, imageAvailableSemaphore);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {imageAvailableSemaphore};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        auto& commandBuffers = commandPool.getCommandBuffersHandle();
        submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

        queueManager.submitToGraphicsQueue(submitInfo);

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        // renderPassInfo.dependencyCount = 1;
        // renderPassInfo.pDependencies = &dependency;

    }

    void AutoDevice::createSemaphores() {
        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        if (vkCreateSemaphore(logicalDevice.getLogicalDeviceHandle(), &semaphoreInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS ||
            vkCreateSemaphore(logicalDevice.getLogicalDeviceHandle(), &semaphoreInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS) {

            throw std::runtime_error("failed to create semaphores!");
        }


    }

}