#ifndef CPU_HPP
#define CPU_HPP

#include "Defs.hpp"

#include <cstdio>
#include <bitset>
#include <climits>
#include <unordered_map>

#include "Memory.hpp"

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

struct Overrides {
    ubyte regOverride = 0;
    ubyte repOverride = 0;
    ubyte lockOverride = 0;    
    
    unsigned int getOverrideCount() {
        unsigned int counter=0;
        if(regOverride != 0)  counter++;
        if(repOverride != 0)  counter++;
        if(lockOverride != 0) counter++;
        return counter; 
    }
   
    inline void reset() {
        regOverride = repOverride = lockOverride = 0; 
    }


    void print() {
        if(regOverride != 0)  printf("%s ", _fixes[regOverride].c_str());
        if(repOverride != 0)  printf("%s ", _fixes[repOverride].c_str());
        if(lockOverride != 0) printf("%s ", _fixes[lockOverride].c_str());
    }
    
    void setOverrides(unsigned int counter, uint8_t* memory) {    
        
        enum regOvrType { REGISTER_OVERRIDE, REP_OVERRIDE, LOCK_OVERRIDE, NOT_OVERRIDE };

        auto isOverrideOpcode = [this, &counter, memory]() {
            if(memory[counter] == CS_OVR || memory[counter] == DS_OVR ||
               memory[counter] == ES_OVR || memory[counter] == SS_OVR) 
                    return REGISTER_OVERRIDE;
            
            if(memory[counter] == REPZ_OVR || memory[counter] == REPNZ_OVR)
                    return REP_OVERRIDE;
           
            if(memory[counter] == LOCK_OVR)
                    return LOCK_OVERRIDE;
           
            return NOT_OVERRIDE;
        };
        
        regOvrType ovr;
        
        while( (ovr = isOverrideOpcode()) != NOT_OVERRIDE ) {        
            if(ovr == REGISTER_OVERRIDE) regOverride  = memory[counter];
            if(ovr == REP_OVERRIDE)      repOverride  = memory[counter];
            if(ovr == LOCK_OVERRIDE)     lockOverride = memory[counter];
            counter++;
        }

    }    
    
    std::unordered_map<ubyte, std::string> _fixes {
        {ES_OVR, "ES:"},
        {SS_OVR, "SS:"},
        {CS_OVR, "CS:"},
        {DS_OVR, "DS:"},
        {REPZ_OVR, "REPZ"},
        {REPNZ_OVR, "REPNZ"},
        {LOCK_OVR, "LOCK(NOT USED)"},
    };
};

struct ModRM {
    std::bitset<2> mode;         
    std::bitset<3> reg;          
    std::bitset<3> rm;         
    
    inline void reset() {
        mode = 0;
        reg = rm = 0;
    }

    inline void decode(ubyte _b) {
        mode = _b >> 6;
        reg = _b >> 3;
        rm = _b;
    }
   
    int getModInstrSize(int overrideCount) {

        switch(mode.to_ulong()) {
            // <op>, <mod_rm>  
            case REGISTER_MODE:
                return 2 + overrideCount;
            
            // special case in this mode
            // e.g mov [0xface], ax -> <op>, <mod>, <0xce>, <0xfa>
            case NO_DISPLACEMENT:
                if(rm == SWORD)
                    return 4 + overrideCount;
                else
                    return 2 + overrideCount;
            
            // op, <mod>, <dis_msb> 
            case BYTE_DISPLACEMENT:
                return 3 + overrideCount;
            
            // <op>, <mod>, <dis_lsb>, <dis_msb> 
            case WORD_DISPLACEMENT:
                return 4 + overrideCount;
            default:
                assert(false);
        }

    }
    void print() const {
        printf("mode:%s reg:%s rm:%s\n", mode.to_string().c_str(), reg.to_string().c_str(), rm.to_string().c_str());
    }
};


class CPU {

public:
    CPU(){}

    /* Interface */
    Memory* getMemory();

    void runTest();    
    void step();
   
    Registers getRegisters();

    void dumpMem() {
        printf("---------------------------\n");  
        printf("AX:%*x BX:%*x CX:%*x DX:%*x\n", 4, regs.ax, 4, regs.bx, 4, regs.cx, 4, regs.dx);
        printf("SI:%*x DI:%*x BP:%*x SP:%*x\n", 4, regs.si, 4, regs.di, 4, regs.bp, 4, regs.sp);
        printf("ES:%*x CS:%*x SS:%*x DS:%*x\n", 4, regs.es, 4, regs.cs, 4, regs.ss, 4, regs.ds);
        printf("IP:%*x\n", 4, regs.ip);
        printf("FLAGS:%s ", regs.flags.to_string().c_str()); 
        if(regs.flags[CF]) printf(" CF");
        if(regs.flags[PF]) printf(" PF");
        if(regs.flags[AF]) printf(" AF");
        if(regs.flags[ZF]) printf(" ZF");
        if(regs.flags[SF]) printf(" SF");
        if(regs.flags[OF]) printf(" OF");
        const int width = 23;
        int addr = 0;
        printf("\n      "); 
        for(int i=0;i<width;++i) {
            printf("\x1b[33m%*x ", 2, i); 
        }
        printf("\n");
        for(int i=0;i<8;++i) {
            printf("\x1b[31m%*x: ", 4, addr); 
            for(int j=0;j<width;++j) {
                printf("\x1b[32m%*x ", 2, memory[addr++]); 
            }
            printf("\x1b[0m\n"); 
        }
    }
    
//private:

    /* IP with added overrides */
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
    uword getFullInstructionSizeModRM();
    dword getAbsoluteAddressModRM();
   
    template<bool, bool>
    void setOperands();
    template<typename T, bool>
    void setFlags(T);

    /* Utils */
    inline void incIP(int Steps) {
        regs.ip += Steps; 
    }

    inline uword getAbs(uword Seg, uword Off) {
        return (Seg << 4) + Off; 
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
