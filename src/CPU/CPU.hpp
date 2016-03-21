#ifndef CPU_HPP
#define CPU_HPP

#include <unordered_map>

#include "Defs.hpp"
#include "ModRM.hpp"
#include "Memory.hpp"
#include "Overrides.hpp"
#include "Registers.hpp"

class CPU {

public:
    CPU(){}

    Memory* getMemory();
    Registers getRegisters();
    
    void step();

//private:


    dword IPReal = 0;

    ModRM     mod;
    Memory    memory; 
    Registers regs;
    Overrides ovrd;

    ubyte* lhs_b, *rhs_b;
    ubyte  lhs_buf_b, rhs_buf_b; 
    uword  tmp_w;

    uword* lhs_w, *rhs_w;
    uword  lhs_buf_w, rhs_buf_w;
    dword  tmp_d;
   

    void fetch();
    void execute();
    void setOverrides();

    uword getOverridedSegmentValue();
    dword getAbsoluteAddressModRM();

    
    uword* getStackTopPtr();


    template<typename T, unsigned int>
    inline void push(T);

    template<typename T, unsigned int>
    inline T pop();

    template<bool, bool>
    void setOperands();
    
    template<typename T, bool withCarry = true, bool isLogicOp = false>
    inline void flagCSZP(T);

    template<typename T>
    inline void flagLogic(T);

    void incIP(unsigned int);
    dword getAbs(uword, uword);

// ****************************** 

    inline void addEbGb();      // 0x00
    inline void addEvGv();      // 0x01
    inline void addGbEb();      // 0x02
    inline void addGvEv();      // 0x03
    inline void addAlIb();      // 0x04
    inline void addAxIv();      // 0x05

    inline void pushEs();       // 0x06
    inline void popEs();        // 0x07

    inline void orEbGb();       // 0x08
    inline void orEvGv();       // 0x09
    inline void orGbEb();       // 0x0A
    inline void orGvEv();       // 0x0B
    inline void orAlIb();       // 0x0C
    inline void orAxIv();       // 0x0D

    inline void pushCs();       // 0x0E

    inline void adcEbGb();      // 0x10
    inline void adcEvGv();      // 0x11
    inline void adcGbEb();      // 0x12
    inline void adcGvEv();      // 0x13
    inline void adcAlIb();      // 0x14
    inline void adcAxIv();      // 0x15

    inline void pushSs();       // 0x16
    inline void popSs();        // 0x17

    inline void sbbEbGb();      // 0x18
    inline void sbbEvGv();      // 0x19
    inline void sbbGbEb();      // 0x1A
    inline void sbbGvEv();      // 0x1B
    inline void sbbAlIb();      // 0x1C
    inline void sbbAxIv();      // 0x1D

    inline void pushDs();       // 0x1E
    inline void popDs();        // 0x1F
    
    inline void andEbGb();      // 0x20 
    inline void andEvGv();      // 0x21
    inline void andGbEb();      // 0x22
    inline void andGvEv();      // 0x23
    inline void andAlIb();      // 0x24
    inline void andAxIv();      // 0x25

    inline void daa();          // 0x27

    inline void subEbGb();      // 0x28
    inline void subEvGv();      // 0x29
    inline void subGbEb();      // 0x2A
    inline void subGvEv();      // 0x2B
    inline void subAlIb();      // 0x2C
    inline void subAxIv();      // 0x2D

    inline void das();          // 0x2F

    inline void xorEbGb();      // 0x30
    inline void xorEvGv();      // 0x31
    inline void xorGbEb();      // 0x32
    inline void xorGvEv();      // 0x33
    inline void xorAlIb();      // 0x34
    inline void xorAxIv();      // 0x35

    inline void aaa();          // 0x37
    
    inline void cmpEbGb();      // 0x38
    inline void cmpEvGv();      // 0x39
    inline void cmpGbEb();      // 0x3A
    inline void cmpGvEv();      // 0x3B
    inline void cmpAlIb();      // 0x3C
    inline void cmpAxIv();      // 0x3D

// ****************************** 

    /* Lookups */
    std::unordered_map<int, ubyte*> byteRegs {
        {AL_AX, &regs.al},
        {CL_CX, &regs.cl},
        {DL_DX, &regs.dl},
        {BL_BX, &regs.bl},
        {AH_SP, &regs.ah},
        {CH_BP, &regs.ch},
        {DH_SI, &regs.dh},
        {BH_DI, &regs.bh},
    };

    std::unordered_map<int, uword*> wordRegs {
        {AL_AX, &regs.ax},
        {CL_CX, &regs.cx},
        {DL_DX, &regs.dx},
        {BL_BX, &regs.bx},
        {AH_SP, &regs.sp},
        {BP_SI, &regs.bp},
        {DH_SI, &regs.si},
        {BH_DI, &regs.di},
    };

    std::unordered_map<ubyte, uword*> segmentRegs {
        {ES, &regs.es},
        {CS, &regs.cs},
        {DS, &regs.ds},
        {SS, &regs.ss},
    };

};

/* Lookup */
static const ubyte parity[256] {
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1,
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
    1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0,
    0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 0, 1
};  

#endif
