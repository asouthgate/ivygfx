#ifndef LOGICAL_DEVICE_HPP
#define LOGICAL_DEVICE_HPP

#include <vulkan/vulkan.h>

#include "queue.hpp"
#include "debug_messenger.hpp"
#include "physical_device.hpp"

namespace ivy {

    // TODO: rename
    class LogicalDevice {

        public:

            // TODO: Dependencies a bit of a mess here for initialization.
            LogicalDevice(const VkSurfaceKHR &surface_, const PhysicalDevice& phys, QueueManager& queueManager, const DebugMessenger& dm);
            ~LogicalDevice();

            // Because of destructor, we want to disable copy
            LogicalDevice(const LogicalDevice&) = delete;
            LogicalDevice &operator=(const LogicalDevice&) = delete;

            const VkDevice& getLogicalDeviceHandle() const;

            VkDevice& getLogicalDeviceHandle();

        private:

            VkDevice logicalDevice;
            void createLogicalDevice(const PhysicalDevice& PhysicalDevice, 
                                    QueueManager& queueManager, const DebugMessenger& dm);
    };
}

#endif