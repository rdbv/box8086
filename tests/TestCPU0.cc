#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TestCPU0
#include <boost/test/unit_test.hpp>

#include "../src/CPU/CPU.hpp"

#define MEM(x) ( &(*x)[0] )

BOOST_AUTO_TEST_CASE( testAbsAddrWithoutDisplacement ) {
    CPU cpu;
    Memory *mem = cpu.getMemory();

    ubyte code[] = {
        0x00, 0x16, 0xce, 0xfa, // add [0xface], dl
    };

    memcpy(MEM(mem), code, sizeof(code) );

    cpu.mod.decode(BX_SI);
    cpu.regs.bx = 0xf33;
    cpu.regs.si = 0x33f;

    BOOST_REQUIRE_EQUAL( cpu.getAbsoluteAddressModRM(), 0xf33 + 0x33f );

    cpu.mod.decode(BX_DI);
    cpu.regs.bx = 0xfade;
    cpu.regs.di = 0xedaf;

    BOOST_REQUIRE_EQUAL( cpu.getAbsoluteAddressModRM(), (uword) (0xfade + 0xedaf) );
   
    cpu.mod.decode(BP_SI);
    cpu.regs.bp = 0x1234;
    cpu.regs.si = 0x4321;

    BOOST_REQUIRE_EQUAL( cpu.getAbsoluteAddressModRM(), (uword) (0x1234 + 0x4321) );

    cpu.mod.decode(BP_DI);
    cpu.regs.bp = 0x5134;
    cpu.regs.di = 0x2134;

    BOOST_REQUIRE_EQUAL( cpu.getAbsoluteAddressModRM(), (uword) (0x5134 + 0x2134) );

    cpu.mod.decode(SI);
    cpu.regs.si = 0x1234;

    BOOST_REQUIRE_EQUAL( cpu.getAbsoluteAddressModRM(), (uword) (0x1234) );

    cpu.mod.decode(DI);
    cpu.regs.di = 0x4321;

    BOOST_REQUIRE_EQUAL( cpu.getAbsoluteAddressModRM(), (uword) (0x4321) );
    
    cpu.mod.decode(UWORD);
    BOOST_REQUIRE_EQUAL( cpu.getAbsoluteAddressModRM(), (uword) 0xface );

    cpu.mod.decode(BX);
    cpu.regs.bx = 0xffff;
    BOOST_REQUIRE_EQUAL( cpu.getAbsoluteAddressModRM(), (uword) (0xffff) );
}

BOOST_AUTO_TEST_CASE( testAbsAddrDisplacement ) {

    CPU cpu;
    Memory *mem = cpu.getMemory();

    ubyte code [] = {
        0x00, 0x40, 0x0f,         // add [bx+si+0xf], al
        0x00, 0x80, 0x3f, 0x03,   // add [bx+si+0x33f], al
        0x00, 0x40, 0xf1,         // add [bx+si-0xf], al
        0x00, 0x80, 0xc1, 0xfc,   // add [bx+si-0x33f], al
    };

    memcpy(MEM(mem), code, sizeof(code));

    /* Add displacement */
    // byte 
    cpu.mod.decode(0x40);
    cpu.regs.bx = 0xf31;
    cpu.regs.si = 0x13f;
    BOOST_REQUIRE_EQUAL( cpu.getAbsoluteAddressModRM(), (uword) (0xf31 + 0x13f) + 0xf );

    // word
    memcpy(MEM(mem), &code[3], 4);
    cpu.mod.decode(0x80);
    BOOST_REQUIRE_EQUAL( cpu.getAbsoluteAddressModRM(), (uword) (0xf31 + 0x13f) + 0x33f );

    /* Sub displacement */
    // byte
    memcpy(MEM(mem), &code[7], 3);
    cpu.mod.decode(0x40);
    BOOST_REQUIRE_EQUAL( cpu.getAbsoluteAddressModRM(), (uword) (0xf31 + 0x13f) - 0xf );

    // word
    memcpy(MEM(mem), &code[10], 4);
    cpu.mod.decode(0x80);
    BOOST_REQUIRE_EQUAL( cpu.getAbsoluteAddressModRM(), (uword) (0xf31 + 0x13f) - 0x33f );
}


BOOST_AUTO_TEST_CASE( setOperandsTest ) {


}




