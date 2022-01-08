#define BOOST_TEST_MODULE TestDevice
#include <boost/test/unit_test.hpp>
#include <boost/log/trivial.hpp>
#include <iostream>

#include <vulkan/vulkan.h>

#include "../src/device/debug_messenger.hpp"
#include "../src/device/ive_window.hpp"
#include "../src/device/instance.hpp"
#include "../src/device/logical_device.hpp"
#include "../src/device/debug_messenger.hpp"
#include "../src/device/physical_device.hpp"
#include "../src/device/autodevice.hpp"
#include "../src/device/swapchain.hpp"


BOOST_AUTO_TEST_CASE(TestDebugMessengerInit)
{
    {
        ive::iveWindow window{ 100, 100, "foo"};
        ive::Instance instance;
        ive::DebugMessenger debugMessenger{instance};
    }
    BOOST_TEST((ive::DebugMessenger::n_messages == 0));
}
