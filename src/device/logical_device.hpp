#ifndef LOGICAL_DEVICE_HPP
#define LOGICAL_DEVICE_HPP

#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <set>
#include <stdexcept>
#include <iostream>

#include "queue.hpp"
#include "validation.hpp"
#include "physical_device.hpp"

namespace ive {

    // TODO: rename
    class LogicalDevice {

        public:

            // TODO: Dependencies a bit of a mess here for initialization.
            LogicalDevice(const VkSurfaceKHR &surface_, const PhysicalDevice& phys, QueueManager& queueManager);
            ~LogicalDevice();
            //VkDevice getLogicalDeviceHandle() { return logicalDevice; };
            //VkQueue graphicsQueue() { return queueManager.getGraphicsQueue(); }
            //VkQueue presentQueue() { return queueManager.getPresentQueue(); }
            // SwapChainSupportDetails querySwapChainSupport(VkSurfaceKHR surface);
            // SwapChainSupportDetails getSwapChainSupport(VkSurfaceKHR surface); 
            //QueueFamilyIndices getQueueFamilyIndices() { 
            //    return queueManager.getQueueFamilyIndices();
            //}

        private:


           // VkInstance instance;
            VkDevice logicalDevice;
            // QueueManager queueManager;

            // TODO: what for is this?
            //VkPhysicalDeviceProperties properties;


            // void pickPhysicalDevice();
            void createLogicalDevice(const VkSurfaceKHR& surface_, const PhysicalDevice& PhysicalDevice, QueueManager& queueManager);
            // bool checkDeviceExtensionSupport(const VkPhysicalDevice& device);
            // bool isDeviceSuitable(const VkPhysicalDevice& device, VkSurfaceKHR surface_);
            // void pickPhysicalDevice(const VkInstance& instance, const VkSurfaceKHR& surface_);
            // void createLogicalDevice(VkSurfaceKHR surface_);

    };
}

#endif