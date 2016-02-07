#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE CPU_Test0
#include <boost/test/unit_test.hpp>
#include "../CPU.hpp"


BOOST_AUTO_TEST_CASE( getAbsTest )
{
    CPU c;
    BOOST_REQUIRE_EQUAL( c.getAbs(0x7c0, 0x1234),  (uword) (0x7c0*16) + 0x1234 );
    BOOST_REQUIRE_EQUAL( c.getAbs(0xfaca, 0xfeda), (uword) ((0xfaca*16) + 0xfeda) );
    BOOST_REQUIRE_EQUAL( c.getAbs(0xffff, 0xffff), (uword) ((0xffff*16) + 0xffff) );
    BOOST_REQUIRE_EQUAL( c.getAbs(-123, 51212),  (uword) ((-123*16) + 51212) );
    BOOST_REQUIRE_EQUAL( c.getAbs(-11512, -21315), (uword) ((-11512*16) + -21315) );
} 



