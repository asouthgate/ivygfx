#ifndef DEBUG_MESSENGER_HPP
#define DEBUG_MESSENGER_HPP

#include <vulkan/vulkan.h>
#include <boost/log/trivial.hpp>

#include "validation.hpp"

namespace ive {

class DebugMessenger {
    public:
        DebugMessenger(const VkInstance &instance) ;
        ~DebugMessenger();

        VkDebugUtilsMessengerEXT debugMessenger;

        VkResult CreateDebugUtilsMessengerEXT(
            VkInstance instance,
            const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
            const VkAllocationCallbacks *pAllocator,
            VkDebugUtilsMessengerEXT *pDebugMessenger) ;

        // Destroy the debug utils messenger
        void DestroyDebugUtilsMessengerEXT(
            VkInstance instance,
            VkDebugUtilsMessengerEXT debugMessenger,
            const VkAllocationCallbacks *pAllocator) ;
        
        // TODO: extract out into debug module
        void setupDebugMessenger(const VkInstance &instance) ;

};

}

#endif