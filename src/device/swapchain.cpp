#include <cstdint> 
#include <algorithm> 
#include <iostream>
#include <vulkan/vulkan.h>
#include <vector>
#include <stdexcept>

#include <boost/log/trivial.hpp>

#include "window.hpp"
#include "physical_device.hpp"
#include "logical_device.hpp"
#include "swapchain.hpp"

namespace ivy {

    SwapChain::SwapChain(ivy::PhysicalDevice physicalDevice,     
                VkSurfaceKHR& surface,
                ivy::LogicalDevice& logicalDevice,
                GLFWwindow* window_ptr, ivy::QueueManager queueManager)
                    : logicalDeviceHandle(logicalDevice)
                {
        BOOST_LOG_TRIVIAL(debug) << "SwapChain::constructor called";
        createSwapChain(physicalDevice, surface, logicalDevice, window_ptr, queueManager);
        createImageViews(logicalDevice.getLogicalDeviceHandle());
        //createFrameBuffers();
    }

    void SwapChain::acquireNextImage(uint32_t& imageIndex, VkSemaphore& semaphore) {
        vkAcquireNextImageKHR(logicalDeviceHandle.getLogicalDeviceHandle(), swapChain, UINT64_MAX, 
                                semaphore, VK_NULL_HANDLE, &imageIndex);
    }


    // void SwapChain::CreateFrameBuffers() {
    //     swapChainFramebuffers.resize(swapChainImageViews.size());
    //     for (size_t i = 0; i < swapChainImageViews.size(); i++) {
    //         VkImageView attachments[] = {
    //             swapChainImageViews[i]
    //         };

    //         VkFramebufferCreateInfo framebufferInfo{};
    //         framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    //         framebufferInfo.renderPass = renderPass;
    //         framebufferInfo.attachmentCount = 1;
    //         framebufferInfo.pAttachments = attachments;
    //         framebufferInfo.width = swapChainExtent.width;
    //         framebufferInfo.height = swapChainExtent.height;
    //         framebufferInfo.layers = 1;

    //         if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
    //             throw std::runtime_error("failed to create framebuffer!");
    //         }
    //     }
    // }

    SwapChain::~SwapChain() {
        BOOST_LOG_TRIVIAL(debug) << "SwapChain::destructor called";
        callVkDestructors();
        BOOST_LOG_TRIVIAL(debug) << "SwapChain::destructor complete";
    }


    // Create a swap chain given a physical device, a surface, and a window
    // TODO: is the window necessary or can we pass something simpler in?
    void SwapChain::createSwapChain(const PhysicalDevice& physicalDevice, 
                        const VkSurfaceKHR& surf, LogicalDevice& logicalDevice,
                        GLFWwindow* window_ptr, QueueManager& queueManager) {

        BOOST_LOG_TRIVIAL(debug) << "SwapChain::createSwapChain with...";
        std::cerr << "\t surface " << surf << std::endl;
        std::cerr << "\t window_ptr" << window_ptr << std::endl;

        // TODO: code smell
        const VkPhysicalDevice & pd = physicalDevice.getVkPhysicalDeviceHandle();
        std::cerr << "\t physicalDevice " << pd << std::endl;

        // Query what swap chain features are supported 
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(pd, surf);

        // Get surface format
        VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);

