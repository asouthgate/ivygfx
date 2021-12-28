#ifndef AUTODEVICE_HPP
#define AUTODEVICE_HPP

#include <vulkan/vulkan.h>

#include "instance.hpp"
#include "physical_device.hpp"
#include "logical_device.hpp"
#include "queue.hpp"
#include "ive_window.hpp"
#include "debug_messenger.hpp"

namespace ive {

    class AutoDevice {

        #ifdef NDEBUG
            const bool enableValidationLayers = false;
        #else
            const bool enableValidationLayers = true;
        #endif

        public:
            AutoDevice() :
                window{ 100, 100, "foo"},
                debugMessenger{(ive::createInstance(vkinstance), vkinstance)},
                surface(window.createWindowSurface(vkinstance)),
                physicalDevice{vkinstance, surface},
                queueManager(physicalDevice.getVkPhysicalDeviceHandle(), surface),
                logicalDevice{surface, physicalDevice, queueManager} {};
            ~AutoDevice() {}
        private:  
            iveWindow window;
            DebugMessenger debugMessenger;
            VkInstance vkinstance;  
            VkSurfaceKHR surface;
            PhysicalDevice physicalDevice;
            LogicalDevice logicalDevice;
            QueueManager queueManager;      
    };
}

#endif