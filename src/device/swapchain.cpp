#include <cstdint> 
#include <algorithm> 

#include <vulkan/vulkan.h>
#include <vector>
#include <stdexcept>

#include "ive_window.hpp"
#include "physical_device.hpp"
#include "logical_device.hpp"
#include "swapchain.hpp"

namespace ive {

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

        // Create a swap chain given a physical device, a surface, and a window
        // TODO: is the window necessary or can we pass something simpler in?
        void SwapChain::createSwapChain(const PhysicalDevice& physicalDevice, 
                            const VkSurfaceKHR& surf, LogicalDevice& logicalDevice,
                            GLFWwindow* window_ptr, QueueManager& queueManager) {

            // TODO: code smell
            const VkPhysicalDevice & pd = physicalDevice.getVkPhysicalDeviceHandle();

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
            VkSwapchainKHR swapChain;

            if (vkCreateSwapchainKHR(logicalDevice.getLogicalDeviceHandle(), &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
                throw std::runtime_error("failed to create swap chain!");
            }

            vkGetSwapchainImagesKHR(logicalDevice.getLogicalDeviceHandle(), swapChain, &imageCount, nullptr);
            swapChainImages.resize(imageCount);
            vkGetSwapchainImagesKHR(logicalDevice.getLogicalDeviceHandle(), swapChain, &imageCount, swapChainImages.data());

            swapChainImageFormat = surfaceFormat.format;
            swapChainExtent = extent;

        }

        void SwapChain::callVkDestructors() {
            for (auto imageView : swapChainImageViews) {
                vkDestroyImageView(logicalDeviceHandle.getLogicalDeviceHandle(), imageView, nullptr);
            }
            vkDestroySwapchainKHR(logicalDeviceHandle.getLogicalDeviceHandle(), swapChain, nullptr);
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

}
