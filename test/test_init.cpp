#define BOOST_TEST_MODULE Init
#include <boost/test/unit_test.hpp>

#include "../src/device/ive_window.hpp"
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

BOOST_AUTO_TEST_CASE(TestExtensions)
{
    glfwInit();
    BOOST_CHECK_EQUAL(2, 2);
    std::vector<const char *> extensions = ive::iveWindow::getGlfwRequiredExtensions();
    for (auto& a : extensions) {
        std::cerr << a << std::endl;
    }
}

