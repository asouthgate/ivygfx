#ifndef AUTODEVICE_HPP
#define AUTODEVICE_HPP

#include <vulkan/vulkan.h>

#include "instance.hpp"
#include "physical_device.hpp"
#include "logical_device.hpp"
#include "queue.hpp"
#include "ive_window.hpp"
#include "debug_messenger.hpp"
#include "swapchain.hpp"

namespace ive {

    class AutoDevice {

        #ifdef NDEBUG
            const bool enableValidationLayers = false;
        #else
            const bool enableValidationLayers = true;
        #endif

        public:
            AutoDevice() :
                window(100, 100, "foo"),
                debugMessenger(DebugMessenger::get_instance((ive::createInstance(vkinstance), vkinstance))),
                surface(window.createWindowSurface(vkinstance)),
                physicalDevice{vkinstance, surface},
                queueManager(physicalDevice.getVkPhysicalDeviceHandle(), surface),
                logicalDevice(surface, physicalDevice, queueManager, debugMessenger),
                swapChain(physicalDevice, surface, logicalDevice, window.getWindowPtr(), queueManager) {};
            ~AutoDevice() {}

            AutoDevice(const AutoDevice &) = delete;
            void operator=(const AutoDevice &) = delete;
            AutoDevice(AutoDevice &&) = delete;
            AutoDevice &operator=(AutoDevice &&) = delete;

            VkDevice& device() {
                    BOOST_LOG_TRIVIAL(debug) << "AutoDevice:: calling getter device() ";   
                    VkDevice& ld = logicalDevice.getLogicalDeviceHandle();
                    BOOST_LOG_TRIVIAL(debug) << "\t AutoDevice:: going to return VkDevice " << ld;    
                    return ld;
                }
        private:  
            iveWindow window;
            DebugMessenger debugMessenger;
            VkInstance vkinstance;  
            VkSurfaceKHR surface;
            PhysicalDevice physicalDevice;
            LogicalDevice logicalDevice;
            QueueManager queueManager;  
            SwapChain swapChain;

            std::vector<VkImageView> swapChainImageViews;    
    };
}

#endif
