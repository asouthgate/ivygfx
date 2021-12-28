#define BOOST_TEST_MODULE TestDevice
#include <boost/test/unit_test.hpp>
#include <boost/log/trivial.hpp>

#include <vulkan/vulkan.h>

#include "../src/validation.hpp"
#include "../src/ive_window.hpp"
#include "../src/instance.hpp"
#include "../src/device/logical_device.hpp"
#include "../src/debug_messenger.hpp"
#include "../src/device/physical_device.hpp"

struct Fixture {
    Fixture() : vkinstance() { 
        BOOST_TEST_MESSAGE("Setting up fixture for device test"); 
        BOOST_LOG_TRIVIAL(debug) << "A debug severity message";

    }
    ~Fixture() {
        BOOST_TEST_MESSAGE("Tearing down fixture for device test"); 
    }

    VkInstance vkinstance;
    ive::iveWindow window{ 100, 100, "foo"};
    ive::DebugMessenger debugMessenger{(ive::createInstance(vkinstance), vkinstance)};
    VkSurfaceKHR surface = window.createWindowSurface(vkinstance);
};

BOOST_AUTO_TEST_CASE(TestDevice1)
{
    BOOST_LOG_TRIVIAL(debug) << "Testing null test";

    BOOST_CHECK_EQUAL(2, 2);
}

BOOST_FIXTURE_TEST_CASE(TestQueueManagerInit, Fixture)
{
    BOOST_LOG_TRIVIAL(debug) << "Testing physical device initialization";
    ive::PhysicalDevice pd (vkinstance, surface);   
    ive::QueueManager(pd.getVkPhysicalDeviceHandle(), surface);
    BOOST_TEST(true);
    BOOST_LOG_TRIVIAL(debug) << "Finished testing physical device initialization!";
}

BOOST_FIXTURE_TEST_CASE(TestPhysicalDeviceInit, Fixture)
{
    BOOST_LOG_TRIVIAL(debug) << "Testing physical device initialization";
    ive::PhysicalDevice(vkinstance, surface);
    BOOST_TEST(true);
    BOOST_LOG_TRIVIAL(debug) << "Finished testing physical device initialization!";
}

BOOST_FIXTURE_TEST_CASE(TestLogicalDeviceInit, Fixture)
{
    BOOST_LOG_TRIVIAL(debug) << "Testing logical device initialization";
    ive::PhysicalDevice pd {vkinstance, surface};
    ive::QueueManager qm(pd.getVkPhysicalDeviceHandle(), surface);

    ive::LogicalDevice log {surface, pd, qm};
    BOOST_TEST(true);
    BOOST_LOG_TRIVIAL(debug) << "Finished testing logical device initialization!";
}

