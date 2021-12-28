#ifndef IVE_WINDOW_HPP
#define IVE_WINDOW_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

namespace ive {

    class iveWindow {
        public:
            iveWindow(int w, int h, std::string name);
            ~iveWindow();

            // we do not want to make copies because our glfwWindow pointer will dangle if one is destroyed
            // this is because of our destructor
            iveWindow(const iveWindow&) = delete;
            iveWindow &operator=(const iveWindow&) = delete;


            bool shouldClose() { return glfwWindowShouldClose(window); }

            VkSurfaceKHR& createWindowSurface(VkInstance& instance_);

        private:
            void initWindow();
            GLFWwindow* window;

            VkSurfaceKHR surface;

            const int width;
            const int height;
            std::string windowName;
    };

}

#endif
