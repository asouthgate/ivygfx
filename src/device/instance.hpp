#ifndef INSTANCE_HPP
#define INSTANCE_HPP

#include <vulkan/vulkan.h>
#include <stdexcept>
#include <boost/log/trivial.hpp>

namespace ivy {
  

    /* Wrapper class for handling VkInstance and guaranteeing proper resource management.

        The VkInstance member variable represents configurable state on a per-application
        basis. Unlike in OpenGL, this state is not global. Initializing the instance also
        initializes the library and allows us to pass configured information to our app.
    */
    class Instance {

        public:


            Instance();

            ~Instance();

            // We don't have a good reason to copy or move this class: delete the operators
            Instance(const Instance &) = delete;
            void operator=(const Instance &) = delete;
            Instance(Instance &&) = delete;
            Instance &operator=(Instance &&) = delete;

            /* Get a handle to the VkInstance object. 
            
                Currently this exposes state, which is not ideal 
            */
            VkInstance& getVkInstanceHandle();

        private:

            VkInstance vkinstance;

            void createInstance();

    };
}

#endif
