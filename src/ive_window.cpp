#include "ive_window.hpp"
#include <stdexcept>

namespace ive {

iveWindow::iveWindow(int w, int h, std::string name) : width{w}, height{h}, windowName{name} {
    initWindow();
}

iveWindow::~iveWindow() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

void iveWindow::initWindow() {
    glfwInit();
    
    // no opengl
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
}

VkSurfaceKHR& iveWindow::createWindowSurface(VkInstance instance_) {
    if (glfwCreateWindowSurface(instance_, window, nullptr, &surface) != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface");
    }
    return surface;
}


}

