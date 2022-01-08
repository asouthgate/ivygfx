#define BOOST_TEST_MODULE Init

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <boost/test/unit_test.hpp>
#include <boost/log/trivial.hpp>
#include <iostream>

#include "../src/device/debug_messenger.hpp"
#include "../src/device/ive_window.hpp"
#include "../src/device/instance.hpp"
#include "../src/device/logical_device.hpp"
#include "../src/device/debug_messenger.hpp"
#include "../src/device/physical_device.hpp"
#include "../src/device/autodevice.hpp"
#include "../src/device/swapchain.hpp"

BOOST_AUTO_TEST_CASE(TestExtensions)
{
    BOOST_LOG_TRIVIAL(debug) << "TestInitOrder1:: begin";
    ive::iveWindow window{ 100, 100, "foo"};
    BOOST_CHECK_EQUAL(2, 2);
    std::vector<const char *> extensions = ive::iveWindow::getGlfwRequiredExtensions();
    for (auto& a : extensions) {
        std::cerr << a << std::endl;
    }
}

BOOST_AUTO_TEST_CASE(TestExtensionsBeforeInit)
{
    BOOST_LOG_TRIVIAL(debug) << "TestExtensionsBeforeInit:: begin, glfw has been initialized? " << ive::iveWindow::glfw_initialized;

    try {
        std::vector<const char *> extensions = ive::iveWindow::getGlfwRequiredExtensions();
        BOOST_LOG_TRIVIAL(debug) << "TestExtensionsBeforeInit:: given the exception, should terminate here. ";
        // should not get here
        BOOST_TEST(false);
    }
    catch (const std::exception&) {
        BOOST_TEST(true);
    }
}