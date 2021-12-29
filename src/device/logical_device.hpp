#ifndef LOGICAL_DEVICE_HPP
#define LOGICAL_DEVICE_HPP

#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <set>
#include <stdexcept>
#include <iostream>

#include "queue.hpp"
#include "debug_messenger.hpp"
#include "physical_device.hpp"

namespace ive {

    // TODO: rename
    class LogicalDevice {

        public:

            // TODO: Dependencies a bit of a mess here for initialization.
            LogicalDevice(const VkSurfaceKHR &surface_, const PhysicalDevice& phys, QueueManager& queueManager, const DebugMessenger& dm);
            ~LogicalDevice();

            // Because of destructor, we want to disable copy
            LogicalDevice(const LogicalDevice&) = delete;
            LogicalDevice &operator=(const LogicalDevice&) = delete;

            const VkDevice& getLogicalDeviceHandle() const {
                BOOST_LOG_TRIVIAL(debug) << "LogicalDevice:: calling getter getLogicalDeviceHandle() ";  
                BOOST_LOG_TRIVIAL(debug) << "\t LogicalDevice:: my VkDevice is " << logicalDevice;    
                return logicalDevice;            
            }

            VkDevice& getLogicalDeviceHandle() { 
                return const_cast<VkDevice&>(const_cast<const LogicalDevice*>(this)->getLogicalDeviceHandle());
            };

        private:

            VkDevice logicalDevice;
            void createLogicalDevice(const VkSurfaceKHR& surface_, const PhysicalDevice& PhysicalDevice, 
                                    QueueManager& queueManager, const DebugMessenger& dm);
    };
}

#endif