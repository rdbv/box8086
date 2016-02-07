#ifndef DISASM_HPP
#define DISASM_HPP


#include <string>
#include <vector>
#include <bitset>
#include <cstdint>
#include <unordered_map>

#include "../CPU/CPU.hpp"

typedef std::unordered_map<ubyte, std::unordered_map<ubyte, std::string>> ModRMStrings;
typedef std::unordered_map<bool, std::unordered_map<ubyte, std::string>> RegisterMapStrings;

static ModRMStrings mrm {
    { 
        NO_DISPLACEMENT,
        {
            {BX_SI, "[BX+SI]"},
            {BX_DI, "[BX+DI]"},
            {BP_SI, "[BP+SI]"},
            {BP_DI, "[BP+DI]"},
            {SI, "[SI]"},
            {DI, "[DI]"},
            {UWORD, "["},
            {BX, "[BX]"},
        }
    },
    { 
        BYTE_DISPLACEMENT,
        {
            {BX_SI, "[BX+SI"},
            {BX_DI, "[BX+DI"},
            {BP_SI, "[BP+SI"},
            {BP_DI, "[BP+DI"},
            {SI, "[SI"},
            {DI, "[DI"},
            {BP, "[BP"},
            {BX, "[BX"},
        }
    },
    { 
        WORD_DISPLACEMENT,
        {
            {BX_SI, "[BX+SI"},
            {BX_DI, "[BX+DI"},
            {BP_SI, "[BP+SI"},
            {BP_DI, "[BP+DI"},
            {SI, "[SI"},
            {DI, "[DI"},
            {BP, "[BP"},
            {BX, "[BX"},

        }
    }
};

static RegisterMapStrings regs {
    {
       false,
       {
           {AL_AX, "AL"},
           {CL_CX, "CL"},
           {DL_DX, "DL"},
           {BL_BX, "BL"},
           {AH_SP, "AH"},
           {CH_BP, "CH"},
           {DH_SI, "DH"},
           {BH_DI, "BH"},
       }
    },
    {
        true,
        {
           {AL_AX, "AX"},
           {CL_CX, "CX"},
           {DL_DX, "DX"},
           {BL_BX, "BX"},
           {AH_SP, "SP"},
           {CH_BP, "BP"},
           {DH_SI, "SI"},
           {BH_DI, "DI"}, 
        }
    }
};

// a, b
#define NORMAL_ARG 2
// a, imm
#define IMMEDIATE_ARG 3
// a
#define ARG_ONLY_LHS 4

struct InstrString {

    std::string instr;
    std::string lhs, rhs;
    std::string disp;

    char dispSign = 0;
    char argType = 0;

};

typedef std::vector<InstrString> Instructions;
class Disasm {

public:
    Disasm(){}

    void bindMem(uint8_t*);

    Instructions disasmCount(unsigned int, unsigned int);

private:
    int pos = 0;
    int skip = 0;
    uint8_t* mem;
   
    ModRM mod;
    Overrides ovrd;
    
    InstrString disasm();

    void setOverrides();

    void disasmFromModRM(bool, bool, InstrString&);

    void modRmOp(bool , bool , std::string, InstrString&);
    void alaxImm(std::string, int, InstrString&);

    inline int insByte() { 
        return skip + pos;
    }

};

#endif
