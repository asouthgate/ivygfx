#include <boost/log/trivial.hpp>
#include <iostream>
#include <set>

#include "logical_device.hpp"
#include "debug_messenger.hpp"
#include "physical_device.hpp"
#include "queue.hpp"

namespace ivy {

    LogicalDevice::LogicalDevice(const VkSurfaceKHR &surface_, 
                                const PhysicalDevice& PhysicalDevice,
                                QueueManager& queueManager,
                                const DebugMessenger& dm) {
        BOOST_LOG_TRIVIAL(debug) << "LogicalDevice::starting constructor...";
        BOOST_LOG_TRIVIAL(debug) << "LogicalDevice::logicalDevice at "  << logicalDevice;
        createLogicalDevice(PhysicalDevice, queueManager, dm);
        BOOST_LOG_TRIVIAL(debug) << "LogicalDevice::logicalDevice at "  << logicalDevice;
        BOOST_LOG_TRIVIAL(debug) << "LogicalDevice::logicalDevice retrieved by getter is  "  << getLogicalDeviceHandle();
    }

    LogicalDevice::~LogicalDevice() {
        vkDestroyDevice(logicalDevice, nullptr);
    }

    const VkDevice& LogicalDevice::getLogicalDeviceHandle() const {
        BOOST_LOG_TRIVIAL(debug) << "LogicalDevice:: calling getter getLogicalDeviceHandle() ";  
        BOOST_LOG_TRIVIAL(debug) << "\t LogicalDevice:: my VkDevice is " << logicalDevice;    
        return logicalDevice;            
    }

    VkDevice& LogicalDevice::getLogicalDeviceHandle() { 
        return const_cast<VkDevice&>(const_cast<const LogicalDevice*>(this)->getLogicalDeviceHandle());
    }

    // Create a logical device to do the rendering to
    // \__#.#__/
    void LogicalDevice::createLogicalDevice(const PhysicalDevice& physicalDevice,
                                            QueueManager& queueManager,
                                            const DebugMessenger& dm) {

        const VkPhysicalDevice& physicalDeviceVk = physicalDevice.getVkPhysicalDeviceHandle();

        uint32_t gFam = queueManager.getGraphicsFamily();
        uint32_t pFam = queueManager.getPresentFamily();

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = {gFam, pFam};

        // TODO: extract some of this out and give to the QueueManager
        // For each unique queueFamily, populate some stuff
        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo = {};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        // Add some device features, which ones are allowed? Check the docs!
        // \__#.#__/
        VkPhysicalDeviceFeatures deviceFeatures = {};
        deviceFeatures.samplerAnisotropy = VK_TRUE;


        const std::vector<const char *> &deviceExtensions = physicalDevice.getDeviceExtensions();
        BOOST_LOG_TRIVIAL(debug) << "LogicalDevice::logicalDevice retrieved extensions:";
        for (auto & a : deviceExtensions) {
            std::cerr << "\tLogicalDevice::logicalDevice extension to be loaded:"  << a << std::endl;
        }
        // Get some params
        // \__#.#__/
        VkDeviceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();

        // TODO: check depreciated? No device specific layers? Then remove
        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(DebugMessenger::getValidationLayers().size());
            createInfo.ppEnabledLayerNames = DebugMessenger::getValidationLayers().data();
        } else {
            createInfo.enabledLayerCount = 0;
        }

        BOOST_LOG_TRIVIAL(debug) << "LogicalDevice::creating a logical device with physical device" << physicalDeviceVk;

        // Finally if we did not succeed, boom! 
        if (vkCreateDevice(physicalDeviceVk, &createInfo, nullptr, &logicalDevice) != VK_SUCCESS) {
            throw std::runtime_error("failed to create logical device!");
        }

        queueManager.createQueues(logicalDevice);
    }

}
