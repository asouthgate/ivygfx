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

    //  This class abstracts away details of device creation.
    //
    //  With Vulkan, we must choose a physical device, which requires considerable setup.
    //  We must also setup a logical VkDevice that will be used for later operations.
    //  We must also setup a debug messenger, surface, swap chain, etc. 
    //  The idea is to hide as much of that configuration as possible. 
    //  This code can then be used as a skeleton and configured on a project-specific basis.
    //  Many of these operations have relationships or require references to Vulkan pointers.
    //  
    //  TODO: review class design
    class AutoDevice {

        #ifdef NDEBUG
            const bool enableValidationLayers = false;
        #else
            const bool enableValidationLayers = true;
        #endif

        public:

            // TODO: create initialization args such as window size and name
            AutoDevice() :
                window(100, 100, "foo"),
                // debugMessenger is a singleton
                // TODO: move the nested function out to a setup function
                vkinstance(instance.getVkInstanceHandle()),
                debugMessenger(vkinstance),
                physicalDevice{vkinstance, window.createWindowSurface(vkinstance)},
                queueManager(physicalDevice.getVkPhysicalDeviceHandle(), window.getSurfaceHandle()),
                logicalDevice(window.getSurfaceHandle(), physicalDevice, queueManager, debugMessenger),
                swapChain(physicalDevice, window.getSurfaceHandle(), logicalDevice, window.getWindowPtr(), queueManager) {};

            // We absolutely do not want copying, moving of this class
            // Because of the way Vk pointers work
            ~AutoDevice() {}
            AutoDevice(const AutoDevice &) = delete;
            void operator=(const AutoDevice &) = delete;
            AutoDevice(AutoDevice &&) = delete;
            AutoDevice &operator=(AutoDevice &&) = delete;

            // Helper function to return VkDevice
            // TODO: rename to getVkDevice(). At the moment is not clear which device this refers to
            VkDevice& device() {
                    // TODO: replace with proper logging
                    BOOST_LOG_TRIVIAL(debug) << "AutoDevice:: calling getter device() ";   
                    VkDevice& ld = logicalDevice.getLogicalDeviceHandle();
                    BOOST_LOG_TRIVIAL(debug) << "\t AutoDevice:: going to return VkDevice " << ld;    
                    return ld;
                }
            
            // Get a handle to the debugMessenger object (not Vk)
            DebugMessenger& getDebugMessengerHandle() {return debugMessenger; }

        private:

            // Fundamental components of the device
            iveWindow window;
            Instance instance;
            DebugMessenger debugMessenger;
            // TODO: could wrap vkinstance, but won't bother, wrapping up too much is more danger
            VkInstance& vkinstance;  
            // VkSurfaceKHR surface;
            PhysicalDevice physicalDevice;
            QueueManager queueManager;  
            LogicalDevice logicalDevice;
            SwapChain swapChain;

            // Swap chain should have these?
            // std::vector<VkImageView> swapChainImageViews;    
    };
}

#endif
