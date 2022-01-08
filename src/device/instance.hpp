#ifndef INSTANCE_HPP
#define INSTANCE_HPP

#include <vulkan/vulkan.h>
#include <stdexcept>
#include <boost/log/trivial.hpp>

namespace ive {
  
    class Instance {

        public:
            // Create Vulkan instance
            //
            // Generally this code follows the convention of populating structs that specify options
            // Don't panic with these! Check the Vulkan docs.
            VkInstance createInstance();
            Instance();
            ~Instance();
            VkInstance& getVkInstanceHandle();

            // We absolutely do not want copying, moving of this class
            // Because of the way Vk pointers work
            Instance(const Instance &) = delete;
            void operator=(const Instance &) = delete;
            Instance(Instance &&) = delete;
            Instance &operator=(Instance &&) = delete;

        private:
            VkInstance vkinstance;
    };
}

#endif