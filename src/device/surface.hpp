#ifndef SURFACE_HPP
#define SURFACE_HPP

#include <vulkan/vulkan.hpp>
#include "instance.hpp"
#include "ive_window.hpp"

namespace ive {

    class Surface {

        public:
            Surface(Instance& instance__, iveWindow& win);
            ~Surface();

            // We absolutely do not want copying, moving of this class
            // Because of the way Vk pointers work
            Surface(const Surface &) = delete;
            void operator=(const Surface &) = delete;
            Surface(Surface &&) = delete;
            Surface &operator=(Surface &&) = delete;
            VkSurfaceKHR& getSurfaceHandle() { return surface; }


        private:

            Instance* instanceptr;
            VkSurfaceKHR surface;
            VkSurfaceKHR& createWindowSurface(Instance& instance__, iveWindow& win);

    };

}

#endif