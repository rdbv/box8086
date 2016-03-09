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

    ubyte* lhs_b, *rhs_b, lhs_buf_b, rhs_buf_b; 
    uword* lhs_w, *rhs_w, lhs_buf_w, rhs_buf_w;
    
    uword  tmp_w;
    dword  tmp_d;

    void fetch();
    void execute();
    void setOverrides();

    uword getOverridedSegmentValue();
    dword getAbsoluteAddressModRM();

    uword* getStackTopPtr();

    template<bool, bool>
    void setOperands();
    
    template<typename T, bool, bool isLogic = false>
    void setFlags(T);

    template<typename T, bool, bool isLogic = false>
    void setFlagsStandard(T);
    
    /* Increment IP */
    inline void incIP(int steps) {
        regs.ip += steps; 
    }

    /* Generate psy. address */
    inline uword getAbs(uword seg, uword off) {
        return (seg << 4) + off; 
    }

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
