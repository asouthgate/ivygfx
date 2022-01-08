#ifndef SWAPCHAIN_CHK_HPP
#define SWAPCHAIN_CHK_HPP

#include <cstdint> 
#include <algorithm> 

#include <vulkan/vulkan.h>
#include <vector>
#include <stdexcept>
#include <boost/log/trivial.hpp>


#include "ive_window.hpp"
#include "physical_device.hpp"
#include "logical_device.hpp"

namespace ive {

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    class SwapChain {

        public:

            SwapChain(ive::PhysicalDevice physicalDevice,     
                        VkSurfaceKHR& surface,
                        ive::LogicalDevice& logicalDevice,
                        GLFWwindow* window_ptr, ive::QueueManager queueManager);

            ~SwapChain();

            // ***** FUNCTIONS FOR 3 SWAP CHAIN PROPERTIES:
            VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

            static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);

            VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, GLFWwindow* window_ptr);
            // *****

            // NB the swap chain is not actually supported by default; it is highly tied to
            // the windowing system
            static SwapChainSupportDetails querySwapChainSupport(const VkPhysicalDevice& device, const VkSurfaceKHR& surface);

            SwapChainSupportDetails getSwapChainSupport(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

            // Create a swap chain given a physical device, a surface, and a window
            // TODO: is the window necessary or can we pass something simpler in?
            void createSwapChain(const PhysicalDevice& physicalDevice, 
                                const VkSurfaceKHR& surf, LogicalDevice& logicalDevice,
                                GLFWwindow* window_ptr, QueueManager& queueManager);

            void createImageViews(VkDevice& device);


            VkSwapchainKHR swapChain;
            std::vector<VkImage> swapChainImages;
            VkFormat swapChainImageFormat;
            VkExtent2D swapChainExtent;
            std::vector<VkImageView> swapChainImageViews;

            LogicalDevice& logicalDeviceHandle;
        private:
            void callVkDestructors();

    };
}

#endif