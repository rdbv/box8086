#ifndef OPCODES_HPP
#define OPCODES_HPP

#include "../CPU/Defs.hpp"
#include <string>

#define GRP_NONE 0
#define GRP1     1
#define GRP2     2
#define GRP3a    0x3A
#define GRP3b    0x3B
#define GRP4     4
#define GRP5     5

#define RUNTIME_DEFINED -1

#define TO_REGISTER(x) ( x & 2 )
#define IS_WORD(x)     ( x & 1 )

enum Operand {
    /* Immediates */
    IMM_IB, IMM_IV,                 
    IMM_ONE, 
    
    /* Regs */
    REG_AL,REG_CL,
    REG_DL,REG_BL,
    REG_AH,REG_CH,
    REG_DH,REG_BH,
    REG_AX,REG_CX,
    REG_DX,REG_BX,
    REG_SP,REG_BP,
    REG_SI,REG_DI,

    REG_SEG_CS,REG_SEG_DS,
    REG_SEG_SS,REG_SEG_ES,                          
    
    /* None */
    NONE
};

enum EncodeType{
    IMM_ENC,
    JMP_ENC,
    GRP_ENC,
    MODRM_ENC,               
    REG_REG_ENC,           
    REG_IMM_ENC,           
    INVALID_ENC,            
    ONE_BYTE_ENC,          
    MODRM_MEM_ENC,         
    MODRM_SEG_ENC,         
    MODRM_IMM_ENC,         
    MODRM_ARG_ONE,         
    MODRM_ONE_ARG,         
    AXAL_SEG_OFF_ENC,      
    RAW_SEG_RAW_OFF_ENC,     
};

struct Opcode {
    std::string instr;
    Operand lhs, rhs;
    EncodeType enc;
    int group, fullSize;
};

