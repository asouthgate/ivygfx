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
#include "../src/device/surface.hpp"

struct Fixture {
    Fixture() : vkinstance() { 
        std::cerr << "\n\n\n===============================\n\n\n" << std::endl;
        std::cerr << "NEW FIXTURE\n\n\n" << std::endl;
        std::cerr << "===============================\n\n\n" << std::endl;
        BOOST_TEST_MESSAGE("Setting up fixture for device test"); 
        BOOST_LOG_TRIVIAL(debug) << "Setting up fixture";
        debugMessenger.n_messages = 0;

    }
    ~Fixture() {
        vkDestroyInstance(vkinstance, nullptr);
        BOOST_TEST_MESSAGE("Tearing down fixture for device test"); 
    }

    ive::Instance instance;
    VkInstance vkinstance = instance.getVkInstanceHandle();
    ive::iveWindow window{ 100, 100, "foo"};
    ive::DebugMessenger debugMessenger{instance};
    ive::Surface surface{instance, window};
};

// BOOST_AUTO_TEST_CASE(TestDevice1)
// {
//     BOOST_LOG_TRIVIAL(debug) << "Testing null test";

//     BOOST_CHECK_EQUAL(2, 2);
// }

// BOOST_FIXTURE_TEST_CASE(TestDebugMessenger, Fixture)
// {
//     BOOST_LOG_TRIVIAL(debug) << "TestDebugMessenger::Testing debug messenger";

//     try {
//         ive::DebugMessenger debugMessenger2(vkinstance);
//         // should never get to here
//         BOOST_TEST(false);
//     }
//     catch (const std::exception&) {
//         BOOST_LOG_TRIVIAL(debug) << "TestDebugMessenger::Testing debug messenger... triggered an exception as intended";
//         BOOST_TEST(true);
//     }
    
//     BOOST_LOG_TRIVIAL(debug) << "TestDebugMessenger::n messages" << debugMessenger.n_messages;
//     vkEnumeratePhysicalDevices(vkinstance, nullptr, nullptr );
//     BOOST_LOG_TRIVIAL(debug) << "TestDebugMessenger::n messages" << debugMessenger.n_messages;
//     BOOST_TEST((debugMessenger.n_messages == 1));
//     vkEnumeratePhysicalDevices(vkinstance, nullptr, nullptr );
//     BOOST_LOG_TRIVIAL(debug) << "TestDebugMessenger::n messages" << debugMessenger.n_messages;
//     BOOST_TEST((debugMessenger.n_messages == 2));
//     BOOST_LOG_TRIVIAL(debug) << "TestDebugMessenger:: FINISHED";
// }


// BOOST_FIXTURE_TEST_CASE(TestDebugMessenger2, Fixture)
// {
//     BOOST_LOG_TRIVIAL(debug) << "TestDebugMessenger2::Testing debug messenger";

//     try {
//         ive::DebugMessenger debugMessenger2(vkinstance);
//         // should never get to here
//         BOOST_TEST(false);
//     }
//     catch (const std::exception&) {
//         BOOST_LOG_TRIVIAL(debug) << "TestDebugMessenger2::Testing debug messenger... triggered an exception as intended";
//         BOOST_TEST(true);
//     }
    
//     BOOST_LOG_TRIVIAL(debug) << "TestDebugMessenger::n messages" << debugMessenger.n_messages;
//     vkEnumeratePhysicalDevices(vkinstance, nullptr, nullptr );
//     BOOST_LOG_TRIVIAL(debug) << "TestDebugMessenger::n messages" << debugMessenger.n_messages;
//     BOOST_TEST((debugMessenger.n_messages == 1));
//     vkEnumeratePhysicalDevices(vkinstance, nullptr, nullptr );
//     BOOST_LOG_TRIVIAL(debug) << "TestDebugMessenger::n messages" << debugMessenger.n_messages;
//     BOOST_TEST((debugMessenger.n_messages == 2));
//     BOOST_LOG_TRIVIAL(debug) << "TestDebugMessenger:: FINISHED";

// }

// BOOST_FIXTURE_TEST_CASE(TestQueueManagerInit, Fixture)
// {
//     BOOST_LOG_TRIVIAL(debug) << "TestQueueManagerInit::Testing physical device initialization";
//     ive::PhysicalDevice pd (vkinstance, surface);   
//     ive::QueueManager(pd.getVkPhysicalDeviceHandle(), surface);
//     BOOST_TEST(true);
//     BOOST_LOG_TRIVIAL(debug) << "TestQueueManagerInit::Finished testing physical device initialization!";
//     BOOST_TEST((debugMessenger.n_messages == 0));
// }

