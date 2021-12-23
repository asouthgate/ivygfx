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
#include"swapchain_chk.hpp"

namespace ive {

    // TODO: rename
    class LogicalDevice {

        public:

            // TODO: Dependencies a bit of a mess here for initialization.
            LogicalDevice(VkInstance instance_, const VkSurfaceKHR &surface_, 
                                        ValidationLayers &validationLayers) 
                                    :   instance(instance_),
                                        queueManager((pickPhysicalDevice(surface_), physicalDevice), surface_) {
                createLogicalDevice(validationLayers, surface_);
            }
            ~LogicalDevice() {
                vkDestroyDevice(logicalDevice, nullptr);
            }
            VkDevice getLogicalDeviceHandle() { return logicalDevice; };
            VkQueue graphicsQueue() { return queueManager.getGraphicsQueue(); }
            VkQueue presentQueue() { return queueManager.getPresentQueue(); }
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
            void pickPhysicalDevice(VkSurfaceKHR surface_);
            void createLogicalDevice(ValidationLayers &validationLayers, VkSurfaceKHR surface_);

    };
}

#endif