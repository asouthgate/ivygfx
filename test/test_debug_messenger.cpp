#define BOOST_TEST_MODULE TestDevice

#include <boost/test/unit_test.hpp>
#include <boost/log/trivial.hpp>

#include "../src/device/debug_messenger.hpp"
#include "../src/device/window.hpp"
#include "../src/device/instance.hpp"

BOOST_AUTO_TEST_CASE(TestDebugMessengerInit)
{
    {
        ivy::Window window{ 100, 100, "foo"};
        ivy::Instance instance;
        ivy::DebugMessenger debugMessenger{instance};
    }
    BOOST_TEST((ivy::DebugMessenger::n_messages == 0));
}
