#include "logical_device.hpp"
#include "validation.hpp"
#include "physical_device.hpp"
#include "queue.hpp"

#include <boost/log/trivial.hpp>

namespace ive {

    LogicalDevice::LogicalDevice(const VkSurfaceKHR &surface_, 
                                const PhysicalDevice& PhysicalDevice,
                                QueueManager& queueManager) {
        BOOST_LOG_TRIVIAL(debug) << "calling createLogicalDevice"  << std::endl;
        createLogicalDevice(surface_, PhysicalDevice, queueManager);
    }

    LogicalDevice::~LogicalDevice() {
        vkDestroyDevice(logicalDevice, nullptr);
    }

    // Create a logical device to do the rendering to
    // \__#.#__/
    void LogicalDevice::createLogicalDevice(const VkSurfaceKHR& surface_, 
                                            const PhysicalDevice& physicalDevice,
                                            QueueManager& queueManager) {

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
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();
        } else {
            createInfo.enabledLayerCount = 0;
        }

        // Finally if we did not succeed, boom! 
        if (vkCreateDevice(physicalDeviceVk, &createInfo, nullptr, &logicalDevice) != VK_SUCCESS) {
            throw std::runtime_error("failed to create logical device!");
        }

        queueManager.createQueues(logicalDevice);
    }

}