#define BOOST_TEST_MODULE TestDevice
#include <boost/test/unit_test.hpp>
#include <boost/log/trivial.hpp>
#include <iostream>

#include <vulkan/vulkan.h>

#include "../src/device/autodevice.hpp"

BOOST_AUTO_TEST_CASE(TestAutoDevice)
{
    std::cerr << "\n\n\n===============================\n\n\n" << std::endl;
    std::cerr << "TEST AUTODEVICE\n\n\n" << std::endl;
    std::cerr << "===============================\n\n\n" << std::endl;
    BOOST_LOG_TRIVIAL(debug) << "TestAutoDevice:Testing AutoDevice initialization";
    ivy::AutoDevice ad;
    BOOST_LOG_TRIVIAL(debug) << "TestAutoDevice::AutoDevice initialization completed";
    BOOST_LOG_TRIVIAL(debug) << "TestAutoDevice::Number of messages " << ad.getDebugMessengerHandle().n_messages;
    BOOST_TEST((ad.getDebugMessengerHandle().n_messages == 0));
    BOOST_LOG_TRIVIAL(debug) << "TestAutoDevice::AutoDevice completed autodevice initialization test. ";

}
