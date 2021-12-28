#include "ive_window.hpp"
#include "validation.hpp"
#include <stdexcept>
#include <boost/log/trivial.hpp>


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

VkSurfaceKHR& iveWindow::createWindowSurface(VkInstance& instance_) {
    BOOST_LOG_TRIVIAL(debug) << "Creating window surface with instance, window, surface " << instance_ 
                                                        << " " << window << " " << surface;
    VkResult result = glfwCreateWindowSurface(instance_, window, NULL, &surface);
    BOOST_LOG_TRIVIAL(debug) << "Result: " << result;
    if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to create window surface");
    }
        BOOST_LOG_TRIVIAL(debug) << "Created window surface with instance, window, surface " << instance_ 
                                                        << " " << window << " " << surface;

    return surface;
}


}

