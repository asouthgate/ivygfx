#pragma once

#include <vulkan/vulkan.h>
#include <boost/log/trivial.hpp>
#include <vector>
#include <iostream>

#include "queue.hpp"
//#include "swapchain_chk.hpp"

namespace ive {

    // struct SwapChainSupportDetails {
    //     VkSurfaceCapabilitiesKHR capabilities;
    //     std::vector<VkSurfaceFormatKHR> formats;
    //     std::vector<VkPresentModeKHR> presentModes;
    // };


    class PhysicalDevice {

        public:
            PhysicalDevice(const VkInstance& instance, const VkSurfaceKHR& surface_);
            //urgent: destroy something?
            ~PhysicalDevice() {};

            const VkPhysicalDevice& getVkPhysicalDeviceHandle() const { return physicalDevice; }
            const std::vector<const char *> getDeviceExtensions() const { return deviceExtensions; }

        private:
            const std::vector<const char *> deviceExtensions {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
            
            VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
            VkPhysicalDeviceProperties properties;

            // Check the physical device chosen has support for extensions
            bool checkDeviceExtensionSupport(const VkPhysicalDevice& physicalDevice,
                                            const std::vector<const char *>& requiredExtensions);

            bool isDeviceSuitable(const VkPhysicalDevice& device, VkSurfaceKHR surface_);

            void pickPhysicalDevice(const VkInstance& instance, const VkSurfaceKHR& surface_);

    };    

}