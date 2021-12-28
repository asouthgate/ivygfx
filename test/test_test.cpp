#define BOOST_TEST_MODULE Test1
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_CASE(PassTest)
{
    BOOST_CHECK_EQUAL(2, 2);
}

