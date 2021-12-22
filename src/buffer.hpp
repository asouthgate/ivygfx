#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <vulkan/vulkan.h>
#include <vector>

#include "command.hpp"

// TODO: move to cpp file

// This class is not responsible for checking if it is supported
// Need a separate checker for that

namespace ive {

    class BufferManager {

        public:
            // move to cpp
            BufferManager();
            BufferManager(VkDevice VkDevice_, CommandPool* pcommandPool_) : device(VkDevice_), 
                pcommandPool(pcommandPool_) {};

        private:

            uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
      
            VkFormat findSupportedFormat(
                const std::vector<VkFormat> &candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

            // Buffer Helper Functions
            void createBuffer(
                VkDeviceSize size,
                VkBufferUsageFlags usage,
                VkMemoryPropertyFlags properties,
                VkBuffer &buffer,
                VkDeviceMemory &bufferMemory);

            void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
            
            void copyBufferToImage(
                VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount);

            void createImageWithInfo(
                const VkImageCreateInfo &imageInfo,
                VkMemoryPropertyFlags properties,
                VkImage &image,
                VkDeviceMemory &imageMemory);

            // Don't forget, this is already a pointer
            VkDevice device;
            CommandPool* pcommandPool; 
    };

}

#endif