#define BOOST_TEST_MODULE Init

#include <GLFW/glfw3.h>
#include <boost/test/unit_test.hpp>
#include <boost/log/trivial.hpp>
#include <iostream>

#include "../src/device/debug_messenger.hpp"
#include "../src/device/window.hpp"


BOOST_AUTO_TEST_CASE(TestExtensions)
{
    BOOST_LOG_TRIVIAL(debug) << "TestInitOrder1:: begin";
    ivy::Window window{ 100, 100, "foo"};
    BOOST_CHECK_EQUAL(2, 2);
    std::vector<const char *> extensions = ivy::Window::getGlfwRequiredExtensions();
    for (auto& a : extensions) {
        std::cerr << a << std::endl;
    }
}

BOOST_AUTO_TEST_CASE(TestExtensionsBeforeInit)
{
    BOOST_LOG_TRIVIAL(debug) << "TestExtensionsBeforeInit:: begin, glfw has been initialized? " << ivy::Window::glfw_initialized;

    try {
        std::vector<const char *> extensions = ivy::Window::getGlfwRequiredExtensions();
        BOOST_LOG_TRIVIAL(debug) << "TestExtensionsBeforeInit:: given the exception, should terminate here. ";
        // should not get here
        BOOST_TEST(false);
    }
    catch (const std::exception&) {
        BOOST_TEST(true);
    }
}