#include "ive_device.hpp"
#include "swapchain_chk.hpp"
#include "command.hpp"
#include "buffer.hpp"

#include <cstring>
#include <iostream>
#include <set>
#include <unordered_set>

namespace ive {



  // TODO: temporary comma operator; put into containers?
  iveDevice::iveDevice(iveWindow &window_) : 
                                            instance{createInstance()}, 
                                            window{window_},
                                            surface{window.createWindowSurface(instance)},
                                            logicalDevice{instance, surface, validationLayers},
                                            commandPool{logicalDevice},
                                            bufferManager{} {
    setupDebugMessenger();
  }



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

    // NB: VK_FALSE is 0U
    return VK_FALSE;
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
  VkResult CreateDebugUtilsMessengerEXT(
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
  void DestroyDebugUtilsMessengerEXT(
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
 

  void iveDevice::createBufferManager() {
      // TODO: move qfi inside CommandPool?
      bufferManager = BufferManager(logicalDevice.getLogicalDeviceHandle(), &commandPool);
  }

  iveDevice::~iveDevice() {
    // We must destroy that which we created
    // TODO: ensure is delegated to Command
  //  vkDestroyDevice(device_, nullptr);
    vkDestroySurfaceKHR(instance, surface, nullptr);

    if (enableValidationLayers) {
      DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
    }

  //  vkDestroySurfaceKHR(instance, surface_, nullptr);
    vkDestroyInstance(instance, nullptr);
  }

  // Create Vulkan instance
  //
  // Generally this code follows the convention of populating structs that specify options
  // Don't panic with these! Check the Vulkan docs.
  // We create 3 structs: VkApplicationInfo appInfo, 
  // VkInstanceCreateInfo createInfo, VkInstanceCreateInfo debugCreateInfo
  VkInstance iveDevice::createInstance() {

    // Quickly check if validation layers are actually supported
    // By default in Vulkan, error checking etc is actually turned off
    if (enableValidationLayers && !validationLayers.checkValidationLayerSupport()) {
      throw std::runtime_error("validation layers requested, but not available!");
    }

    // When we create application info we must also supply sType
    // This is the struccture type for the info
    // A lot of these conventions from C
    // In general for these, look at vulkan spec for the struct
    VkApplicationInfo appInfo = {};
    // TODO: find out why isnt this default?
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Ivy Engine App";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    // Just needs to be uint32_t, dont know why VK_MAKE_VERSION Is necessary
    // TODO: simplify?
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    // Parameters for newly created instance
    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    auto extensions = getGlfwRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();

    // Next specify params for utils messenger create info
    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
    if (enableValidationLayers) {
      createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.getVector().size());
      createInfo.ppEnabledLayerNames = validationLayers.getVector().data();

      populateDebugMessengerCreateInfo(debugCreateInfo);
      createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
    } else {
      createInfo.enabledLayerCount = 0;
      createInfo.pNext = nullptr;
    }

    VkInstance vkinstance;

    if (vkCreateInstance(&createInfo, nullptr, &vkinstance) != VK_SUCCESS) {
      throw std::runtime_error("failed to create instance!");
    }

    // Do a check that the required extensions are supported
    hasGflwRequiredInstanceExtensions();

    return vkinstance;
  }

  //TODO: abstract away the C stuff
  void iveDevice::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo) {
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

  // TODO: extract out into debug module
  void iveDevice::setupDebugMessenger() {
    if (!enableValidationLayers) return;
    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(createInfo);
    if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS) {
      throw std::runtime_error("failed to set up debug messenger!");
    }
  }

  // Get required extensions for glfw
  //
  // Returns:
  //    std::vector<const char*> extensions: vector of C string extension names
  std::vector<const char *> iveDevice::getGlfwRequiredExtensions() {
    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

    if (enableValidationLayers) {
      extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    return extensions;
  }

  // TODO: rename
  // This function throws an error if GLFW has not gotten its required extensions
  void iveDevice::hasGflwRequiredInstanceExtensions() {
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

}  // namespace