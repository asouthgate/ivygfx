#include <stdexcept>
#include <boost/log/trivial.hpp>
#include <unordered_set>
#include <iostream>

#include "ive_window.hpp"
#include "debug_messenger.hpp"


namespace ive {

    bool iveWindow::glfw_initialized = false;

    iveWindow::iveWindow(int w, int h, std::string name) : width{w}, height{h}, windowName{name} {
        BOOST_LOG_TRIVIAL(debug) << "iveWindow::constructor called";
        initWindow();
    }

    iveWindow::~iveWindow() {
        glfwDestroyWindow(window);
        glfwTerminate();
        iveWindow::glfw_initialized = false;
    }


    void iveWindow::initWindow() {
        glfwInit();
        iveWindow::glfw_initialized = true;
        
        // no opengl
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
    }


    // Get required extensions for glfw
    //
    // Returns:
    //    std::vector<const char*> extensions: vector of C string extension names
    std::vector<const char *> iveWindow::getGlfwRequiredExtensions() {

        // if we dnot glfwinit first, it will only give back some extensions
        if (!iveWindow::glfw_initialized) { 
            BOOST_LOG_TRIVIAL(debug) << "iveWindow::glfw_initialized is " << iveWindow::glfw_initialized;
            throw std::runtime_error("Cannot get extensions for glfw before init."); 
        }
        else {
            BOOST_LOG_TRIVIAL(debug) << "iveWindow::glfw_initialized is " << iveWindow::glfw_initialized;
        }

        uint32_t glfwExtensionCount = 0;
        const char **glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        BOOST_LOG_TRIVIAL(debug) << "iveWindow::glfwExtensionCount is " << glfwExtensionCount;

        for (int i = 0; i < glfwExtensionCount; ++i) {
            BOOST_LOG_TRIVIAL(debug) << glfwExtensions[i];            
        }

        std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);


        if (enableValidationLayers) {
            BOOST_LOG_TRIVIAL(debug) << "iveWindow::Adding VK_EXT_DEBUG_UTILS_EXTENSION_NAME layer";
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        for (const auto &extension : extensions) {
            BOOST_LOG_TRIVIAL(debug) << "glfw required extension:" << extension;
        }

        return extensions;
    }

      // TODO: rename
    // This function throws an error if GLFW has not gotten its required extensions
    void iveWindow::hasGflwRequiredInstanceExtensions() {
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

}

