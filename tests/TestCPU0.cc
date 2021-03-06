#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE TestCPU0

#include <boost/test/unit_test.hpp>
#include "../src/CPU/CPU.hpp"

#define MEM(x) ( &(*x)[0] )

/* =============== Random Dump of some testing tests =============== */
// All tests here is veery basic tests, i'm still newbie in software testing :P


/* Test of getAbsoluteAddress() func
   don't tests segmentation or displacements */

BOOST_AUTO_TEST_CASE( getAbsAddrWithoutDisplacement ) {
    
    CPU cpu;
    
    /* ========= */
    cpu.mod.decode(BX_SI);
    cpu.regs.bx = 0xf33;
    cpu.regs.si = 0x33f;

    BOOST_REQUIRE_EQUAL( cpu.getAbsoluteAddressModRM(), 0xf33 + 0x33f );

    /* ========= */
    cpu.mod.decode(BX_DI);
    cpu.regs.bx = 0xfade;
    cpu.regs.di = 0xedaf;

    BOOST_REQUIRE_EQUAL( cpu.getAbsoluteAddressModRM(), (uword) (0xfade + 0xedaf) );
   
    /* ========= */
    cpu.mod.decode(BP_SI);
    cpu.regs.bp = 0x1234;
    cpu.regs.si = 0x4321;

    BOOST_REQUIRE_EQUAL( cpu.getAbsoluteAddressModRM(), (uword) (0x1234 + 0x4321) );

    /* ========= */
    cpu.mod.decode(BP_DI);
    cpu.regs.bp = 0x5134;
    cpu.regs.di = 0x2134;

    BOOST_REQUIRE_EQUAL( cpu.getAbsoluteAddressModRM(), (uword) (0x5134 + 0x2134) );

    /* ========= */
    cpu.mod.decode(SI);
    cpu.regs.si = 0x1234;

    BOOST_REQUIRE_EQUAL( cpu.getAbsoluteAddressModRM(), (uword) (0x1234) );

    /* ========= */
    cpu.mod.decode(DI);
    cpu.regs.di = 0x4321;

    BOOST_REQUIRE_EQUAL( cpu.getAbsoluteAddressModRM(), (uword) (0x4321) );
    
    /* ========= */
    cpu.mod.decode(BX);
    cpu.regs.bx = 0xffff;

    BOOST_REQUIRE_EQUAL( cpu.getAbsoluteAddressModRM(), (uword) (0xffff) );

    /* ========= */
    Memory *mem = cpu.getMemory();
    ubyte code[] = {
        0x00, 0x16, 0xce, 0xfa, // add [0xface], dl
    }; 
    memcpy(MEM(mem), code, sizeof(code) );

    cpu.mod.decode(UWORD);
    BOOST_REQUIRE_EQUAL( cpu.getAbsoluteAddressModRM(), (uword) 0xface );

}

/* Now test with displacements */

BOOST_AUTO_TEST_CASE( getAbsAddrWithDisplacement0 ) {

    uint32_t absAddr;
    CPU cpu;
    Memory *mem = cpu.getMemory();
    ubyte code[] = {
        0x00, 0x47, 0x13,       // add [bx+0x13], al 
        0x00, 0x87, 0x24, 0x13, // add [bx+0x1324], al
        0x00, 0x47, 0xed,       // add [bx-0x13], al
        0x00, 0x87, 0xdc, 0xec, // add [bx-0x1324], al
        0x00, 0x40, 0x36,       // add [bx+si+0x36], al
        0x00, 0x40, 0xca,       // add [bx+si-0x36], al 
    }; 
    memcpy(MEM(mem), code, sizeof(code) );

    /* ========= */
    cpu.mod.decode(0x47);
    cpu.regs.bx = 0x1234;
    
    absAddr = 0x1234 + 0x13;
    BOOST_REQUIRE_EQUAL( cpu.getAbsoluteAddressModRM(), absAddr );

    /* ========= */
    cpu.IPReal += 3;
    cpu.mod.decode(0x87);
    cpu.regs.bx = 0x3333;

    absAddr = 0x3333 + 0x1324;
    BOOST_REQUIRE_EQUAL( cpu.getAbsoluteAddressModRM(), absAddr );

    /* ========= */
    cpu.IPReal += 4;
    cpu.mod.decode(0x47);
    cpu.regs.bx = 0x6666;
    
    absAddr = 0x6666 - 0x13;
    BOOST_REQUIRE_EQUAL( cpu.getAbsoluteAddressModRM(), absAddr );

    /* ========= */
    cpu.IPReal += 3;
    cpu.mod.decode(0x87);
    cpu.regs.bx = 0x5555;

    absAddr = 0x5555 - 0x1324;
    BOOST_REQUIRE_EQUAL( cpu.getAbsoluteAddressModRM(), absAddr );

    /* ========= */
    cpu.IPReal += 4;
    cpu.mod.decode(0x40);
    cpu.regs.bx = 0x1234;
    cpu.regs.si = 0x4444;
    
    absAddr = 0x1234 + 0x4444 + 0x36;
    BOOST_REQUIRE_EQUAL( cpu.getAbsoluteAddressModRM(), absAddr );

    /* ========= */
    cpu.IPReal += 3;
    cpu.mod.decode(0x40);
    cpu.regs.bx = 0x4444;
    cpu.regs.si = 0xface;

    absAddr = ((uword)(0x4444 + 0xface)) - 0x36;

    BOOST_REQUIRE_EQUAL( cpu.getAbsoluteAddressModRM(), absAddr );


}

