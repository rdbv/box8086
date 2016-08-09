#include "Disasm.hpp" 

/* Bind memory with code */

void Disasm::bindMemory(ubyte* mem) {
    assert(mem != nullptr);
    memory = mem;
}

/* 
 * Set beginning address for disassembly,
 * Set count of instructions for disassembly
 */

std::vector<InstrData> Disasm::disasm(unsigned int absAddr, unsigned int count) {
    assert(absAddr < DEFAULT_MEMORY_SIZE);

    std::vector<InstrData> instr;
    position = absAddr;

    unsigned int pos = 0;
    unsigned int instrSize = 0;
    Overrides ov;
    for(unsigned int i=0;i<count;++i) {
        pos = position; 
        std::string in = disasm(instrSize, ov);
        instr.push_back(InstrData{pos, instrSize, in, ov});
    }

    return instr;
}

/* 
 * Main disassembly function, identify 
 * instruction, and disasm 
 */

std::string Disasm::disasm(unsigned int& instrSize, Overrides& ov) {
    std::string inst;

    /* Reset overrides */
    ovr.clear();

    /* Fetch overrides */
    ovr.setOverrides(position, memory);

    /* Skip overrides */ 
    position += ovr.getOverrideCount();

    /* Fetch byte from memory */
    ubyte opByte = memory[position];

    /* Get opcode data from lookup */ 
    Opcode opData = opcodes[opByte];

    /* 
     * If is not group encoded instruction,
     * then add instruction mnemonic (add, sub, jmp) to
     * instr string
     */
    if(opData.enc != GRP_ENC)
       inst += opData.instr;

    instrSize = position;
    ov = ovr;

    //printf("EncType:%s byte:%x\n", encode_debug[opData.enc].c_str(), opByte);

    /* Decode */
    switch(opData.enc) {
        
        /* 0 Arguments, only opcode */
        case REG_REG_ENC:
        case ONE_BYTE_ENC:
            disasmOneByte(inst);
            position++;
            break;

        case IMM_ENC:
            disasmImm(opData.lhs == IMM_IV, inst);
            position += opData.lhs == IMM_IV?3:2;
            break;

        case MODRM_ENC:
        case MODRM_MEM_ENC:
            mod.decode(memory[position+1]);
            disasmModRM(TO_REGISTER(opByte), IS_WORD(opByte), opData.enc == MODRM_MEM_ENC, inst);
            position += mod.getModInstrSize(0);
            break;
        
        case MODRM_SEG_ENC:
            mod.decode(memory[position+1]);
            disasmModRMSeg(TO_REGISTER(opByte), inst);
            position += mod.getModInstrSize(0);
            break;
        
        case REG_IMM_ENC:
            disasmRegImm(opData.rhs == IMM_IV, opData.lhs, inst);
            position += opData.rhs == IMM_IV?3:2;
            break;

        case JMP_ENC:
            disasmJmp(opData.lhs == IMM_IV, inst);
            position += opData.lhs == IMM_IV?3:2;
            break;

        case MODRM_IMM_ENC:
            mod.decode(memory[position+1]);
            disasmModRMImm(IS_WORD(opByte), false, inst);
            position += mod.getModInstrSize(IS_WORD(opByte)?2:1);
            break;

        case GRP_ENC:
            mod.decode(memory[position+1]);
            disasmGrp(opData, opByte, inst);
            break;

        case INVALID_ENC:
            position += opData.fullSize;
            break;

        case RAW_SEG_RAW_OFF_ENC:
            disasmRawSegRawOff(inst);
            position += opData.fullSize;
            break;

        case AXAL_SEG_OFF_ENC:
            disasmAxAlSegOff(IS_WORD(opByte), TO_REGISTER(opByte), inst);
            position += opData.fullSize;
            break;

        case MODRM_ONE_ARG:
        case MODRM_ARG_ONE:
            assert(false);
        
    }

    /* Compute instruction size */
    instrSize -= position;
    instrSize = -instrSize;
    instrSize += ovr.getOverrideCount();
    
    return inst;
}

/*
 * Just add override prefixes
 */

void Disasm::disasmOneByte(std::string& instr) {
    auto overrides = getOverrides();
    instr = overrides.first + instr; 
}

/* 
 * Disasm immediate instruction
 * ex. int 0x30
 */

void Disasm::disasmImm(bool isWord, std::string& instr) {
    char buf[BUF_LEN];
    auto overrides = getOverrides();

    if(!isWord) 
        snprintf(buf, BUF_LEN, "%#x", GET_BYTE(1) );
    else 
        snprintf(buf, BUF_LEN, "%#x", GET_WORD(2, 1) );

    instr += " " + std::string(buf);
    instr = overrides.first + overrides.second + instr;
}

