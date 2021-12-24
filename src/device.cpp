#include "device.hpp"
#include "validation.hpp"

namespace ive {

    LogicalDevice::LogicalDevice(VkInstance instance_, const VkSurfaceKHR &surface_) 
                            :   queueManager((pickPhysicalDevice(instance_, surface_), physicalDevice), surface_) 
                            {
        createLogicalDevice(surface_);
    }
    LogicalDevice::~LogicalDevice() {
        vkDestroyDevice(logicalDevice, nullptr);
    }

    // Create a logical device to do the rendering to
    //
    // TODO: why logical device? physical device is not physical
    // \__#.#__/
    void LogicalDevice::createLogicalDevice(VkSurfaceKHR surface_) {
        // TODO: del
    //    queueManager.findQueueFamilies(physicalDevice, surface_);

        // TODO: why multiple queueCreateInfos?
        // Make multiple queues?
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = {queueManager.getQueueFamilyIndices().graphicsFamily, 
                                                queueManager.getQueueFamilyIndices().presentFamily};

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
        if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &logicalDevice) != VK_SUCCESS) {
            throw std::runtime_error("failed to create logical device!");
        }

        // Now get the queue handle from the device
        // TODO: delegate to QueueManager
        vkGetDeviceQueue(logicalDevice, queueManager.getQueueFamilyIndices().graphicsFamily, 0, queueManager.getGraphicsQueuePtr());
        vkGetDeviceQueue(logicalDevice, queueManager.getQueueFamilyIndices().presentFamily, 0, queueManager.getPresentQueuePtr());
    }


    // Pick a GPU! Sometimes more than one is available.
    //
    // Currently this function picks the first available.
    void LogicalDevice::pickPhysicalDevice(VkInstance instance, VkSurfaceKHR surface_) {
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
        if (deviceCount == 0) {
            throw std::runtime_error("failed to find GPUs with Vulkan support!");
        }
        std::cout << "Device count: " << deviceCount << std::endl;
        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

        // Just pick the first
        for (const auto &device : devices) {
            if (isDeviceSuitable(device, surface_)) {
                physicalDevice = device;
                break;
            }
        }

        if (physicalDevice == VK_NULL_HANDLE) {
            throw std::runtime_error("failed to find a suitable GPU!");
        }

        // TODO: where is properties?
        vkGetPhysicalDeviceProperties(physicalDevice, &properties);
        std::cout << "physical device: " << properties.deviceName << std::endl;
    }

    // Check the physical device chosen has support for extensions
    bool LogicalDevice::checkDeviceExtensionSupport(VkPhysicalDevice device) {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(
            device,
            nullptr,
            &extensionCount,
            availableExtensions.data());

        std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

        for (const auto &extension : availableExtensions) {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }



    // Check if device is suitable
    //
    // Perform a few checks to make sure required extensions/features are supported
    //
    // Args: 
    //    device (VkPhysicalDevice): the device we check
    // Returns:
    //    boolean whether device is suitable
    bool LogicalDevice::isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface_) {
    // Firstly get the available queue families for a device
        QueueFamilyIndices indices = queueManager.getQueueFamilyIndices();

        bool extensionsSupported = checkDeviceExtensionSupport(device);

        bool swapChainAdequate = false;
        if (extensionsSupported) {

            SwapChainSupportDetails swapChainSupport = querySwapChainSupport(surface_);
            
            swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        }

        VkPhysicalDeviceFeatures supportedFeatures;
        vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

        return indices.isComplete() && extensionsSupported && swapChainAdequate &&
                supportedFeatures.samplerAnisotropy;
    }   


    // NB the swap chain is not actually supported by default; it is highly tied to
    // the windowing system
    SwapChainSupportDetails LogicalDevice::querySwapChainSupport(VkSurfaceKHR surface) {

        SwapChainSupportDetails details;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &details.capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);

        if (formatCount != 0) {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, details.formats.data());
        }

        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);

        if (presentModeCount != 0) {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(
                physicalDevice,                     
                surface,
                &presentModeCount,
                details.presentModes.data());
        }
        return details;
    }

    SwapChainSupportDetails LogicalDevice::getSwapChainSupport(VkSurfaceKHR surface) { 
        return querySwapChainSupport(surface); 
    }


}