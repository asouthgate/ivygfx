#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <boost/log/trivial.hpp>

#include <vulkan/vulkan.h>
#include <vector>
#include <string.h>
#include <iostream>
#include <unordered_set>
#include <string>

namespace ive {

    #ifdef NDEBUG
        const bool enableValidationLayers = false;
    #else
        const bool enableValidationLayers = true;
    #endif

    const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};

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
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        // NB: all of these flags are just typedef for VkFlags
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
        void *pUserData) {
        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
        return VK_FALSE;
    }

    // Get required extensions for glfw
    //
    // Returns:
    //    std::vector<const char*> extensions: vector of C string extension names
    std::vector<const char *> getGlfwRequiredExtensions() {
        uint32_t glfwExtensionCount = 0;
        const char **glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        BOOST_LOG_TRIVIAL(debug) << "glfwExtensionCount is " << glfwExtensionCount << std::endl;

        for (int i = 0; i < glfwExtensionCount; ++i) {
            BOOST_LOG_TRIVIAL(debug) << glfwExtensions[i];            
        }

        std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);


        if (enableValidationLayers) {
            BOOST_LOG_TRIVIAL(debug) << "Adding VK_EXT_DEBUG_UTILS_EXTENSION_NAME layer";
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        for (const auto &extension : extensions) {
            BOOST_LOG_TRIVIAL(debug) << "glfw required extension:" << extension << std::endl;
        }

        return extensions;
    }

      // TODO: rename
    // This function throws an error if GLFW has not gotten its required extensions
    void hasGflwRequiredInstanceExtensions() {
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

        std::cout << "available extensions:" << std::endl;
        std::unordered_set<std::string> available;
        for (const auto &extension : extensions) {
            std::cout << "\t" << extension.extensionName << std::endl;
            available.insert(extension.extensionName);
        }

        std::cout << "required extensions:" << std::endl;
        auto requiredExtensions = getGlfwRequiredExtensions();
        for (const auto &required : requiredExtensions) {
            std::cout << "\t" << required << std::endl;
            if (available.find(required) == available.end()) {
                throw std::runtime_error("Missing required glfw extension");
            }   
        }
    }


    //TODO: abstract away the C stuff
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo) {
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

    bool checkValidationLayerSupport() {
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


}
