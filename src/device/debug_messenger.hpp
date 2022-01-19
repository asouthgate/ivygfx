#ifndef DEBUG_MESSENGER_HPP
#define DEBUG_MESSENGER_HPP

#include <vulkan/vulkan.h>

#include "instance.hpp"

namespace ivy {

    #ifdef NDEBUG
        const bool enableValidationLayers = false;
    #else
        const bool enableValidationLayers = true;
    #endif

    class DebugMessenger {
        
        public:
            DebugMessenger(Instance &instance) ;
            ~DebugMessenger();
        
            static bool checkValidationLayerSupport();

            static const std::vector<const char *>& getValidationLayers();

            static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);

            static int n_messages;

        private:

            VkDebugUtilsMessengerEXT debugMessenger;
            Instance& instance;

            static const std::vector<const char *> validationLayers;


            static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            // NB: all of these flags are just typedef for VkFlags
                                            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                            VkDebugUtilsMessageTypeFlagsEXT messageType,
                                            const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                            void *pUserData);



            VkResult CreateDebugUtilsMessengerEXT(
                const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                const VkAllocationCallbacks *pAllocator,
                VkDebugUtilsMessengerEXT *pDebugMessenger) ;

            // Destroy the debug utils messenger
            void DestroyDebugUtilsMessengerEXT(
                VkDebugUtilsMessengerEXT debugMessenger,
                const VkAllocationCallbacks *pAllocator);
            
            // TODO: extract out into debug module
            void setupDebugMessenger() ;
    };

}

#endif