        // Get present mode
        VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);

        // Finally some basic params: resolution and number of images
        // Get resolution
        VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities, window_ptr);

        // Get max images to use
        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
        if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        // Finally create
        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = surf;
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyIndices indices = queueManager.getQueueFamilyIndices();
        uint32_t gfi = queueManager.getGraphicsFamily();
        uint32_t pfi = queueManager.getPresentFamily();
        uint32_t queueFamilyIndices[] = {gfi, pfi};

        if (indices.graphicsFamily != indices.presentFamily) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        } else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0; // Optional
            createInfo.pQueueFamilyIndices = nullptr; // Optional
        }

        createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        VkDevice& ld = logicalDevice.getLogicalDeviceHandle();

        std::cerr << "\t logicalDevice " << ld << std::endl;

        if (vkCreateSwapchainKHR(ld, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
            throw std::runtime_error("failed to create swap chain!");
        }
        BOOST_LOG_TRIVIAL(debug) << "SwapChain::created VkSwapChainHKR:" << swapChain ;

        vkGetSwapchainImagesKHR(ld, swapChain, &imageCount, nullptr);
        swapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(ld, swapChain, &imageCount, swapChainImages.data());

        swapChainImageFormat = surfaceFormat.format;
        swapChainExtent = extent;

        BOOST_LOG_TRIVIAL(debug) << "SwapChain::created my SwapchainKHR" << swapChain;
        BOOST_LOG_TRIVIAL(debug) << "SwapChain::created my SwapchainKHR at" << &swapChain;

    }

    VkExtent2D& SwapChain::getSwapChainExtent() {
        return swapChainExtent;
    }

    VkFormat& SwapChain::getSwapChainImageFormat() {
        return swapChainImageFormat;
    }

    std::vector<VkImageView>& SwapChain::getSwapChainImageViews() {
        return swapChainImageViews;
    }

    void SwapChain::callVkDestructors() {

        VkDevice& ld = logicalDeviceHandle.getLogicalDeviceHandle();


        BOOST_LOG_TRIVIAL(debug) << "SwapChain::destroying myself with logical device:" << ld ;
        // BOOST_LOG_TRIVIAL(debug) << "SwapChain::destroying my framebuffers" << swapChain;
        // for (auto framebuffer : swapChainFramebuffers) {
        //     vkDestroyFramebuffer(ld, framebuffer, nullptr);
        // }
        BOOST_LOG_TRIVIAL(debug) << "SwapChain::destroying my SwapchainKHR" << swapChain;
        BOOST_LOG_TRIVIAL(debug) << "SwapChain::destroying my SwapchainKHR at" << &swapChain;
        vkDestroySwapchainKHR(ld, swapChain, nullptr);
        for (auto imageView : swapChainImageViews) {
            BOOST_LOG_TRIVIAL(debug) << "SwapChain::destroying my imageView" << imageView;
            vkDestroyImageView(ld, imageView, nullptr);
        }
    }

 // ***** FUNCTIONS FOR 3 SWAP CHAIN PROPERTIES:
    VkSurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {

        if (availableFormats.size() == 0) {
            throw std::runtime_error("no formats available!");
        }

        for (const auto& availableFormat : availableFormats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return availableFormat;
            }
        }
        return availableFormats[0];
    }

    VkPresentModeKHR SwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
        for (const auto& availablePresentMode : availablePresentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return availablePresentMode;
            }
        }
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D SwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window_ptr) {
        if (capabilities.currentExtent.width != UINT32_MAX) {
            return capabilities.currentExtent;
        } else {
            int width, height;
            glfwGetFramebufferSize(window_ptr, &width, &height);

            VkExtent2D actualExtent = {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
            };

            actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return actualExtent;
        }
    }
    // *****

    // NB the swap chain is not actually supported by default; it is highly tied to
    // the windowing system
    SwapChainSupportDetails SwapChain::querySwapChainSupport(const VkPhysicalDevice& device, const VkSurfaceKHR& surface) {

        SwapChainSupportDetails details;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

        if (formatCount != 0) {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

        if (presentModeCount != 0) {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(
                device,                     
                surface,
                &presentModeCount,
                details.presentModes.data());
        }
        return details;
    }

    SwapChainSupportDetails SwapChain::getSwapChainSupport(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) { 
        return querySwapChainSupport(physicalDevice, surface); 
    }





    void SwapChain::createImageViews(VkDevice& device) {
        swapChainImageViews.resize(swapChainImages.size());
        for (size_t i = 0; i < swapChainImages.size(); i++) {
            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = swapChainImages[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = swapChainImageFormat;
            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(device, &createInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create image views!");
            }
        }

    }


} //namespace