static Opcode opcodes[256] = {
    Opcode{"add", NONE, NONE, MODRM_ENC, GRP_NONE, RUNTIME_DEFINED},
    Opcode{"add", NONE, NONE, MODRM_ENC, GRP_NONE, RUNTIME_DEFINED},
    Opcode{"add", NONE, NONE, MODRM_ENC, GRP_NONE, RUNTIME_DEFINED},
    Opcode{"add", NONE, NONE, MODRM_ENC, GRP_NONE, RUNTIME_DEFINED},
    Opcode{"add", REG_AL, IMM_IB, REG_IMM_ENC, GRP_NONE, 2} ,
    Opcode{"add", REG_AX, IMM_IV, REG_IMM_ENC, GRP_NONE, 3} ,
    Opcode{"push es", REG_SEG_ES, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"pop es", REG_SEG_ES, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"or",  NONE, NONE, MODRM_ENC, GRP_NONE, RUNTIME_DEFINED} ,
    Opcode{"or",  NONE, NONE, MODRM_ENC, GRP_NONE, RUNTIME_DEFINED} ,
    Opcode{"or",  NONE, NONE, MODRM_ENC, GRP_NONE, RUNTIME_DEFINED} ,
    Opcode{"or",  NONE, NONE, MODRM_ENC, GRP_NONE, RUNTIME_DEFINED} ,
    Opcode{"or",  REG_AL, IMM_IB, REG_IMM_ENC, GRP_NONE, 2} ,
    Opcode{"or",  REG_AX, IMM_IV, REG_IMM_ENC, GRP_NONE, 3} ,
    Opcode{"push",  REG_SEG_CS, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"invalid", NONE, NONE, INVALID_ENC, GRP_NONE, 1} ,             
    Opcode{"adc", NONE, NONE, MODRM_ENC, GRP_NONE, RUNTIME_DEFINED} ,
    Opcode{"adc", NONE, NONE, MODRM_ENC, GRP_NONE, RUNTIME_DEFINED} ,
    Opcode{"adc", NONE, NONE, MODRM_ENC, GRP_NONE, RUNTIME_DEFINED} ,
    Opcode{"adc", NONE, NONE, MODRM_ENC, GRP_NONE, RUNTIME_DEFINED} ,
    Opcode{"adc", REG_AL, IMM_IB, REG_IMM_ENC, GRP_NONE, 2} ,
    Opcode{"adc", REG_AX, IMM_IV, REG_IMM_ENC, GRP_NONE, 3} ,
    Opcode{"push ss", REG_SEG_SS, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"pop ss", REG_SEG_SS, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"sbb", NONE, NONE, MODRM_ENC, GRP_NONE, RUNTIME_DEFINED} ,
    Opcode{"sbb", NONE, NONE, MODRM_ENC, GRP_NONE, RUNTIME_DEFINED} ,
    Opcode{"sbb", NONE, NONE, MODRM_ENC, GRP_NONE, RUNTIME_DEFINED} ,
    Opcode{"sbb", NONE, NONE, MODRM_ENC, GRP_NONE, RUNTIME_DEFINED} ,
    Opcode{"sbb", REG_AL, IMM_IB, REG_IMM_ENC, GRP_NONE, 2} ,
    Opcode{"sbb", REG_AX, IMM_IV, REG_IMM_ENC, GRP_NONE, 3} ,
    Opcode{"push ds", REG_SEG_DS, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"pop ds", REG_SEG_DS, NONE,  ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"and", NONE, NONE, MODRM_ENC, GRP_NONE, RUNTIME_DEFINED} ,
    Opcode{"and", NONE, NONE, MODRM_ENC, GRP_NONE, RUNTIME_DEFINED} ,
    Opcode{"and", NONE, NONE, MODRM_ENC, GRP_NONE, RUNTIME_DEFINED} ,
    Opcode{"and", NONE, NONE, MODRM_ENC, GRP_NONE, RUNTIME_DEFINED} ,
    Opcode{"and", REG_AL, IMM_IB, REG_IMM_ENC, GRP_NONE, 2} ,
    Opcode{"and", REG_AX, IMM_IV, REG_IMM_ENC, GRP_NONE, 3} ,
    Opcode{"es:", NONE, NONE, INVALID_ENC, GRP_NONE, 1} ,
    Opcode{"daa", NONE, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"sub", NONE, NONE, MODRM_ENC, GRP_NONE, RUNTIME_DEFINED} ,
    Opcode{"sub", NONE, NONE, MODRM_ENC, GRP_NONE, RUNTIME_DEFINED} ,
    Opcode{"sub", NONE, NONE, MODRM_ENC, GRP_NONE, RUNTIME_DEFINED} ,
    Opcode{"sub", NONE, NONE, MODRM_ENC, GRP_NONE, RUNTIME_DEFINED} ,
    Opcode{"sub", REG_AL, IMM_IB, REG_IMM_ENC, GRP_NONE, 2} ,
    Opcode{"sub", REG_AX, IMM_IV, REG_IMM_ENC, GRP_NONE, 3} ,
    Opcode{"cs:", NONE, NONE, INVALID_ENC, GRP_NONE, 1} ,
    Opcode{"das", NONE, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,   
    Opcode{"xor", NONE, NONE, MODRM_ENC, GRP_NONE, RUNTIME_DEFINED} ,
    Opcode{"xor", NONE, NONE, MODRM_ENC, GRP_NONE, RUNTIME_DEFINED} ,
    Opcode{"xor", NONE, NONE, MODRM_ENC, GRP_NONE, RUNTIME_DEFINED} ,
    Opcode{"xor", NONE, NONE, MODRM_ENC, GRP_NONE, RUNTIME_DEFINED} ,
    Opcode{"xor", REG_AL, IMM_IB, REG_IMM_ENC, GRP_NONE, 2} ,
    Opcode{"xor", REG_AX, IMM_IV, REG_IMM_ENC, GRP_NONE, 3} ,
    Opcode{"ss:", NONE, NONE, INVALID_ENC, GRP_NONE, 1} ,
    Opcode{"aaa", NONE, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"cmp", NONE, NONE, MODRM_ENC, GRP_NONE, RUNTIME_DEFINED} ,
    Opcode{"cmp", NONE, NONE, MODRM_ENC, GRP_NONE, RUNTIME_DEFINED} ,
    Opcode{"cmp", NONE, NONE, MODRM_ENC, GRP_NONE, RUNTIME_DEFINED} ,
    Opcode{"cmp", NONE, NONE, MODRM_ENC, GRP_NONE, RUNTIME_DEFINED} ,
    Opcode{"cmp", REG_AL, IMM_IB, REG_IMM_ENC, GRP_NONE, 2} ,
    Opcode{"cmp", REG_AX, IMM_IV, REG_IMM_ENC, GRP_NONE, 3} ,
    Opcode{"ds:", NONE, NONE, INVALID_ENC, GRP_NONE, 1} ,
    Opcode{"aas",  NONE, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"inc ax",  REG_AX, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"inc cx", REG_CX, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"inc dx", REG_DX, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"inc bx", REG_BX, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"inc sp", REG_SP, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"inc bp", REG_BP, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"inc si", REG_SI, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"inc di", REG_DI, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"dec ax", REG_AX, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"dec cx", REG_CX, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"dec dx", REG_DX, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"dec bx", REG_BX, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"dec sp", REG_SP, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"dec bp", REG_BP, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"dec si", REG_SI, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"dec di", REG_DI, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"push ax", REG_AX, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"push cx", REG_CX, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"push dx", REG_DX, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"push bx", REG_BX, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"push sp", REG_SP, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"push bp", REG_BP, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"push si", REG_SI, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"push di", REG_DI, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"pop ax", REG_AX, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"pop cx", REG_CX, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"pop dx", REG_DX, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"pop bx", REG_BX, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"pop sp", REG_SP, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"pop bp", REG_BP, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"pop si", REG_SI, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"pop di", REG_DI, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"invalid", NONE, NONE, INVALID_ENC, GRP_NONE, 1} ,
    Opcode{"invalid", NONE, NONE, INVALID_ENC, GRP_NONE, 1} ,
    Opcode{"invalid", NONE, NONE, INVALID_ENC, GRP_NONE, 1} ,
    Opcode{"invalid", NONE, NONE, INVALID_ENC, GRP_NONE, 1} ,
    Opcode{"invalid", NONE, NONE, INVALID_ENC, GRP_NONE, 1} ,
    Opcode{"invalid", NONE, NONE, INVALID_ENC, GRP_NONE, 1} ,
    Opcode{"invalid", NONE, NONE, INVALID_ENC, GRP_NONE, 1} ,
    Opcode{"invalid", NONE, NONE, INVALID_ENC, GRP_NONE, 1} ,
    Opcode{"invalid", NONE, NONE, INVALID_ENC, GRP_NONE, 1} ,
    Opcode{"invalid", NONE, NONE, INVALID_ENC, GRP_NONE, 1} ,
    Opcode{"invalid", NONE, NONE, INVALID_ENC, GRP_NONE, 1} ,
    Opcode{"invalid", NONE, NONE, INVALID_ENC, GRP_NONE, 1} ,
    Opcode{"invalid", NONE, NONE, INVALID_ENC, GRP_NONE, 1} ,
    Opcode{"invalid", NONE, NONE, INVALID_ENC, GRP_NONE, 1} ,
    Opcode{"invalid", NONE, NONE, INVALID_ENC, GRP_NONE, 1} ,
    Opcode{"invalid", NONE, NONE, INVALID_ENC, GRP_NONE, 1} ,
    Opcode{"jo",  IMM_IB, NONE, JMP_ENC, GRP_NONE, 2} ,
    Opcode{"jno", IMM_IB, NONE, JMP_ENC, GRP_NONE, 2} ,
    Opcode{"jb",  IMM_IB, NONE, JMP_ENC, GRP_NONE, 2} ,
    Opcode{"jnb", IMM_IB, NONE, JMP_ENC, GRP_NONE, 2} ,
    Opcode{"jz",  IMM_IB, NONE, JMP_ENC, GRP_NONE, 2} ,
    Opcode{"jnz", IMM_IB, NONE, JMP_ENC, GRP_NONE, 2} ,
    Opcode{"jbe", IMM_IB, NONE, JMP_ENC, GRP_NONE, 2} ,
    Opcode{"ja",  IMM_IB, NONE, JMP_ENC, GRP_NONE, 2} ,
    Opcode{"js",  IMM_IB, NONE, JMP_ENC, GRP_NONE, 2} ,
    Opcode{"jns", IMM_IB, NONE, JMP_ENC, GRP_NONE, 2} ,
    Opcode{"jpe", IMM_IB, NONE, JMP_ENC, GRP_NONE, 2} ,
    Opcode{"jpo", IMM_IB, NONE, JMP_ENC, GRP_NONE, 2} ,
    Opcode{"jl",  IMM_IB, NONE, JMP_ENC, GRP_NONE, 2} ,
    Opcode{"jge", IMM_IB, NONE, JMP_ENC, GRP_NONE, 2} ,
    Opcode{"jle", IMM_IB, NONE, JMP_ENC, GRP_NONE, 2} ,
    Opcode{"jg",  IMM_IB, NONE, JMP_ENC, GRP_NONE, 2} ,
    Opcode{"grp1", NONE, IMM_IB, GRP_ENC, GRP1, RUNTIME_DEFINED} ,
    Opcode{"grp1", NONE, IMM_IV, GRP_ENC, GRP1, RUNTIME_DEFINED} ,
    Opcode{"grp1", NONE, NONE, GRP_ENC, GRP1, RUNTIME_DEFINED} ,
    Opcode{"grp1", NONE, IMM_IB, GRP_ENC, GRP1, RUNTIME_DEFINED} ,
    Opcode{"test", NONE, NONE, MODRM_ENC, GRP_NONE, RUNTIME_DEFINED} ,
    Opcode{"test", NONE, NONE, MODRM_ENC, GRP_NONE, RUNTIME_DEFINED} ,
    Opcode{"xchg",  NONE, NONE, MODRM_ENC, GRP_NONE, RUNTIME_DEFINED} ,
    Opcode{"xchg",  NONE, NONE, MODRM_ENC, GRP_NONE, RUNTIME_DEFINED} ,
    Opcode{"mov", NONE, NONE, MODRM_ENC, GRP_NONE, RUNTIME_DEFINED} ,
    Opcode{"mov", NONE, NONE, MODRM_ENC, GRP_NONE, RUNTIME_DEFINED} ,
    Opcode{"mov", NONE, NONE, MODRM_ENC, GRP_NONE, RUNTIME_DEFINED} ,
    Opcode{"mov", NONE, NONE, MODRM_ENC, GRP_NONE, RUNTIME_DEFINED} ,
    Opcode{"mov", NONE, NONE, MODRM_SEG_ENC, GRP_NONE, RUNTIME_DEFINED} ,              
    Opcode{"lea", NONE, NONE, MODRM_MEM_ENC, GRP_NONE, RUNTIME_DEFINED} ,              
    Opcode{"mov", NONE, NONE, MODRM_SEG_ENC, GRP_NONE, RUNTIME_DEFINED} ,             
    Opcode{"pop", NONE, NONE, MODRM_ENC, GRP_NONE, RUNTIME_DEFINED} ,
    Opcode{"nop", NONE, NONE, ONE_BYTE_ENC, ONE_BYTE_ENC, 1} ,
    Opcode{"xchg cx, ax", REG_CX, REG_AX, REG_REG_ENC, GRP_NONE, 1} ,
    Opcode{"xchg dx, ax", REG_DX, REG_AX, REG_REG_ENC, GRP_NONE, 1} ,
    Opcode{"xchg bx, ax", REG_BX, REG_AX, REG_REG_ENC, GRP_NONE, 1} ,
    Opcode{"xchg sp, ax", REG_SP, REG_AX, REG_REG_ENC, GRP_NONE, 1} ,
    Opcode{"xchg bp, ax", REG_BP, REG_AX, REG_REG_ENC, GRP_NONE, 1} ,
    Opcode{"xchg si, ax", REG_SI, REG_AX, REG_REG_ENC, GRP_NONE, 1} ,
    Opcode{"xchg di, ax", REG_DI, REG_AX, REG_REG_ENC, GRP_NONE, 1} ,
    Opcode{"cbw", NONE, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"cwd", NONE, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"call",  NONE, NONE, RAW_SEG_RAW_OFF_ENC, GRP_NONE, 5} ,           
    Opcode{"wait",  NONE, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"pushf", NONE, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"popf",  NONE, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"sahf",  NONE, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"lahf",  NONE, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"mov", REG_AL, NONE, AXAL_SEG_OFF_ENC, GRP_NONE, 3} ,           
    Opcode{"mov", REG_AX, NONE, AXAL_SEG_OFF_ENC, GRP_NONE, 3} ,           
    Opcode{"mov", NONE, REG_AL, AXAL_SEG_OFF_ENC, GRP_NONE, 3} ,           
    Opcode{"mov", NONE, REG_AX, AXAL_SEG_OFF_ENC, GRP_NONE, 3} ,
    Opcode{"movsb",  NONE, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"movsw",  NONE, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"cmpsb",  NONE, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"cmpsw",  NONE, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"test", REG_AL, IMM_IB, REG_IMM_ENC, GRP_NONE, 2} ,
    Opcode{"test", REG_AX, IMM_IV, REG_IMM_ENC, GRP_NONE, 3} ,
    Opcode{"stosb", NONE, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"stosw", NONE, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"lodsb", NONE, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"lodsw", NONE, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"scasb", NONE, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"scasw", NONE, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"mov", REG_AL, IMM_IB, REG_IMM_ENC, GRP_NONE, 2} ,
    Opcode{"mov", REG_CL, IMM_IB, REG_IMM_ENC, GRP_NONE, 2} ,
    Opcode{"mov", REG_DL, IMM_IB, REG_IMM_ENC, GRP_NONE, 2} ,
    Opcode{"mov", REG_BL, IMM_IB, REG_IMM_ENC, GRP_NONE, 2} ,
    Opcode{"mov", REG_AH, IMM_IB, REG_IMM_ENC, GRP_NONE, 2} ,
    Opcode{"mov", REG_CH, IMM_IB, REG_IMM_ENC, GRP_NONE, 2} ,
    Opcode{"mov", REG_DH, IMM_IB, REG_IMM_ENC, GRP_NONE, 2} ,
    Opcode{"mov", REG_BH, IMM_IB, REG_IMM_ENC, GRP_NONE, 2} ,
    Opcode{"mov", REG_AX, IMM_IV, REG_IMM_ENC, GRP_NONE, 3} ,
    Opcode{"mov", REG_CX, IMM_IV, REG_IMM_ENC, GRP_NONE, 3} ,
    Opcode{"mov", REG_DX, IMM_IV, REG_IMM_ENC, GRP_NONE, 3} ,
    Opcode{"mov", REG_BX, IMM_IV, REG_IMM_ENC, GRP_NONE, 3} ,
    Opcode{"mov", REG_SP, IMM_IV, REG_IMM_ENC, GRP_NONE, 3} ,
    Opcode{"mov", REG_BP, IMM_IV, REG_IMM_ENC, GRP_NONE, 3} ,
    Opcode{"mov", REG_SI, IMM_IV, REG_IMM_ENC, GRP_NONE, 3} ,
    Opcode{"mov", REG_DI, IMM_IV, REG_IMM_ENC, GRP_NONE, 3} ,
    Opcode{"invalid", NONE, NONE, INVALID_ENC, GRP_NONE, 1} ,
    Opcode{"invalid", NONE, NONE, INVALID_ENC, GRP_NONE, 1} ,
    Opcode{"ret", IMM_IV, NONE, IMM_ENC, IMM_ENC, 3} ,
    Opcode{"ret", NONE, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"les", NONE, NONE, MODRM_MEM_ENC, GRP_NONE, RUNTIME_DEFINED} ,
    Opcode{"lds", NONE, NONE, MODRM_MEM_ENC, GRP_NONE, RUNTIME_DEFINED} ,
    Opcode{"mov", NONE, NONE, MODRM_IMM_ENC, GRP_NONE, RUNTIME_DEFINED} ,
    Opcode{"mov", NONE, NONE, MODRM_IMM_ENC, GRP_NONE, RUNTIME_DEFINED} ,
    Opcode{"invalid", NONE, NONE, INVALID_ENC, GRP_NONE, 1} ,
    Opcode{"invalid", NONE, NONE, INVALID_ENC, GRP_NONE, 1} ,
    Opcode{"retf",  IMM_IV, NONE, IMM_ENC, GRP_NONE, 3} ,
    Opcode{"retf",  NONE, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"int3",  NONE, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"int", IMM_IB, NONE, IMM_ENC, GRP_NONE, 2} ,
    Opcode{"into",  NONE, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"iret",  NONE, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"grp2", NONE, IMM_ONE, GRP_ENC, GRP2, RUNTIME_DEFINED} ,
    Opcode{"grp2", NONE, IMM_ONE, GRP_ENC, GRP2, RUNTIME_DEFINED} ,
    Opcode{"grp2", NONE, REG_CL, GRP_ENC, GRP2, RUNTIME_DEFINED} ,
    Opcode{"grp2", NONE, REG_CL, GRP_ENC, GRP2, RUNTIME_DEFINED} ,
    Opcode{"aam", IMM_IB, NONE, IMM_ENC, GRP_NONE, 2} ,
    Opcode{"aad", IMM_IB, NONE, IMM_ENC, GRP_NONE, 2} ,
    Opcode{"invalid", NONE, NONE, INVALID_ENC, GRP_NONE, 1} ,
    Opcode{"xlat", NONE, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,
    Opcode{"invalid",  NONE, NONE, INVALID_ENC, GRP_NONE, 1} ,
    Opcode{"invalid",  NONE, NONE, INVALID_ENC, GRP_NONE, 1} ,
    Opcode{"invalid", NONE, NONE, INVALID_ENC, GRP_NONE, 1} ,
    Opcode{"invalid", NONE, NONE, INVALID_ENC, GRP_NONE, 1} ,
    Opcode{"invalid", NONE, NONE, INVALID_ENC, GRP_NONE, 1} ,
    Opcode{"invalid",  NONE, NONE, INVALID_ENC, GRP_NONE, 1} ,
    Opcode{"invalid",  NONE, NONE, INVALID_ENC, GRP_NONE, 1} ,
    Opcode{"invalid",  NONE, NONE, INVALID_ENC, GRP_NONE, 1} ,
    Opcode{"loopnz", IMM_IB, NONE, JMP_ENC, GRP_NONE, 2} ,
    Opcode{"loopz", IMM_IB, NONE, JMP_ENC, GRP_NONE, 2} ,
    Opcode{"loop", IMM_IB, NONE, JMP_ENC, GRP_NONE, 2} ,
    Opcode{"jcxz", IMM_IB, NONE, JMP_ENC, GRP_NONE, 2} ,
    Opcode{"in", REG_AL, IMM_IB, REG_IMM_ENC, GRP_NONE, 2} ,
    Opcode{"in", REG_AX, IMM_IB, REG_IMM_ENC, GRP_NONE, 2},
    Opcode{"out", IMM_IB, REG_AL, IMM_ENC, GRP_NONE, 2} ,
    Opcode{"out", IMM_IB, REG_AX, IMM_ENC, GRP_NONE, 2} ,
    Opcode{"call", IMM_IV, NONE, JMP_ENC, GRP_NONE, 3} ,
    Opcode{"jmp",  IMM_IV, NONE, JMP_ENC, GRP_NONE, 3} ,
    Opcode{"jmp",  NONE, NONE, RAW_SEG_RAW_OFF_ENC, GRP_NONE, 5} ,       
    Opcode{"jmp",  IMM_IB, NONE, JMP_ENC, GRP_NONE, 2} ,               
    Opcode{"in",  REG_AL, REG_DX, ONE_BYTE_ENC, GRP_NONE, 1} ,          
    Opcode{"in",  REG_AX, REG_DX, ONE_BYTE_ENC, GRP_NONE, 1} ,          
    Opcode{"out",  REG_DX, REG_AL, ONE_BYTE_ENC, GRP_NONE, 1} ,       
    Opcode{"out", REG_DX, REG_AX, ONE_BYTE_ENC, GRP_NONE, 1} ,  
    Opcode{"lock", NONE, NONE, INVALID_ENC, GRP_NONE, 1} ,         
    Opcode{"invalID", NONE, NONE, INVALID_ENC, GRP_NONE, 1} ,  
    Opcode{"repnz ",  NONE, NONE, INVALID_ENC, GRP_NONE, 1} ,       
    Opcode{"repz ",  NONE, NONE, INVALID_ENC, GRP_NONE, 1} ,         
    Opcode{"hlt", NONE, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,            
    Opcode{"cmc", NONE, NONE, ONE_BYTE_ENC, GRP_NONE, 1} ,            
    Opcode{"grp3a", NONE, NONE, GRP_ENC, GRP3a, RUNTIME_DEFINED} ,                   
    Opcode{"grp3b", NONE, NONE, GRP_ENC, GRP3b, RUNTIME_DEFINED} ,                   
    Opcode{"clc", NONE, NONE, ONE_BYTE_ENC, GRP_NONE,  1} ,            
    Opcode{"stc", NONE, NONE, ONE_BYTE_ENC, GRP_NONE,  1} ,            
    Opcode{"cli", NONE, NONE, ONE_BYTE_ENC, GRP_NONE,  1} ,            
    Opcode{"sti", NONE, NONE, ONE_BYTE_ENC, GRP_NONE,  1} ,            
    Opcode{"cld", NONE, NONE, ONE_BYTE_ENC, GRP_NONE,  1} ,            
    Opcode{"std", NONE, NONE, ONE_BYTE_ENC, GRP_NONE,  1} ,            
    Opcode{"grp4", NONE, NONE, GRP_ENC, GRP4, RUNTIME_DEFINED} ,                     
    Opcode{"grp5", NONE, NONE, GRP_ENC, GRP5, RUNTIME_DEFINED} ,       
};

static std::unordered_map<ubyte, std::unordered_map<ubyte, Opcode>> grp {
    
    {
        GRP1,
        {
            {0b000, Opcode{"add", NONE, NONE, MODRM_IMM_ENC}},
            {0b001, Opcode{"or", NONE, NONE, MODRM_IMM_ENC}},
            {0b010, Opcode{"adc", NONE, NONE, MODRM_IMM_ENC} },
            {0b011, Opcode{"sbb", NONE, NONE, MODRM_IMM_ENC} },
            {0b100, Opcode{"and", NONE, NONE, MODRM_IMM_ENC} },
            {0b101, Opcode{"sub", NONE, NONE, MODRM_IMM_ENC} },
            {0b110, Opcode{"xor", NONE, NONE, MODRM_IMM_ENC} },
            {0b111, Opcode{"cmp", NONE, NONE, MODRM_IMM_ENC} },
        }
    },
    {
        GRP2, 
        {
            {0b000, Opcode{"rol", NONE, NONE, MODRM_ARG_ONE} },
            {0b001, Opcode{"ror", NONE, NONE, MODRM_ARG_ONE} },
            {0b010, Opcode{"rcl", NONE, NONE, MODRM_ARG_ONE} },
            {0b011, Opcode{"rcr", NONE, NONE, MODRM_ARG_ONE} },
            {0b100, Opcode{"shl", NONE, NONE, MODRM_ARG_ONE} },
            {0b101, Opcode{"shr", NONE, NONE, MODRM_ARG_ONE} },
            {0b110, Opcode{"bad", NONE, NONE, INVALID_ENC} },
            {0b111, Opcode{"sar", NONE, NONE, MODRM_ARG_ONE} },
        } 
    },
    {
        GRP3a,
         {
            {0b000, Opcode{"test",NONE, IMM_IB, MODRM_IMM_ENC} },
            {0b001, Opcode{"bad", NONE, NONE, INVALID_ENC} },
            {0b010, Opcode{"not", NONE, NONE, MODRM_ONE_ARG} },
            {0b011, Opcode{"neg", NONE, NONE, MODRM_ONE_ARG} },
            {0b100, Opcode{"mul", NONE, NONE, MODRM_ONE_ARG} },
            {0b101, Opcode{"imul",NONE, NONE, MODRM_ONE_ARG} },
            {0b110, Opcode{"div", NONE, NONE, MODRM_ONE_ARG} },
            {0b111, Opcode{"idiv",NONE, NONE, MODRM_ONE_ARG} },
         } 
    },
    
    {
        GRP3b, 
        {
            {0b000, Opcode{"test",NONE, IMM_IV, MODRM_IMM_ENC} },
            {0b001, Opcode{"",    NONE, NONE, INVALID_ENC} },
            {0b010, Opcode{"not", NONE, NONE, MODRM_ONE_ARG} },
            {0b011, Opcode{"neg", NONE, NONE, MODRM_ONE_ARG} },
            {0b100, Opcode{"mul", NONE, NONE, MODRM_ONE_ARG} },
            {0b101, Opcode{"imul",NONE, NONE, MODRM_ONE_ARG} },
            {0b110, Opcode{"div", NONE, NONE, MODRM_ONE_ARG} },
            {0b111, Opcode{"idiv",NONE, NONE, MODRM_ONE_ARG} },
        } 
    },
    {
        GRP4, 
        {
            {0b000, Opcode{"inc", NONE, NONE, MODRM_ONE_ARG} },       
            {0b001, Opcode{"dec", NONE, NONE, MODRM_ONE_ARG} },       
            {0b010, Opcode{"bad", NONE, NONE, INVALID_ENC} },
            {0b011, Opcode{"bad", NONE, NONE, INVALID_ENC} },
            {0b100, Opcode{"bad", NONE, NONE, INVALID_ENC} },
            {0b101, Opcode{"bad", NONE, NONE, INVALID_ENC} },
            {0b110, Opcode{"bad", NONE, NONE, INVALID_ENC} },
            {0b111, Opcode{"bad", NONE, NONE, INVALID_ENC} },
        } 
    },
    {
        GRP5, 
        {
            {0b000, Opcode{"inc", NONE, NONE, MODRM_ONE_ARG} },         
            {0b001, Opcode{"dec", NONE, NONE, MODRM_ONE_ARG} },         
            {0b010, Opcode{"call",NONE, NONE, MODRM_ONE_ARG} },       
            {0b011, Opcode{"call",IMM_IB, NONE, INVALID_ENC} },       
            {0b100, Opcode{"jmp", NONE, NONE, MODRM_ONE_ARG} },
            {0b101, Opcode{"jmp", IMM_IB, NONE, INVALID_ENC} },        
            {0b110, Opcode{"push",NONE, NONE, MODRM_ONE_ARG} },
            {0b111, Opcode{"", NONE, NONE, INVALID_ENC} },
        } 
    }

};

#endif
