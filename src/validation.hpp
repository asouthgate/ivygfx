#ifndef VALIDATION_HPP
#define VALIDATION_HPP

#include <vulkan/vulkan.h>
#include <vector>
#include <string.h>

namespace ive {
    class ValidationLayers {
        public:
            bool checkValidationLayerSupport() {
                uint32_t layerCount;
                vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

                std::vector<VkLayerProperties> availableLayers(layerCount);
                vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

                for (const char *layerName : validationLayers) {
                    bool layerFound = false;
                    // linear scan through the available layers
                    for (const auto &layerProperties : availableLayers) {
                        if (strcmp(layerName, layerProperties.layerName) == 0) {
                            layerFound = true;
                            break;
                        }
                    }

                    if (!layerFound) {
                        return false;
                    }
                }

                return true;
            }

            const std::vector<const char *>& getVector() { return validationLayers; };
            bool enabled = true;
        private:
            const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};

        protected:
        
    };


}

#endif