/* Now with displacements, segements, and some quirks (memory wrapping) */

BOOST_AUTO_TEST_CASE( getAbsAddrSegments0 ) {


    CPU cpu;
    uint32_t absAddr;

    /* If BP in ModRM segment always == SS */
    cpu.mod.decode(BP_SI);
    cpu.regs.ss = 0xffff;
    cpu.regs.bp = 0x10;
    cpu.regs.si = 0x0;

    /* Memory wrapping */
    absAddr = ((0xffff<<4) + 0x10) % DEFAULT_MEMORY_SIZE;
    BOOST_REQUIRE_EQUAL( cpu.getAbsoluteAddressModRM(), absAddr );



}

BOOST_AUTO_TEST_CASE( setOperands0 ) {

    uint32_t absAddr = 0;

    CPU cpu;
    Memory *mem = cpu.getMemory();
    ubyte code[] = {
        0x89, 0x1e, 0xde, 0xfa,       // mov ds:0xfade, ax        
        0x36, 0x89, 0x16, 0xac, 0x03, // mov ss:0x3ac, dx 
        0x26, 0x89, 0x0f,             // mov es:[bx], cx
        0x26, 0x89, 0x10,             // mov es:[bx+si], dx
        0x26, 0x88, 0x14,             // mov es:[si], dl

    }; 
    memcpy(MEM(mem), code, sizeof(code) );

    cpu.mod.decode(0x1e);
    cpu.setOperands<false, true>();

    absAddr = 0xfade;
    BOOST_REQUIRE_EQUAL( cpu.lhs_w, (uword*) &cpu.memory[absAddr] );
    BOOST_REQUIRE_EQUAL( cpu.rhs_w, &cpu.regs.bx );

    cpu.regs.ip += 5;
    cpu.IPReal += 5;

    cpu.regs.ss = 0x1234;
    cpu.ovrd.regOverride = SS_OVR;
    cpu.mod.decode(0x16);
    cpu.setOperands<false, true>();

    absAddr = (0x1234 << 4) + 0x3ac;
    BOOST_REQUIRE_EQUAL( cpu.lhs_w, (uword*) &cpu.memory[absAddr] );
    BOOST_REQUIRE_EQUAL( cpu.rhs_w, &cpu.regs.dx );

    cpu.regs.ip += 6;
    cpu.IPReal += 6;

    cpu.regs.es = 0x4321;
    cpu.regs.bx = 0x1234;
    cpu.ovrd.regOverride = ES_OVR;
    cpu.mod.decode(0xf);
    cpu.setOperands<false, true>();

    absAddr = (0x4321<<4) + 0x1234;
    BOOST_REQUIRE_EQUAL( cpu.lhs_w, (uword*) &cpu.memory[absAddr] );
    BOOST_REQUIRE_EQUAL( cpu.rhs_w, &cpu.regs.cx );

    cpu.regs.ip += 4;
    cpu.IPReal += 4;

    cpu.regs.es = 0x031c;
    cpu.regs.bx = 0x1234;
    cpu.regs.si = 0x2345;
    cpu.ovrd.regOverride = ES_OVR;
    cpu.mod.decode(0x10);
    cpu.setOperands<false, true>();

    absAddr = (0x31c<<4) + (0x1234 + 0x2345);
    BOOST_REQUIRE_EQUAL( cpu.lhs_w, (uword*) &cpu.memory[absAddr] );
    BOOST_REQUIRE_EQUAL( cpu.rhs_w, &cpu.regs.dx );


}

