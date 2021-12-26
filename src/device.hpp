#ifndef LOGICAL_DEVICE_HPP
#define LOGICAL_DEVICE_HPP

#include<vulkan/vulkan.h>
#include<string>
#include<vector>
#include<set>
#include<stdexcept>
#include<iostream>

#include"queue.hpp"
#include"validation.hpp"

namespace ive {

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    // TODO: rename
    class LogicalDevice {

        public:

            // TODO: Dependencies a bit of a mess here for initialization.
            LogicalDevice(VkInstance instance_, const VkSurfaceKHR &surface_);
            ~LogicalDevice();
            VkDevice getLogicalDeviceHandle() { return logicalDevice; };
            VkQueue graphicsQueue() { return queueManager.getGraphicsQueue(); }
            VkQueue presentQueue() { return queueManager.getPresentQueue(); }
            SwapChainSupportDetails querySwapChainSupport(VkSurfaceKHR surface);
            SwapChainSupportDetails getSwapChainSupport(VkSurfaceKHR surface); 
            QueueFamilyIndices getQueueFamilyIndices() { 
                return queueManager.getQueueFamilyIndices();
            }

        private:

            VkInstance instance;
            VkDevice logicalDevice;
            VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
            QueueManager queueManager;

            // TODO: what for is this?
            VkPhysicalDeviceProperties properties;

            const std::vector<const char *> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

            // void pickPhysicalDevice();
            // void createLogicalDevice();
            bool checkDeviceExtensionSupport(VkPhysicalDevice device);
            bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface_);
            void pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface_);
            void createLogicalDevice(VkSurfaceKHR surface_);

    };
}

#endif