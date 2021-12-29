#define BOOST_TEST_MODULE TestDevice
#include <boost/test/unit_test.hpp>
#include <boost/log/trivial.hpp>

#include <vulkan/vulkan.h>

#include "../src/device/debug_messenger.hpp"
#include "../src/device/ive_window.hpp"
#include "../src/device/instance.hpp"
#include "../src/device/logical_device.hpp"
#include "../src/device/debug_messenger.hpp"
#include "../src/device/physical_device.hpp"
#include "../src/device/autodevice.hpp"
#include "../src/device/swapchain.hpp"

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
    ive::DebugMessenger& debugMessenger = ive::DebugMessenger::get_instance((ive::createInstance(vkinstance), vkinstance));
    VkSurfaceKHR surface = window.createWindowSurface(vkinstance);
};

BOOST_AUTO_TEST_CASE(TestDevice1)
{
    BOOST_LOG_TRIVIAL(debug) << "Testing null test";

    BOOST_CHECK_EQUAL(2, 2);
}

BOOST_FIXTURE_TEST_CASE(TestDebugMessenger, Fixture)
{
    BOOST_LOG_TRIVIAL(debug) << "Testing debug messenger";

    try {
        ive::DebugMessenger debugMessenger2{(ive::createInstance(vkinstance), vkinstance)};
        BOOST_TEST(false);
    }
    catch (const std::exception&) {
        BOOST_TEST(true);
    }
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

    ive::LogicalDevice log {surface, pd, qm, debugMessenger};
    BOOST_TEST(true);
    BOOST_LOG_TRIVIAL(debug) << "Finished testing logical device initialization!";
}

BOOST_FIXTURE_TEST_CASE(TestChooseSwapSurfaceFormat, Fixture)
{
    BOOST_LOG_TRIVIAL(debug) << "Testing swap chain support";
    const std::vector<VkSurfaceFormatKHR> availableFormats;
    VkFormat pref = VK_FORMAT_B8G8R8A8_SRGB;
    VkColorSpaceKHR pref2 = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    

    std::vector<VkPresentModeKHR> emptyvec{};
    BOOST_TEST((ive::SwapChain::chooseSwapPresentMode(emptyvec) == VK_PRESENT_MODE_FIFO_KHR));
    BOOST_LOG_TRIVIAL(debug) << "AutoDevice initialization completed";
}

// Integration test; for vulkan API swap chain creation requires many other parts
BOOST_FIXTURE_TEST_CASE(TestSwapChainCreation, Fixture)
{
    BOOST_LOG_TRIVIAL(debug) << "TestSwapChainCreation:: Test swap chain creation";
    ive::PhysicalDevice physicalDevice (vkinstance, surface);      
    BOOST_LOG_TRIVIAL(debug) << "TestSwapChainCreation:: Created physical device";
    ive::QueueManager queueManager(physicalDevice.getVkPhysicalDeviceHandle(), surface);
    BOOST_LOG_TRIVIAL(debug) << "TestSwapChainCreation:: Created queue manager";
    ive::LogicalDevice logicalDevice {surface, physicalDevice, queueManager, debugMessenger};
    BOOST_LOG_TRIVIAL(debug) << "TestSwapChainCreation:: Created swap chain";
    ive::SwapChain swapChain(physicalDevice, surface, logicalDevice, window.getWindowPtr(), queueManager);
    BOOST_LOG_TRIVIAL(debug) << "TestSwapChainCreation::Finished swap chain creation";
    BOOST_TEST(true);
}

BOOST_AUTO_TEST_CASE(TestAutoDevice)
{
    BOOST_LOG_TRIVIAL(debug) << "Testing AutoDevice initialization";
    ive::AutoDevice ad;
    BOOST_LOG_TRIVIAL(debug) << "AutoDevice initialization completed";
}