/* 
 * Disasm jmp instruction
 * ex. jmp <label>
 * is needed to calculate absolute address by
 * absolute_value = (signed) value + byteNo + jmp_instr_size
 * abs_val = overflowed value 
 */

void Disasm::disasmJmp(bool isWord, std::string& instr) {
    char buf[BUF_LEN];

    if(!isWord) {
        sword rel = GET_BYTE(1) + position + 0x2;
        if(rel < 0) {
        }
        snprintf(buf, BUF_LEN, "%#x", (uword) rel);
    }
    else {
        sword rel = GET_WORD(2, 1) + position + 0x3;
        snprintf(buf, BUF_LEN, "%#x", (uword) rel);
    }
    
    instr += " " + std::string(buf);
}


void Disasm::disasmGrp(Opcode& op, ubyte& opbyte, std::string& instr) {
    Opcode _op = grp[op.group][mod.reg.to_ulong()];
   
    instr += _op.instr;

    if(_op.enc == MODRM_IMM_ENC) {
        //printf("is_word:%d rhs:%d\n", IS_WORD(opbyte), IMM_IV);
        disasmModRMImm(IS_WORD(opbyte), op.rhs == IMM_IV, instr);
        position += mod.getModInstrSize(op.rhs == IMM_IV?2:1);
        return;
    }
    else if(_op.enc == MODRM_ARG_ONE || _op.enc == MODRM_ONE_ARG) {
        disasmModRMOne(IS_WORD(opbyte), op.rhs, instr);
        position += mod.getModInstrSize(0); 
        return;
    }

    printf("COP:%x pos:%#x\n", opbyte, position);

    /* Unhandled group instructions */
    assert(false);
}

void Disasm::disasmModRM(bool toRegister, bool isWord, bool isMemOperation, std::string& instr) {
    std::string lhs, rhs; 
    auto overrides = getOverrides();
   
    /* From register to regsiter */
    if(mod.mode == REGISTER_MODE) {
        if(!toRegister) {
            lhs = regs[isWord][mod.rm.to_ulong()];
            rhs = regs[isWord][mod.reg.to_ulong()];
        }
        else {
            lhs = regs[isWord][mod.reg.to_ulong()];
            rhs = regs[isWord][mod.rm.to_ulong()];
        }
        instr = overrides.first + instr + " " + lhs + ", " + rhs; 
        return; 
    }

    
    bool memSideLeft = false;

    // lea ax, [bx] <- Mem operation, only to register
    if(toRegister || isMemOperation) {
        lhs = regs[isWord][mod.reg.to_ulong()];
        rhs = mrm[mod.mode.to_ulong()][mod.rm.to_ulong()];
    }
    else {
        lhs = mrm[mod.mode.to_ulong()][mod.rm.to_ulong()];
        rhs = regs[isWord][mod.reg.to_ulong()];
        memSideLeft = true;
    }


    if(!memSideLeft) 
        instr = overrides.first + instr + " " + lhs + ", " + 
                overrides.second + rhs + getModRMDisplacement(); 
    else 
        instr = overrides.first + instr + " " + 
                overrides.second + lhs + getModRMDisplacement() + ", " + rhs;  
    
}

void Disasm::disasmModRMSeg(bool toSegmentRegister, std::string& instr) {
    std::string lhs, rhs;
    auto overrides = getOverrides();
    
    if(mod.mode == REGISTER_MODE) {
        if(!toSegmentRegister) {
            lhs = regs[true][mod.rm.to_ulong()];
            rhs = regSeg[mod.reg.to_ulong()];
        }
        else {
            lhs = regSeg[mod.reg.to_ulong()];
            rhs = regs[true][mod.rm.to_ulong()];
        }
        instr = overrides.first + instr + " " + lhs + ", " + rhs; 
        return; 
    }
  
    bool memSideLeft = false;

    if(toSegmentRegister) {
        lhs = regSeg[mod.reg.to_ulong()];
        rhs = mrm[mod.mode.to_ulong()][mod.rm.to_ulong()]; 
    }
    else {
        lhs = mrm[mod.mode.to_ulong()][mod.rm.to_ulong()];
        rhs = regSeg[mod.reg.to_ulong()]; 
        memSideLeft = true; 
    }
   
    if(!memSideLeft) 
        instr = overrides.first + instr + " " + lhs + ", " + 
                overrides.second + rhs + getModRMDisplacement(); 
    else 
        instr = overrides.first + instr + " " + 
                overrides.second + lhs + getModRMDisplacement() + ", " + rhs;  
}

