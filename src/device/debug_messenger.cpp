#include <vulkan/vulkan.h>
#include <boost/log/trivial.hpp>
#include <iostream>
#include <stdexcept>

#include "debug_messenger.hpp"

namespace ive {

    bool DebugMessenger::instance_exists = false;

    DebugMessenger& DebugMessenger::get_instance(const VkInstance &instance) {
        static DebugMessenger s(instance);
        return s;
    }

    const std::vector<const char *> DebugMessenger::validationLayers = {"VK_LAYER_KHRONOS_validation"};
    int DebugMessenger::n_messages = 0;
    
    //  A callback function used in debugging
    //
    //  Currently this function just outputs a debug message to stderr
    //
    //  Args:
    //    messageSeverity: a severity bit flag
    //    messageType: a message type bit flag
    //    pCallbackData: a pointer to 'callback data'
    //  Returns:
    //    VK_FALSE: an unsigned zero
    VKAPI_ATTR VkBool32 VKAPI_CALL DebugMessenger::debugCallback(
        // NB: all of these flags are just typedef for VkFlags
                VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                VkDebugUtilsMessageTypeFlagsEXT messageType,
                const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                void *pUserData) {
        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
        BOOST_LOG_TRIVIAL(debug) << "DebugMessenger::debugCallback::validation layer: " << pCallbackData->pMessage;
        return VK_FALSE;
    }

        //TODO: abstract away the C stuff
    void DebugMessenger::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo) {
        createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = debugCallback;
        createInfo.pUserData = nullptr;  // Optional
    }

    bool DebugMessenger::checkValidationLayerSupport() {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char *layerName : validationLayers) {
            bool layerFound = false;
            // linear scan through the available layers
            for (const auto &layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) {
                return false;
            }
        }

        return true;
    }

    DebugMessenger::DebugMessenger(const VkInstance &instance) {
        if (instance_exists) {
            throw std::runtime_error("Only one instance of DebugMessenger allowed. Use DebugMessenger::get_instance() to get a handle.");
        }
        setupDebugMessenger(instance);
        instance_exists = true;
    }

    DebugMessenger::~DebugMessenger() {
        // TODO: urgently fix, RAII!
    }

    // Get a handle to vkCreateDebugUtilsMessengerEXT, which is not loaded automaically    
    VkResult DebugMessenger::CreateDebugUtilsMessengerEXT(
                                VkInstance instance,
                                const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                const VkAllocationCallbacks *pAllocator,
                                VkDebugUtilsMessengerEXT *pDebugMessenger) {
        // Get a function pointer to vkCreateDebugUtilsMessengerEXT
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(
            instance,
            "vkCreateDebugUtilsMessengerEXT");
        // If we succeed, do the creation and return, otherwise give an error code
        if (func != nullptr) {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        } else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    // Destroy the debug utils messenger
    void DebugMessenger::DestroyDebugUtilsMessengerEXT(
        VkInstance instance,
        VkDebugUtilsMessengerEXT debugMessenger,
        const VkAllocationCallbacks *pAllocator) {
        // Get function pointer for destructor
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(
            instance,
            "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr) {
            func(instance, debugMessenger, pAllocator);
        }
    }

    // TODO: extract out into debug module
    void DebugMessenger::setupDebugMessenger(const VkInstance &instance) {

        BOOST_LOG_TRIVIAL(debug) << "DebugMessenger::setupDebugMessenger::setupDebugMessenger instance:" << instance;

        if (!enableValidationLayers) {
            BOOST_LOG_TRIVIAL(debug) << "DebugMessenger::setupDebugMessenger::setupDebugMessenger not happening, validation layers are disabled";
            return;
        }

        BOOST_LOG_TRIVIAL(debug) << "DebugMessenger::setupDebugMessenger::setupDebugMessenger going ahead. Validation layers are enabled";

        VkDebugUtilsMessengerCreateInfoEXT createInfo;
        populateDebugMessengerCreateInfo(createInfo);
        if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
            throw std::runtime_error("failed to set up debug messenger!");
        }
        BOOST_LOG_TRIVIAL(debug) << "DebugMessenger::setupDebugMessenger Successfully called CreateDebugUtilsMessengerEXT ";
    }

    const std::vector<const char *>& DebugMessenger::getValidationLayers() { return validationLayers; }
}