// BOOST_FIXTURE_TEST_CASE(TestPhysicalDeviceInit, Fixture)
// {
//     BOOST_LOG_TRIVIAL(debug) << "TestPhysicalDeviceInit::Testing physical device initialization";
//     ive::PhysicalDevice(vkinstance, surface);
//     BOOST_TEST(true);
//     BOOST_LOG_TRIVIAL(debug) << "TestPhysicalDeviceInit::Finished testing physical device initialization!";
//     BOOST_TEST((debugMessenger.n_messages == 0));
// }

// BOOST_FIXTURE_TEST_CASE(TestLogicalDeviceInit, Fixture)
// {
//     BOOST_LOG_TRIVIAL(debug) << "TestLogicalDeviceInit::Testing logical device initialization";
//     ive::PhysicalDevice pd {vkinstance, surface};
//     ive::QueueManager qm(pd.getVkPhysicalDeviceHandle(), surface);

//     ive::LogicalDevice log {surface, pd, qm, debugMessenger};
//     BOOST_TEST(true);
//     BOOST_LOG_TRIVIAL(debug) << "TestLogicalDeviceInit::Finished testing logical device initialization!";
//     BOOST_LOG_TRIVIAL(debug) << "TestLogicalDeviceInit::Number of messages " << debugMessenger.n_messages;
//     BOOST_TEST((debugMessenger.n_messages == 0));

//     BOOST_LOG_TRIVIAL(debug) << "TestLogicalDeviceInit::n messages" << debugMessenger.n_messages;
//     vkEnumeratePhysicalDevices(vkinstance, nullptr, nullptr );
//     BOOST_LOG_TRIVIAL(debug) << "TestLogicalDeviceInit::n messages" << debugMessenger.n_messages;
//     BOOST_TEST((debugMessenger.n_messages == 1));
// }

// BOOST_FIXTURE_TEST_CASE(TestChooseSwapSurfaceFormat, Fixture)
// {
//     BOOST_LOG_TRIVIAL(debug) << "TestChooseSwapSurfaceFormat::Testing swap chain support";
//     const std::vector<VkSurfaceFormatKHR> availableFormats;
//     VkFormat pref = VK_FORMAT_B8G8R8A8_SRGB;
//     VkColorSpaceKHR pref2 = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    

//     std::vector<VkPresentModeKHR> emptyvec{};
//     BOOST_TEST((ive::SwapChain::chooseSwapPresentMode(emptyvec) == VK_PRESENT_MODE_FIFO_KHR));
//     BOOST_LOG_TRIVIAL(debug) << "TestChooseSwapSurfaceFormat::AutoDevice initialization completed";
//     BOOST_LOG_TRIVIAL(debug) << "TestChooseSwapSurfaceFormat::Number of messages " << debugMessenger.n_messages;
//     BOOST_TEST((debugMessenger.n_messages == 0));


// }


// Integration test; for vulkan API swap chain creation requires many other parts
BOOST_FIXTURE_TEST_CASE(TestSwapChainCreation, Fixture)
{
    BOOST_LOG_TRIVIAL(debug) << "TestSwapChainCreation:: Test swap chain creation";
    ive::PhysicalDevice physicalDevice (vkinstance, surface.getSurfaceHandle());  

    BOOST_LOG_TRIVIAL(debug) << "TestSwapChainCreation:: Created physical device";
    ive::QueueManager queueManager(physicalDevice.getVkPhysicalDeviceHandle(), surface.getSurfaceHandle());


    BOOST_LOG_TRIVIAL(debug) << "TestSwapChainCreation:: Created queue manager";
    ive::LogicalDevice logicalDevice {surface.getSurfaceHandle(), physicalDevice, queueManager, debugMessenger};
    {
        // Need local scope so destructor gets called and debug messages can be assessed
        ive::SwapChain swapChain(physicalDevice, surface.getSurfaceHandle(), logicalDevice, window.getWindowPtr(), queueManager);
        BOOST_LOG_TRIVIAL(debug) << "TestSwapChainCreation:: Created swap chain";
    }

    BOOST_TEST(true);
    BOOST_LOG_TRIVIAL(debug) << "TestSwapChainCreation:: Number of messages " << debugMessenger.n_messages;
    BOOST_TEST((debugMessenger.n_messages == 0));

    BOOST_LOG_TRIVIAL(debug) << "TestSwapChainCreation::n messages" << debugMessenger.n_messages;
    vkEnumeratePhysicalDevices(vkinstance, nullptr, nullptr );
    BOOST_LOG_TRIVIAL(debug) << "TestSwapChainCreation::n messages" << debugMessenger.n_messages;
    BOOST_TEST((debugMessenger.n_messages == 1));

    BOOST_LOG_TRIVIAL(debug) << "TestSwapChainCreation::Finished testing swap chain creation";

}
