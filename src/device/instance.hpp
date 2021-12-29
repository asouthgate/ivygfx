#ifndef INSTANCE_HPP
#define INSTANCE_HPP

#include <vulkan/vulkan.h>
#include <stdexcept>
#include <boost/log/trivial.hpp>

#include "ive_window.hpp"
#include "debug_messenger.hpp"

namespace ive {
  
    // Create Vulkan instance
    //
    // Generally this code follows the convention of populating structs that specify options
    // Don't panic with these! Check the Vulkan docs.
    VkInstance createInstance(VkInstance& vkinstance) {


        BOOST_LOG_TRIVIAL(debug) << "createInstance()::createInstance instance start:" << vkinstance;

        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Ivy Engine App";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo instanceCreateInfo = {};
        instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCreateInfo.pApplicationInfo = &appInfo;
        auto extensions = iveWindow::getGlfwRequiredExtensions();
        instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        instanceCreateInfo.ppEnabledExtensionNames = extensions.data();

        // Optional: debug/validation info
        if (enableValidationLayers) {
            // DebugMessenger& dm = DebugMessenger::get_instance(vkinstance);

            BOOST_LOG_TRIVIAL(debug) << "instance::createInstance::Enabling validation layers";

            if (!DebugMessenger::checkValidationLayerSupport()) {
                throw std::runtime_error("validation layers requested, but not available!");
            }

            instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(DebugMessenger::validationLayers.size());
            instanceCreateInfo.ppEnabledLayerNames = DebugMessenger::validationLayers.data();
            VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
            DebugMessenger::populateDebugMessengerCreateInfo(debugCreateInfo);
            instanceCreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;

        } 
        else {
            BOOST_LOG_TRIVIAL(debug) << "instance::createInstance::NOT enabling validation layers";
            instanceCreateInfo.enabledLayerCount = 0;
            instanceCreateInfo.pNext = nullptr;
        }


        if (vkCreateInstance(&instanceCreateInfo, nullptr, &vkinstance) != VK_SUCCESS) {
            BOOST_LOG_TRIVIAL(debug) << "createInstance()::failed to create an instance:" << vkinstance;
            throw std::runtime_error("failed to create instance!");
        }

        BOOST_LOG_TRIVIAL(debug) << "createInstance instance end:" << vkinstance;


        iveWindow::hasGflwRequiredInstanceExtensions();

        // Do a check that the required extensions are supported

        BOOST_LOG_TRIVIAL(debug) << "createInstance()::createInstance instance end:" << vkinstance;
        return vkinstance;
    }

}

#endif