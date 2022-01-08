#define BOOST_TEST_MODULE TestPipeline
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
#include "../src/device/surface.hpp"

#include "../src/pipeline/ive_pipeline.hpp"

struct Fixture {
    Fixture() : vkinstance() { 
        BOOST_TEST_MESSAGE("Setting up fixture for device test"); 
        BOOST_LOG_TRIVIAL(debug) << "A debug severity message";

    }
    ~Fixture() {
        BOOST_TEST_MESSAGE("Tearing down fixture for device test"); 
    }

    ive::Instance instance;
    VkInstance vkinstance = instance.getVkInstanceHandle();
    ive::iveWindow window{ 100, 100, "foo"};
    ive::DebugMessenger debugMessenger{instance};
    ive::Surface surface{instance, window};
};

BOOST_AUTO_TEST_CASE(TestPipelineInit)
{
    BOOST_LOG_TRIVIAL(debug) << "TestPipelineInit:: Testing pipeline initialization";
    ive::AutoDevice ad;
    BOOST_LOG_TRIVIAL(debug) << "TestPipelineInit:: AutoDevice initialized";
    BOOST_LOG_TRIVIAL(debug) << "TestPipelineInit:: AutoDevice device() is " << ad.device();   
    ive::PipelineConfigInfo configInfo = ive::ivePipeline::defaultPipelineConfigInfo(100, 100);
    BOOST_LOG_TRIVIAL(debug) << "TestPipelineInit:: Got default config";
    ive::ivePipeline pipeline { ad, "../shaders/simple_shader.vert.spv", "../shaders/simple_shader.frag.spv",
                           configInfo };
    BOOST_LOG_TRIVIAL(debug) << "TestPipelineInit::Pipeline initialization completed";
}
