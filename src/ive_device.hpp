#pragma once

#include "ive_window.hpp"

#include <string>
#include <vector>

#include "command.hpp"
#include "queue.hpp"
#include "buffer.hpp"
#include "device.hpp"
#include "validation.hpp"

namespace ive {

  class iveDevice {

    public:

      #ifdef NDEBUG
        const bool enableValidationLayers = false;
      #else
        const bool enableValidationLayers = true;
      #endif

      iveDevice(iveWindow &window);
      ~iveDevice();

      // Disable copy and move
      iveDevice(const iveDevice &) = delete;
      void operator=(const iveDevice &) = delete;
      iveDevice(iveDevice &&) = delete;
      iveDevice &operator=(iveDevice &&) = delete;

      VkCommandPool getCommandPool() { return commandPool.getVkCPool(); }

    private:

      VkInstance createInstance();
      void setupDebugMessenger();

      // TODO: delete
      void createCommandPool();
      void createBufferManager();

      // helper functions
      std::vector<const char *> getGlfwRequiredExtensions();
    //  QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
      void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &createInfo);
      void hasGflwRequiredInstanceExtensions();

      iveWindow &window;
      ValidationLayers validationLayers;
      VkInstance instance;
      // Todo: remove surface object and acccess through a member of window
      VkSurfaceKHR &surface;
      VkDebugUtilsMessengerEXT debugMessenger;
      LogicalDevice logicalDevice;
      CommandPool commandPool;
      BufferManager bufferManager;

      // TODO: what is?
      VkBool32 presentSupport = false;
};

}