#ifndef DISASM_HPP
#define DISASM_HPP

#include <string>
#include <vector>
#include <unordered_map>

#include "Opcodes.hpp"
#include "../CPU/ModRM.hpp"
#include "../CPU/Overrides.hpp"

#define BUF_LEN 32
#define GET_BYTE(x) (memory[position+x])
#define GET_WORD(x,y) ((memory[position+x]<<8)|memory[position+y])

static std::unordered_map<ubyte, std::unordered_map<ubyte, std::string>> mrm {

    { 
        NO_DISPLACEMENT,
        {
            {BX_SI, "[bx+si]"},
            {BX_DI, "[bx+di]"},
            {BP_SI, "[bp+si]"},
            {BP_DI, "[bp+di]"},
            {SI, "[si]"},
            {DI, "[di]"},
            {UWORD, "["},
            {BX, "[bx]"},
        }
    },
    { 
        BYTE_DISPLACEMENT,
        {
            {BX_SI, "[bx+si"},
            {BX_DI, "[bx+di"},
            {BP_SI, "[bp+si"},
            {BP_DI, "[bp+di"},
            {SI, "[si"},
            {DI, "[di"},
            {BP, "[bp"},
            {BX, "[bx"},
        }
    },
    { 
        WORD_DISPLACEMENT,
        {
            {BX_SI, "[bx+si"},
            {BX_DI, "[bx+di"},
            {BP_SI, "[bp+si"},
            {BP_DI, "[bp+di"},
            {SI, "[si"},
            {DI, "[di"},
            {BP, "[bp"},
            {BX, "[bx"},

        }
    }

};

static std::unordered_map<ubyte, std::unordered_map<uword, std::string>> regs {

    {
       0,
       {
           {AL_AX, "al"},
           {CL_CX, "cl"},
           {DL_DX, "dl"},
           {BL_BX, "bl"},
           {AH_SP, "ah"},
           {CH_BP, "ch"},
           {DH_SI, "dh"},
           {BH_DI, "bh"},
       }
    },
    {
        1,
        {
           {AL_AX, "ax"},
           {CL_CX, "cx"},
           {DL_DX, "dx"},
           {BL_BX, "bx"},
           {AH_SP, "sp"},
           {CH_BP, "bp"},
           {DH_SI, "si"},
           {BH_DI, "di"}, 
        }
    },
    {
        2,
        {
            {ES, "es"},
            {CS, "cs"},
            {DS, "ds"},
            {SS, "ss"},
        }
    }

};

static std::unordered_map<ubyte, std::string> regSeg {
       {ES, "es"},
       {CS, "cs"},
       {DS, "ds"},
       {SS, "ss"},
};

static std::unordered_map<ubyte, std::string> regsHelper {
    {REG_AL, "al"},
    {REG_CL, "cl"},
    {REG_DL, "dl"},
    {REG_BL, "bl"},
    {REG_AH, "ah"},
    {REG_CH, "ch"},
    {REG_DH, "dh"},
    {REG_BH, "bh"},
    {REG_AX, "ax"},
    {REG_CX, "cx"},
    {REG_DX, "dx"},
    {REG_BX, "bx"},
    {REG_SP, "sp"},
    {REG_BP, "bp"},
    {REG_SI, "si"},
    {REG_DI, "di"},
    {REG_SEG_CS, "cs"},
    {REG_SEG_DS, "ds"},
    {REG_SEG_SS, "ss"},
    {REG_SEG_ES, "es"},
};

struct InstrData {
    unsigned int position;
    unsigned int size;
    std::string instr;
};

class Disasm {

public:
    Disasm() {}
    void bindMemory(ubyte*);
    std::vector<InstrData> disasm(unsigned int, unsigned int);

private:
    unsigned int position = 0;
    ubyte* memory;
    ModRM mod;
    Overrides ovr;


    std::string disasm(unsigned int&);

    void disasmImm(bool, std::string&);
    void disasmJmp(bool, std::string&); 
    void disasmGrp(Opcode&, ubyte&, std::string&);

    void disasmModRM(bool, bool, bool, std::string&);
    void disasmModRMSeg(bool, std::string&);
    void disasmModRMImm(bool, bool, std::string&); 
    
    void disasmModRMOne(bool, Operand, std::string&); 
    void disasmRegImm(bool, Operand, std::string&);
   

    std::string getModRMDisplacement();
    std::pair<std::string, std::string> getOverrides();

};


#endif
