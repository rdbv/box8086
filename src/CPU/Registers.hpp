#ifndef REGISTERS_HPP
#define REGISTERS_HPP

#include "Defs.hpp"

struct Registers {

    union {
        uword regs16[13] = {0};
        struct {
            uword ax, bx, cx, dx, cs, ds, ss, es, bp, sp, di, si, ip; 
        };
        ubyte regs8[8];
        struct {
            ubyte al, ah;
            ubyte bl, bh;
            ubyte cl, ch;
            ubyte dl, dh;
        };
    };
    std::bitset<16> flags;

};  

#endif
