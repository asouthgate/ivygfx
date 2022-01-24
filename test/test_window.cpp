#define BOOST_TEST_MODULE Init

#include <boost/test/unit_test.hpp>
#include <boost/log/trivial.hpp>

#include "../src/device/debug_messenger.hpp"
#include "../src/device/window.hpp"
#include "../src/device/instance.hpp"

BOOST_AUTO_TEST_CASE(TestInitOrder1)
{
    // This test is necessary because initializing in the wrong order can cause unexpected crashes
    BOOST_LOG_TRIVIAL(debug) << "TestInitOrder1:: begin";
    ivy::Window window{ 100, 100, "foo"};
    ivy::Instance instance;

    {
        ivy::DebugMessenger debugMessenger(instance);
        BOOST_TEST((debugMessenger.n_messages == 0));
    }

    BOOST_LOG_TRIVIAL(debug) << "TestInitOrder1:: end";
}

//TODO: move to test instance
BOOST_AUTO_TEST_CASE(TestInitOrder2)
{
    // This test is necessary because initializing in the wrong order can cause unexpected crashes
    BOOST_LOG_TRIVIAL(debug) << "TestInitOrder2:: begin";
    try {
        ivy::Instance instance;
        BOOST_TEST(false);
    }
    catch(const std::exception& e) {
        BOOST_TEST(true);
    }
    BOOST_LOG_TRIVIAL(debug) << "TestInitOrder2:: end";
}

BOOST_AUTO_TEST_CASE(TestWindow)
{
    // This test is necessary because initializing in the wrong order can cause unexpected crashes
    BOOST_LOG_TRIVIAL(debug) << "TestInitOrder1:: begin";
    ivy::Window window{ 100, 100, "foo"};
    while (!window.shouldClose()) {
        window.pollEvents();
    }

    BOOST_LOG_TRIVIAL(debug) << "TestInitOrder1:: end";
}

