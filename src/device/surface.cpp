#include "surface.hpp"
#include "ive_window.hpp"

#include <vulkan/vulkan.h>
#include <boost/log/trivial.hpp>


namespace ive {

    Surface::Surface(Instance& inst, iveWindow& win) {
        BOOST_LOG_TRIVIAL(debug) << "Surface::Constructor with empty surface:" << surface;
        createWindowSurface(inst, win);
        BOOST_LOG_TRIVIAL(debug) << "Surface::Created my surface:" << surface;
    }

    Surface::~Surface() {
        BOOST_LOG_TRIVIAL(debug) << "Surface::destroying my surface:" << surface;
        vkDestroySurfaceKHR(instanceptr->getVkInstanceHandle(), surface, NULL);
    }

    VkSurfaceKHR& Surface::createWindowSurface(Instance& instance, iveWindow& ivewindow) {

        instanceptr = &instance;
        VkInstance& instance_ = instance.getVkInstanceHandle();

        GLFWwindow* window = ivewindow.getWindowPtr();


        if (instance_ == VK_NULL_HANDLE) {
            throw std::runtime_error("instance is VK_NULL_HANDLE, cannot create a surface with this; something has gone wrong.");
        }
        BOOST_LOG_TRIVIAL(debug) << "Surface::Creating window surface with instance, window, surface " << instance_ 
                                                            << " " << window << " " << surface;

        VkResult result = glfwCreateWindowSurface(instance_, window, NULL, &surface);

        BOOST_LOG_TRIVIAL(debug) << "Surface::Result: " << result;

        if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface");
        }

        BOOST_LOG_TRIVIAL(debug) << "Surface::Created window surface with instance, window, surface " << instance_ 
                                                            << " " << window << " " << surface;

        return surface;
    }
}