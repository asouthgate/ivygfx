#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <cstdint>
#include <vector>
#include <vulkan/vulkan.h>

// TODO: what is presentFamily?

struct QueueFamilyIndices {
  uint32_t graphicsFamily;
  uint32_t presentFamily;
  bool graphicsFamilyHasValue = false;
  bool presentFamilyHasValue = false;
  bool isComplete() { return graphicsFamilyHasValue && presentFamilyHasValue; }
};

//TODO: move into cpp file

namespace ive {
    class QueueManager {
        public:
            QueueManager(const VkPhysicalDevice& dev, const VkSurfaceKHR& surf) {
                findQueueFamilies(dev, surf);
            }
            ~QueueManager() {}

            QueueFamilyIndices& getQueueFamilyIndices() { return queueFamilyIndices; }

            VkQueue& getGraphicsQueue() { return graphicsQueue_; }
            VkQueue& getPresentQueue() { return presentQueue_; }

            VkQueue* getGraphicsQueuePtr() { return &graphicsQueue_; }
            VkQueue* getPresentQueuePtr() { return &presentQueue_; }

        private:
            VkQueue graphicsQueue_;
            VkQueue presentQueue_;

            // TODO: add caching for this so doesnt have to run so often
            QueueFamilyIndices queueFamilyIndices;

                        QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice& device, 
                                                 const VkSurfaceKHR& surface){
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

            QueueFamilyIndices findPhysicalQueueFamilies(const VkPhysicalDevice& device,
                                                         const VkSurfaceKHR& surface) {
                return findQueueFamilies(device, surface);
            }
    };
}

#endif