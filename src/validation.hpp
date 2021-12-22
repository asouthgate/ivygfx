#include<vulkan/vulkan.h>
#include<vector>

namespace ive {
    class ValidationLayers {
        public:
            bool checkValidationLayerSupport();

        private:
            const std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation"};

        protected:
        
    };


}