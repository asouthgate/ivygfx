#define BOOST_TEST_MODULE TestDevice

#include <boost/test/unit_test.hpp>
#include <boost/log/trivial.hpp>
#include <iostream>

#include <vulkan/vulkan.h>

#include "../src/device/debug_messenger.hpp"
#include "../src/device/window.hpp"
#include "../src/device/instance.hpp"
#include "../src/device/logical_device.hpp"
#include "../src/device/debug_messenger.hpp"
#include "../src/device/physical_device.hpp"
#include "../src/device/surface.hpp"

struct Fixture {
    Fixture() { 
        std::cerr << "\n\n\n===============================\n\n\n" << std::endl;
        std::cerr << "NEW FIXTURE\n\n\n" << std::endl;
        std::cerr << "===============================\n\n\n" << std::endl;
        BOOST_TEST_MESSAGE("Setting up fixture for device test"); 
        BOOST_LOG_TRIVIAL(debug) << "Setting up fixture";
        debugMessenger.n_messages = 0;

    }
    ~Fixture() {
        BOOST_TEST_MESSAGE("Tearing down fixture for device test"); 
    }

    ivy::Window window{ 100, 100, "foo"};
    ivy::Instance instance;
    ivy::DebugMessenger debugMessenger{instance};
    ivy::Surface surface{instance, window};
};

BOOST_FIXTURE_TEST_CASE(TestQueueManagerInit, Fixture)
{
    BOOST_LOG_TRIVIAL(debug) << "TestQueueManagerInit::Testing physical device initialization";
    ivy::PhysicalDevice pd (instance.getVkInstanceHandle(), surface.getSurfaceHandle());   
    ivy::QueueManager(pd.getVkPhysicalDeviceHandle(), surface.getSurfaceHandle());
    BOOST_TEST(true);
    BOOST_LOG_TRIVIAL(debug) << "TestQueueManagerInit::Finished testing physical device initialization!";
    BOOST_TEST((debugMessenger.n_messages == 0));
}

BOOST_FIXTURE_TEST_CASE(TestPhysicalDeviceInit, Fixture)
{
    BOOST_LOG_TRIVIAL(debug) << "TestPhysicalDeviceInit::Testing physical device initialization";
    ivy::PhysicalDevice(instance.getVkInstanceHandle(), surface.getSurfaceHandle());
    BOOST_TEST(true);
    BOOST_LOG_TRIVIAL(debug) << "TestPhysicalDeviceInit::Finished testing physical device initialization!";
    BOOST_TEST((debugMessenger.n_messages == 0));
}

BOOST_FIXTURE_TEST_CASE(TestLogicalDeviceInit, Fixture)
{
    BOOST_LOG_TRIVIAL(debug) << "TestLogicalDeviceInit::Testing logical device initialization";
    ivy::PhysicalDevice pd {instance.getVkInstanceHandle(), surface.getSurfaceHandle()};
    ivy::QueueManager qm(pd.getVkPhysicalDeviceHandle(), surface.getSurfaceHandle());

    ivy::LogicalDevice log {surface.getSurfaceHandle(), pd, qm, debugMessenger};
    BOOST_TEST(true);
    BOOST_LOG_TRIVIAL(debug) << "TestLogicalDeviceInit::Finished testing logical device initialization!";
    BOOST_LOG_TRIVIAL(debug) << "TestLogicalDeviceInit::Number of messages " << debugMessenger.n_messages;
    BOOST_TEST((debugMessenger.n_messages == 0));

    BOOST_LOG_TRIVIAL(debug) << "TestLogicalDeviceInit::n messages" << debugMessenger.n_messages;
    vkEnumeratePhysicalDevices(instance.getVkInstanceHandle(), nullptr, nullptr );
    BOOST_LOG_TRIVIAL(debug) << "TestLogicalDeviceInit::n messages" << debugMessenger.n_messages;
    BOOST_TEST((debugMessenger.n_messages == 1));
}