#include <vulkan/vulkan.h>
#include <boost/log/trivial.hpp>
#include <vector>
#include <iostream>
#include <set>

#include "physical_device.hpp"
#include "queue.hpp"
#include "swapchain.hpp"


namespace ive {

    PhysicalDevice::PhysicalDevice(const VkInstance& instance, const VkSurfaceKHR& surface_) {
        BOOST_LOG_TRIVIAL(debug) << "PhysicalDevice::starting constructor...";
        pickPhysicalDevice(instance, surface_);

    }

    bool PhysicalDevice::checkDeviceExtensionSupport(const VkPhysicalDevice& physicalDevice,
                                    const std::vector<const char *>& requiredExtensions) {
        BOOST_LOG_TRIVIAL(debug) << "checking device ext support" << physicalDevice  << std::endl;
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);
        BOOST_LOG_TRIVIAL(debug) << extensionCount << "extensions"  << std::endl;

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(
            physicalDevice,
            nullptr,
            &extensionCount,
            availableExtensions.data());
        BOOST_LOG_TRIVIAL(debug) << "got available ext"  << std::endl;

        // for (VkExtensionProperties& a : availableExtensions) { 
        //     BOOST_LOG_TRIVIAL(debug) << "available ext:" << a.extensionName << std::endl;
        // }

        for (auto& a : requiredExtensions) { 
            BOOST_LOG_TRIVIAL(debug) << "required device ext:" << a << std::endl;
        }
        std::set<std::string> requiredExtensionsSet(requiredExtensions.begin(), requiredExtensions.end());
        BOOST_LOG_TRIVIAL(debug) << "got required ext"  << std::endl;

        for (const auto &extension : availableExtensions) {
            // looks like you can erase from a set even if not there
            requiredExtensionsSet.erase(extension.extensionName);
        }

        BOOST_LOG_TRIVIAL(debug) << "Finished checking required and available extensions";

        for (const auto& extension: requiredExtensionsSet) {
            BOOST_LOG_TRIVIAL(debug) << "Warning: extension remaining not available:" << extension;

        }
        return requiredExtensionsSet.empty();
    }




// Check if device is suitable
    //
    // Perform a few checks to make sure required extensions/features are supported
    //
    // Args: 
    //    device (VkPhysicalDevice): the device we check
    // Returns:
    //    boolean whether device is suitable
    bool PhysicalDevice::isDeviceSuitable(const VkPhysicalDevice& device, VkSurfaceKHR surface_) {
        BOOST_LOG_TRIVIAL(debug) << "checking device suitability" << device  << std::endl;
    // Firstly get the available queue families for a device
        QueueFamilyIndices indices = findQueueFamilies(device, surface_);

        bool extensionsSupported = checkDeviceExtensionSupport(device, deviceExtensions);

        BOOST_LOG_TRIVIAL(debug) << "Are extensions supported? " << extensionsSupported  << std::endl;

        bool swapChainAdequate = false;
        if (extensionsSupported) {

            SwapChainSupportDetails swapChainSupport = SwapChain::querySwapChainSupport(device, surface_);
            
            swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        }

        VkPhysicalDeviceFeatures supportedFeatures;
        vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

        return indices.isComplete() && extensionsSupported && swapChainAdequate &&
                supportedFeatures.samplerAnisotropy;
    }   



    // Pick a GPU! Sometimes more than one is available.
    //
    // Currently this function picks the first available.
    void PhysicalDevice::pickPhysicalDevice(const VkInstance& instance, const VkSurfaceKHR& surface_) {

        BOOST_LOG_TRIVIAL(debug) << "calling pickPhysicalDevice"  << std::endl;
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
                BOOST_LOG_TRIVIAL(debug) << "picking device" << device  << std::endl;
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

}