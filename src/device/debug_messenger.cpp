#include <vulkan/vulkan.h>
#include <boost/log/trivial.hpp>

#include "validation.hpp"
#include "debug_messenger.hpp"

namespace ive {

DebugMessenger::DebugMessenger(const VkInstance &instance) {
    setupDebugMessenger(instance);
}

DebugMessenger::~DebugMessenger() {
    // TODO: urgently fix, RAII!
}

        //  Create a a callback function for debug messages
        //
        //  Args:
        //    instance: application state object
        //    pCreateInfo *: a ptr to a const info struct
        //    pAllocator *: ptr to stuct containing mem allocation callback function pointers
        //    pDebugMessenger *: a ptr to a debug messenger
        //  
        //  Returns:
        //    
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

    BOOST_LOG_TRIVIAL(debug) << "setupDebugMessenger instance:" << instance;

    if (!enableValidationLayers) return;
        VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(createInfo);
    if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
        throw std::runtime_error("failed to set up debug messenger!");
    }
    BOOST_LOG_TRIVIAL(debug) << "Successfully called CreateDebugUtilsMessengerEXT ";
}


}