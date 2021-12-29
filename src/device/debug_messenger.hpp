#ifndef DEBUG_MESSENGER_HPP
#define DEBUG_MESSENGER_HPP

#include <vulkan/vulkan.h>
#include <boost/log/trivial.hpp>

//SINGLETON PTTERN; MEYERS SINGLETON

namespace ive {

    #ifdef NDEBUG
        const bool enableValidationLayers = false;
    #else
        const bool enableValidationLayers = true;
    #endif

    class DebugMessenger {
        
        public:
            DebugMessenger(VkInstance &instance) ;
            ~DebugMessenger();
        
            static bool checkValidationLayerSupport();

            static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
            // NB: all of these flags are just typedef for VkFlags
                                            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                            VkDebugUtilsMessageTypeFlagsEXT messageType,
                                            const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                            void *pUserData);


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

            void destroy();

            static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);

            static const std::vector<const char *>& getValidationLayers();

            static DebugMessenger& get_instance(VkInstance &instance);

            static const std::vector<const char *> validationLayers;

            static int n_messages;

        private:

//TODO: make these const; a real singleton
            VkDebugUtilsMessengerEXT debugMessenger;

            static VkInstance* pprev_instance;

            static bool instance_exists;
    };

}

#endif