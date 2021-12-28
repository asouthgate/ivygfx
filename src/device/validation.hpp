#ifndef VALIDATION_HPP
#define VALIDATION_HPP

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

    const std::vector<const char *> validationLayers;
   
    static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
        // NB: all of these flags are just typedef for VkFlags
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
        void *pUserData);

    // Get required extensions for glfw
    //
    // Returns:
    //    std::vector<const char*> extensions: vector of C string extension names
    std::vector<const char *> getGlfwRequiredExtensions();

      // TODO: rename
    // This function throws an error if GLFW has not gotten its required extensions
    void hasGflwRequiredInstanceExtensions();


    //TODO: abstract away the C stuff
    void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);

    bool checkValidationLayerSupport();


}

#endif