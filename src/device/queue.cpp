#include "queue.hpp"
#include <boost/log/trivial.hpp>

namespace ive {

    QueueManager::QueueManager(const VkPhysicalDevice& dev, const VkSurfaceKHR& surf) {
        BOOST_LOG_TRIVIAL(debug) << "QueueManager::constructor called";
        BOOST_LOG_TRIVIAL(debug) << "QueueManager::finding queue families with device " << dev << " and surface " << surf;
        queueFamilyIndices = findQueueFamilies(dev, surf);
    }


    QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice& device, const VkSurfaceKHR& surface) {
        QueueFamilyIndices indices;

        // Firstly get the number of queue families
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        // Now get the queue families data itself
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        // Now iterate through the available queue families until completion
        int i = 0;
        for (const auto &queueFamily : queueFamilies) {
            if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphicsFamily = i;
                indices.graphicsFamilyHasValue = true;
            }
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
            if (queueFamily.queueCount > 0 && presentSupport) {
                indices.presentFamily = i;
                indices.presentFamilyHasValue = true;
            }
            if (indices.isComplete()) {
                break;
            }

            i++;
        }

        return indices;

    }

    void QueueManager::createQueues(const VkDevice& logicalDevice) {
        vkGetDeviceQueue(logicalDevice, getGraphicsFamily(), 0, &graphicsQueue_);
        vkGetDeviceQueue(logicalDevice, getPresentFamily(), 0, &presentQueue_);
    }

    QueueFamilyIndices& QueueManager::getQueueFamilyIndices() { return queueFamilyIndices; }

    const VkQueue& QueueManager::getGraphicsQueue() const { return graphicsQueue_; }
    const VkQueue& QueueManager::getPresentQueue() const { return presentQueue_; }

    const uint32_t& QueueManager::getGraphicsFamily() const { return queueFamilyIndices.graphicsFamily; }
    const uint32_t& QueueManager::getPresentFamily() const { return queueFamilyIndices.presentFamily; }

    VkQueue* QueueManager::getGraphicsQueuePtr() { return &graphicsQueue_; }
    VkQueue* QueueManager::getPresentQueuePtr() { return &presentQueue_; }


    QueueFamilyIndices QueueManager::findPhysicalQueueFamilies(const VkPhysicalDevice& device,
                                                    const VkSurfaceKHR& surface) {
        return findQueueFamilies(device, surface);
    }
            
}