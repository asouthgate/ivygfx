#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <cstdint>
#include <vulkan/vulkan.h>

#include <boost/log/trivial.hpp>


// TODO: this code is smelly; hard to test

// TODO: what is presentFamily?

struct QueueFamilyIndices {
    uint32_t graphicsFamily;
    uint32_t presentFamily;
    bool graphicsFamilyHasValue = false;
    bool presentFamilyHasValue = false;
    bool isComplete() { return graphicsFamilyHasValue && presentFamilyHasValue; }
};

//TODO: move into cpp file

namespace ivy {

    QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice& device, const VkSurfaceKHR& surface);

    class QueueManager {
        public:
            QueueManager(const VkPhysicalDevice& dev, const VkSurfaceKHR& surf);
            ~QueueManager() {}

            void createQueues(const VkDevice& logicalDevice);

            QueueFamilyIndices& getQueueFamilyIndices();

            const VkQueue& getGraphicsQueue() const;
            const VkQueue& getPresentQueue() const;

            const uint32_t& getGraphicsFamily() const;
            const uint32_t& getPresentFamily() const;

            VkQueue* getGraphicsQueuePtr();
            VkQueue* getPresentQueuePtr();

        private:
            VkQueue graphicsQueue_;
            VkQueue presentQueue_;

            // TODO: add caching for this so doesnt have to run so often
            QueueFamilyIndices queueFamilyIndices;



            QueueFamilyIndices findPhysicalQueueFamilies(const VkPhysicalDevice& device,
                                                         const VkSurfaceKHR& surface);
    };
}

#endif