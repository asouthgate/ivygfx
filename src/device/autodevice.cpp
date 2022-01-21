#include <vulkan/vulkan.hpp>
#include <boost/log/trivial.hpp>

#include "autodevice.hpp"

namespace ivy {
    

    AutoDevice::~AutoDevice() {
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroySemaphore(logicalDevice.getLogicalDeviceHandle(), renderFinishedSemaphores[i], nullptr);
            vkDestroySemaphore(logicalDevice.getLogicalDeviceHandle(), imageAvailableSemaphores[i], nullptr);
            vkDestroyFence(logicalDevice.getLogicalDeviceHandle(), inFlightFences[i], nullptr);
        }    
    }

    void AutoDevice::main() {
        while (!window.shouldClose()) {
            BOOST_LOG_TRIVIAL(debug) << "AutoDevice::main:: starting loop again";
            window.pollEvents();
            drawFrame();
        }

    }

    void AutoDevice::drawFrame() {
        // 1 get image from swap chain
        // 2 execute command buffer with that image as attachment
        // 3 retrn the image to swap chain for presentation
        // ez?

        vkWaitForFences(logicalDevice.getLogicalDeviceHandle(), 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

        uint32_t imageIndex;
        swapChain.acquireNextImage(imageIndex, imageAvailableSemaphores[currentFrame]);

        if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
            vkWaitForFences(logicalDevice.getLogicalDeviceHandle(), 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
        }
        imagesInFlight[imageIndex] = inFlightFences[currentFrame];

        // Create nfo for sumission
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        // TODO: extract out
        // Create semaphores
        // TODO: should this happen in the main loop? Should the semaphores be 'created'  here
        // or just used?
        VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        auto& commandBuffers = commandPool.getCommandBuffersHandle();
        submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

        VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;
        vkResetFences(logicalDevice.getLogicalDeviceHandle(), 1, &inFlightFences[currentFrame]);

        queueManager.submitToGraphicsQueue(submitInfo, inFlightFences[currentFrame]);

        // // Inter-subpass memory and execution dependencies
        // VkSubpassDependency dependency{};
        // dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        // dependency.dstSubpass = 0;
        // dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        // dependency.srcAccessMask = 0;
        // dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        // dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;


        // PRESENTATION

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;
        VkSwapchainKHR swapChains[] = {swapChain.getSwapChainHandle()};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;
        presentInfo.pResults = nullptr; // Optional
        queueManager.submitToPresentQueue(presentInfo);
        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    // TODO: rename, not just semaphres
    void AutoDevice::createSemaphores() {

        imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
        int n_images = swapChain.getSwapChainImageViews().size();
        imagesInFlight.resize(n_images, VK_NULL_HANDLE);

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            if (vkCreateSemaphore(logicalDevice.getLogicalDeviceHandle(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
                vkCreateSemaphore(logicalDevice.getLogicalDeviceHandle(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
                vkCreateFence(logicalDevice.getLogicalDeviceHandle(), &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create synchronization objects for a frame!");
            }
        }

    }

}