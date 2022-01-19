#ifndef IVE_WINDOW_HPP
#define IVE_WINDOW_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>
#include <vector>

#include "instance.hpp"

namespace ivy {

    class Window {
        public:
            Window(int w, int h, std::string name);
            ~Window();

            // we do not want to make copies because our glfwWindow pointer will dangle if one is destroyed
            // this is because of our destructor
            Window(const Window&) = delete;
            Window &operator=(const Window&) = delete;

            GLFWwindow* getWindowPtr() {return window;}

            bool shouldClose() { return glfwWindowShouldClose(window); }
            void pollEvents() {glfwPollEvents();}

            // VkSurfaceKHR& createWindowSurface(Instance);

            static std::vector<const char *> getGlfwRequiredExtensions();

            // TODO: rename
            // This function throws an error if GLFW has not gotten its required extensions
            static void hasGflwRequiredInstanceExtensions();

            static bool glfw_initialized;

        private:
            void initWindow();
            GLFWwindow* window;

//            void destroy_surface();

            const int width;
            const int height;
            std::string windowName;
    };

}

#endif
