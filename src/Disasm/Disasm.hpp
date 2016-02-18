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

static std::unordered_map<ubyte, std::unordered_map<uword, std::string>> regs {

    {
       0,
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
        1,
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
    },
    {
        2,
        {
            {ES, "ES"},
            {CS, "CS"},
            {DS, "DS"},
            {SS, "SS"},
        }
    }

};

static std::unordered_map<ubyte, std::string> regSeg {
       {ES, "ES"},
       {CS, "CS"},
       {DS, "DS"},
       {SS, "SS"},
};

static std::unordered_map<ubyte, std::string> regsHelper {
    {REG_AL, "AL"},
    {REG_CL, "CL"},
    {REG_DL, "DL"},
    {REG_BL, "BL"},
    {REG_AH, "AH"},
    {REG_CH, "CH"},
    {REG_DH, "DH"},
    {REG_BH, "BH"},
    {REG_AX, "AX"},
    {REG_CX, "CX"},
    {REG_DX, "DX"},
    {REG_BX, "BX"},
    {REG_SP, "SP"},
    {REG_BP, "BP"},
    {REG_SI, "SI"},
    {REG_DI, "DI"},
    {REG_SEG_CS, "CS"},
    {REG_SEG_DS, "DS"},
    {REG_SEG_SS, "SS"},
    {REG_SEG_ES, "ES"},
};

class Disasm {

public:
    Disasm() {}
    void bindMemory(ubyte*);
    std::vector<std::string> disasm(unsigned int, unsigned int);

private:
    unsigned int position = 0;
    ubyte* memory;
    ModRM mod;
    Overrides ovr;


    std::string disasm();

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
