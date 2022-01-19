#define BOOST_TEST_MODULE TestDevice
#include <boost/test/unit_test.hpp>
#include <boost/log/trivial.hpp>
#include <iostream>

#include <vulkan/vulkan.h>

#include "../src/device/debug_messenger.hpp"
#include "../src/device/window.hpp"
#include "../src/device/instance.hpp"
#include "../src/device/framebuffer.hpp"
#include "../src/device/logical_device.hpp"
#include "../src/device/debug_messenger.hpp"
#include "../src/device/pipeline.hpp"
#include "../src/device/physical_device.hpp"
#include "../src/device/renderpass.hpp"
#include "../src/device/swapchain.hpp"
#include "../src/device/surface.hpp"
#include "../src/device/command.hpp"

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

// Integration test; for vulkan API swap chain creation requires many other parts
BOOST_FIXTURE_TEST_CASE(TestSwapChainCreation, Fixture)
{
    BOOST_LOG_TRIVIAL(debug) << "TestSwapChainCreation:: Test swap chain creation";
    ivy::PhysicalDevice physicalDevice (instance.getVkInstanceHandle(), surface.getSurfaceHandle());  

    BOOST_LOG_TRIVIAL(debug) << "TestSwapChainCreation:: Created physical device";
    ivy::QueueManager queueManager(physicalDevice.getVkPhysicalDeviceHandle(), surface.getSurfaceHandle());

    BOOST_LOG_TRIVIAL(debug) << "TestSwapChainCreation:: Created queue manager";
    ivy::LogicalDevice logicalDevice {surface.getSurfaceHandle(), physicalDevice, queueManager, debugMessenger};
    ivy::SwapChain swapChain(physicalDevice, surface.getSurfaceHandle(), logicalDevice, window.getWindowPtr(), queueManager);
    ivy::RenderPass renderPass {logicalDevice, swapChain};
    ivy::FrameBufferCollection fb {logicalDevice, swapChain, renderPass};

    uint32_t w = 100; 
    uint32_t h = 100;
    ivy::Pipeline pl {logicalDevice, swapChain, renderPass, 
                    "/home/j/projects/gdev/gfx/gfxengine/build/../shaders/simple_shader.vert.spv",
                    "/home/j/projects/gdev/gfx/gfxengine/build/../shaders/simple_shader.frag.spv",
                            w, h};

    {
        ivy::CommandPool commandPool{logicalDevice, queueManager, swapChain, 
                        static_cast<uint32_t>(fb.getFrameBuffersHandle().size())};
        // Need local scope so destructor gets called and debug messages can be assessed
        BOOST_LOG_TRIVIAL(debug) << "TestSwapChainCreation:: Created command pool";
        commandPool.startRecording(logicalDevice, fb,
            swapChain, renderPass, pl);
        BOOST_LOG_TRIVIAL(debug) << "TestSwapChainCreation:: Successfully began recording";

    }

    BOOST_TEST(true);
    BOOST_LOG_TRIVIAL(debug) << "TestSwapChainCreation:: Number of messages " << debugMessenger.n_messages;
    BOOST_TEST((debugMessenger.n_messages == 0));

    BOOST_LOG_TRIVIAL(debug) << "TestSwapChainCreation::n messages" << debugMessenger.n_messages;
    vkEnumeratePhysicalDevices(instance.getVkInstanceHandle(), nullptr, nullptr );
    BOOST_LOG_TRIVIAL(debug) << "TestSwapChainCreation::n messages" << debugMessenger.n_messages;
    BOOST_TEST((debugMessenger.n_messages == 1));

    BOOST_LOG_TRIVIAL(debug) << "TestSwapChainCreation::Finished testing swap chain creation";

}
