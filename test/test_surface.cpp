#define BOOST_TEST_MODULE Init

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <boost/test/unit_test.hpp>
#include <boost/log/trivial.hpp>
#include <iostream>

#include "../src/device/window.hpp"
#include "../src/device/instance.hpp"
#include "../src/device/debug_messenger.hpp"
#include "../src/device/surface.hpp"

BOOST_AUTO_TEST_CASE(TestSurfaceCreation)
{
    {
        // This test is necessary because initializing in the wrong order can cause unexpected crashes
        BOOST_LOG_TRIVIAL(debug) << "TestSurfaceCreation:: begin";
        ivy::Window window{ 100, 100, "foo"};

        ivy::Instance instance;
        ivy::DebugMessenger debugMessenger(instance);

        BOOST_LOG_TRIVIAL(debug) << "TestSurfaceCreation:: window initialized";
        {
            ivy::Surface surf(instance, window);
            BOOST_TEST((ivy::DebugMessenger::n_messages == 0));
        }
        BOOST_LOG_TRIVIAL(debug) << "TestSurfaceCreation:: n messages:" << ivy::DebugMessenger::n_messages;

    }
    BOOST_TEST((ivy::DebugMessenger::n_messages == 0));
    BOOST_LOG_TRIVIAL(debug) << "TestSurfaceCreation:: n messages:" << ivy::DebugMessenger::n_messages;

    BOOST_LOG_TRIVIAL(debug) << "TestSurfaceCreation:: end";
}