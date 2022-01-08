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

BOOST_AUTO_TEST_CASE(TestInitOrder1)
{
    // This test is necessary because initializing in the wrong order can cause unexpected crashes
    BOOST_LOG_TRIVIAL(debug) << "TestInitOrder1:: begin";
    ive::iveWindow window{ 100, 100, "foo"};
    ive::Instance instance;

    {
        ive::DebugMessenger debugMessenger(instance);
        BOOST_TEST((debugMessenger.n_messages == 0));
    }

    BOOST_LOG_TRIVIAL(debug) << "TestInitOrder1:: end";
}

BOOST_AUTO_TEST_CASE(TestInitOrder2)
{
    // This test is necessary because initializing in the wrong order can cause unexpected crashes
    BOOST_LOG_TRIVIAL(debug) << "TestInitOrder2:: begin";
    try {
        ive::Instance instance;
        BOOST_TEST(false);
    }
    catch(const std::exception& e) {
        BOOST_TEST(true);
    }
    BOOST_LOG_TRIVIAL(debug) << "TestInitOrder2:: end";
}