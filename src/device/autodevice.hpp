#ifndef AUTODEVICE_HPP
#define AUTODEVICE_HPP

#include <vulkan/vulkan.h>

#include "instance.hpp"
#include "physical_device.hpp"
#include "logical_device.hpp"
#include "framebuffer.hpp"
#include "queue.hpp"
#include "window.hpp"
#include "pipeline.hpp"
#include "debug_messenger.hpp"
#include "swapchain.hpp"
#include "renderpass.hpp"
#include "surface.hpp"
#include "command.hpp"

namespace ivy {

    //  This class abstracts away details of device creation.
    //
    //  With Vulkan, we must choose a physical device, which requires considerable setup.
    //  We must also setup a logical VkDevice that will be used for later operations.
    //  We must also setup a debug messenger, surface, swap chain, etc. 
    //  The idea is to hide as much of that configuration as possible. 
    //  This code can then be used as a skeleton and configured on a project-specific basis.
    //  Many of these operations have relationships or require references to Vulkan pointers.
    //  
    //  TODO: review class design
    class AutoDevice {

        #ifdef NDEBUG
            const bool enableValidationLayers = false;
        #else
            const bool enableValidationLayers = true;
        #endif

        public:

            // TODO: create initialization args such as window size and name
            AutoDevice() :
                window(100, 100, "foo"),
                // debugMessenger is a singleton
                // TODO: move the nested function out to a setup function
                vkinstance(instance.getVkInstanceHandle()),
                debugMessenger(instance),
                surface(instance, window),
                physicalDevice{vkinstance, surface.getSurfaceHandle()},
                queueManager(physicalDevice.getVkPhysicalDeviceHandle(), surface.getSurfaceHandle()),
                logicalDevice(surface.getSurfaceHandle(), physicalDevice, queueManager, debugMessenger),
                swapChain(physicalDevice, surface.getSurfaceHandle(), logicalDevice, window.getWindowPtr(), queueManager), 
                renderPass(logicalDevice, swapChain),
                frameBufferCollection(logicalDevice, swapChain, renderPass),
                pipeline {logicalDevice, swapChain, renderPass, 
                            "/home/j/projects/gdev/gfx/gfxengine/build/../shaders/simple_shader.vert.spv",
                            "/home/j/projects/gdev/gfx/gfxengine/build/../shaders/simple_shader.frag.spv",
                            100, 100},
                commandPool { logicalDevice, queueManager, swapChain, 
                        static_cast<uint32_t>(frameBufferCollection.getFrameBuffersHandle().size()) }
                {

                    commandPool.startRecording(logicalDevice, frameBufferCollection,
                                swapChain, renderPass, pipeline);
                };

            // We absolutely do not want copying, moving of this class
            // Because of the way Vk pointers work
            ~AutoDevice();
            AutoDevice(const AutoDevice &) = delete;
            void operator=(const AutoDevice &) = delete;
            AutoDevice(AutoDevice &&) = delete;
            AutoDevice &operator=(AutoDevice &&) = delete;

            void main();
            void drawFrame();
            void createSemaphores();

            // Helper function to return VkDevice
            // TODO: rename to getVkDevice(). At the moment is not clear which device this refers to
            VkDevice& getLogicalDeviceHandle() {
                    // TODO: replace with proper logging
                    BOOST_LOG_TRIVIAL(debug) << "AutoDevice:: calling getter device() ";   
                    VkDevice& ld = logicalDevice.getLogicalDeviceHandle();
                    BOOST_LOG_TRIVIAL(debug) << "\t AutoDevice:: going to return VkDevice " << ld;    
                    return ld;
                }
            
            // Get a handle to the debugMessenger object (not Vk)
            DebugMessenger& getDebugMessengerHandle() {return debugMessenger; }
            VkExtent2D& getSwapChainExtent() { return swapChain.getSwapChainExtent(); }
            VkFormat& getSwapChainImageFormat() {
                return swapChain.getSwapChainImageFormat();
            }


        private:

            // Fundamental components of the device
            Window window;
            Instance instance;
            Surface surface;
            DebugMessenger debugMessenger;
            // TODO: could wrap vkinstance, but won't bother, wrapping up too much is more danger
            VkInstance& vkinstance;  
            // VkSurfaceKHR surface;
            PhysicalDevice physicalDevice;
            QueueManager queueManager;  
            LogicalDevice logicalDevice;
            SwapChain swapChain;
            RenderPass renderPass;
            FrameBufferCollection frameBufferCollection;
            Pipeline pipeline;
            CommandPool commandPool;
            VkSemaphore imageAvailableSemaphore;
            VkSemaphore renderFinishedSemaphore;
            // Swap chain should have these?
            // std::vector<VkImageView> swapChainImageViews;    
    };
}

#endif
