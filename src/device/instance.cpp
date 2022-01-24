#include <boost/log/trivial.hpp>

#include "instance.hpp" 
#include "window.hpp"
#include "debug_messenger.hpp"

namespace ivy {
    Instance::Instance() {
        createInstance();
    }

    Instance::~Instance() {
        BOOST_LOG_TRIVIAL(debug) << "Instance()::destroying instance:" << vkinstance;
        vkDestroyInstance(vkinstance, nullptr);
    }

    VkInstance& Instance::getVkInstanceHandle() {
        return vkinstance;
    }

    /* Setup the instance member variable */
    void Instance::createInstance() {

        // Firstly specify the appliction info, this is fairly self-explanatory

        BOOST_LOG_TRIVIAL(debug) << "createInstance()::createInstance instance start:" << vkinstance;

        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Ivy Engine App";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        // Secondly, secify the instance creation info. This registers required extensions.

        VkInstanceCreateInfo instanceCreateInfo = {};
        instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCreateInfo.pApplicationInfo = &appInfo;
        auto extensions = Window::getGlfwRequiredExtensions();
        instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        instanceCreateInfo.ppEnabledExtensionNames = extensions.data();

        // If validation layers are desired, enable them.

        // TODO: this could be moved totally into DebugMessenger, if we pass a pointer to
        //      the create info and populated it in a DebugMessenger static method. We
        //      already do this, just not for the whole thing.

        if (enableValidationLayers) {

            BOOST_LOG_TRIVIAL(debug) << "instance::createInstance::Enabling validation layers";

            // It is possible that validation layers are not supported.

            if (!DebugMessenger::checkValidationLayerSupport()) {
                throw std::runtime_error("validation layers requested, but not available!");
            }

            for (auto & a : DebugMessenger::getValidationLayers()) {
                BOOST_LOG_TRIVIAL(debug) << "instance::createInstance::Validation layer available:" << a;
            }

            // Pass in the validation layers (as a pointer to an array).

            instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(DebugMessenger::getValidationLayers().size());
            instanceCreateInfo.ppEnabledLayerNames = DebugMessenger::getValidationLayers().data();

            // Get the debug messenger creaate info from DebugMessenger

            VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
            DebugMessenger::populateDebugMessengerCreateInfo(debugCreateInfo);
            BOOST_LOG_TRIVIAL(debug) << "instance::createInstance::finished debugCreateInfo, validation callback fn pointer is at:" << (void*)& (debugCreateInfo.pfnUserCallback);
            instanceCreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;

        } 
        else {
            BOOST_LOG_TRIVIAL(debug) << "instance::createInstance::NOT enabling validation layers";
            instanceCreateInfo.enabledLayerCount = 0;
            instanceCreateInfo.pNext = nullptr;
        }

        // Attempt to make the instance itself

        if (vkCreateInstance(&instanceCreateInfo, nullptr, &vkinstance) != VK_SUCCESS) {
            BOOST_LOG_TRIVIAL(debug) << "createInstance()::failed to create an instance:" << vkinstance;
            throw std::runtime_error("failed to create instance!");
        }

        BOOST_LOG_TRIVIAL(debug) << "createInstance instance end:" << vkinstance;

        // Finally, we must also check that on creation of the instance, the requested GLFW
        //      extensions worked.

        // TODO: this responsibility, for checking window's extensions seems wrong.

        Window::hasGflwRequiredInstanceExtensions();

        // Do a check that the required extensions are supported

        BOOST_LOG_TRIVIAL(debug) << "createInstance()::createInstance instance end:" << vkinstance;
    }
}