void Disasm::disasmModRMImm(bool isWord, bool isImmWord, std::string& instr) {
    char buf[BUF_LEN] = {0};
    
    std::string lhs;
    auto overrides = getOverrides();

    if(!isWord || !isImmWord) 
        snprintf(buf, BUF_LEN, "%#x", memory[position+mod.getDisplacementSize(1)+1]);
    else if(isWord || isImmWord) {
        uword imm = memory[position+mod.getDisplacementSize(1)+2] << 8 | 
                    memory[position+mod.getDisplacementSize(1)+1];
        snprintf(buf, BUF_LEN, "%#x", imm);
    }

    if(mod.mode != REGISTER_MODE)
        lhs = mrm[mod.mode.to_ulong()][mod.rm.to_ulong()];
    else
        lhs = regs[isWord][mod.rm.to_ulong()];

    instr = overrides.first + instr + " " + 
            overrides.second + lhs + getModRMDisplacement() + ", " + buf;
}

/*
 * ex. mov ax, <byte>/<word>
 *     xor al, <byte>/<word> ... 
 *     on ax/al all operations (xor,add,sub,mov) on other regs only mov)
 */

void Disasm::disasmRegImm(bool isWord, Operand lhs, std::string& instr) {
    char buf[BUF_LEN] = {0};
    auto overrides = getOverrides();

    if(!isWord) 
        snprintf(buf, BUF_LEN, "%#x", GET_BYTE(1) );
    else 
        snprintf(buf, BUF_LEN, "%#x", GET_WORD(2, 1) );

    instr += " " + regsHelper[lhs] + ", " + std::string(buf);
    instr = overrides.first + instr;
}

void Disasm::disasmModRMOne(bool isWord, Operand rhsSelector, std::string& instr) {
    std::string lhs, rhs;

    auto overrides = getOverrides();

    if(mod.mode == REGISTER_MODE)
        lhs = regs[isWord][mod.rm.to_ulong()];
    else
        lhs = mrm[mod.mode.to_ulong()][mod.rm.to_ulong()];

    
    if(rhsSelector == IMM_ONE)
        rhs = ", 1";
    
    if(rhsSelector == REG_CL)
        rhs = ", cl";

    instr = overrides.first + instr + " " + overrides.second + lhs + getModRMDisplacement() + rhs;
}

void Disasm::disasmRawSegRawOff(std::string& instr) {
    std::string lhs;
    char buf[BUF_LEN] = {0};

    auto overrides = getOverrides();

    uword segment, offset;
    segment = memory[position+4] << 8 | memory[position+3];
    offset  = memory[position+2] << 8 | memory[position+1];
    
    snprintf(buf, BUF_LEN, "%#x:%#x", segment, offset);
    
    instr += " " + std::string(buf);
    instr = overrides.first + instr;
}

/*
 * a0, a1
 * mov ax/al, <addr_word>
 * mov <addr_word>, ax, al
 */

void Disasm::disasmAxAlSegOff(bool isWord, bool toRegister,  std::string& instr) {
    char buf[BUF_LEN] = {0};
    auto overrides = getOverrides();

    uword offset;
    offset = memory[position+2] << 8 | memory[position+1];

    snprintf(buf, BUF_LEN, "[%#x]", offset);

    Operand reg = (isWord? REG_AX : REG_AL);

    if(toRegister)  
         instr += " " + regsHelper[reg] + ", " + overrides.second + std::string(buf);
    else 
         instr += " " + overrides.second + std::string(buf) + ", " + regsHelper[reg]; 

    instr = overrides.first + instr;
}

std::string Disasm::getModRMDisplacement() {
    char buf[BUF_LEN] = {0};

    /* 
     * Special case,
     * ex. mov [0x1234], cx
     */
    
    if(mod.mode == NO_DISPLACEMENT && mod.rm == UWORD) {
        uint16_t value = 0; 
        value = GET_WORD(3, 2);     
        snprintf(buf, BUF_LEN, "%#x]", value); 
        return buf;
    }

    if(mod.mode == BYTE_DISPLACEMENT) {
        int8_t value = GET_BYTE(2);
        snprintf(buf, BUF_LEN, "%c%#x]", (value<0)?'-':'+', value<0?-value:value);
    }

    if(mod.mode == WORD_DISPLACEMENT) {
        int16_t value = GET_WORD(3, 2); 
        snprintf(buf, BUF_LEN, "%c%#x]", (value<0)?'-':'+', value<0?-value:value);
    }

    return buf;
}

/*
 * Return pair with strings overrides
 * REPNZ/REPZ or ES/SS/CS/DS
 * LOCK is don't used here
 */

std::pair<std::string, std::string> Disasm::getOverrides() {
    std::string overridesBegin, overridesEnd;
    if(ovr.repOverride != 0) overridesBegin = opcodes[ovr.repOverride].instr; 
    if(ovr.regOverride != 0) overridesEnd = opcodes[ovr.regOverride].instr;
    return std::pair<std::string, std::string>(overridesBegin, overridesEnd);
}

