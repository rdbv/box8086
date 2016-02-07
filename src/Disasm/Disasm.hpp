#ifndef DISASM_HPP
#define DISASM_HPP


#include <string>
#include <vector>
#include <bitset>
#include <cstdint>
#include <unordered_map>

#include "../CPU/CPU.hpp"

static std::unordered_map<uint8_t, std::unordered_map<uint8_t, std::string>> mrm {
    { 
        NO_DISPLACEMENT,
        {
            {0b000, "[BX+SI]"},
            {0b001, "[BX+DI]"},
            {0b010, "[BP+SI]"},
            {0b011, "[BP+DI]"},
            {0b100, "[SI]"},
            {0b101, "[DI]"},
            {0b110, "["},
            {0b111, "[BX]"},
        }
    },
    { 
        BYTE_DISPLACEMENT,
        {
            {0b000, "[BX+SI"},
            {0b001, "[BX+DI"},
            {0b010, "[BP+SI"},
            {0b011, "[BP+DI"},
            {0b100, "[SI"},
            {0b101, "[DI"},
            {0b110, "[BP"},
            {0b111, "[BX"},
        }
    },
    { 
        WORD_DISPLACEMENT,
        {
            {0b000, "[BX+SI"},
            {0b001, "[BX+DI"},
            {0b010, "[BP+SI"},
            {0b011, "[BP+DI"},
            {0b100, "[SI"},
            {0b101, "[DI"},
            {0b110, "["},
            {0b111, "[BX"},

        }
    }
};

static std::unordered_map<bool, std::unordered_map<uint8_t, std::string>> regs {
    {
       false,
       {
           {0b000, "AL"},
           {0b001, "CL"},
           {0b010, "DL"},
           {0b011, "BL"},
           {0b100, "AH"},
           {0b101, "CH"},
           {0b110, "DH"},
           {0b111, "BH"},
       }
    },
    {
        true,
        {
           {0b000, "AX"},
           {0b001, "CX"},
           {0b010, "DX"},
           {0b011, "BX"},
           {0b100, "SP"},
           {0b101, "BP"},
           {0b110, "SI"},
           {0b111, "DI"}, 
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
