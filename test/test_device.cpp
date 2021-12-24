//#ifndef TEST_DEVICE_HPP
//#define TEST_DEVICE_HPP

#define BOOST_TEST_MODULE TestDevice
#include <boost/test/unit_test.hpp>
#include <vulkan/vulkan.h>

#include "../src/validation.hpp"
#include "../src/ive_window.hpp"
#include "../src/instance.hpp"
#include "../src/device.hpp"

struct Fixture {
    Fixture() { 
        BOOST_TEST_MESSAGE("Setting up fixture for device test"); 
    }
    ~Fixture() {
        BOOST_TEST_MESSAGE("Tearing down fixture for device test"); 
    }

    VkInstance instance = ive::createInstance();
    ive::iveWindow window{100, 100, "foo"};
    VkSurfaceKHR surface = window.createWindowSurface(instance);
};

BOOST_AUTO_TEST_CASE(TestDevice1)
{
    BOOST_CHECK_EQUAL(2, 2);
}

BOOST_FIXTURE_TEST_CASE(TestDeviceInit, Fixture)
{
    ive::LogicalDevice(instance, surface);
    BOOST_TEST(true);
}

//#endif