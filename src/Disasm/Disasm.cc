#include "Disasm.hpp" 

void Disasm::bindMemory(ubyte* mem) {
    assert(mem != nullptr);
    memory = mem;
}

std::vector<std::string> Disasm::disasm(unsigned int AbsAddr, unsigned int Count) {
    assert(AbsAddr < DEFAULT_MEMORY_SIZE);

    std::vector<std::string> instr;
    position = AbsAddr;

    unsigned int pos = 0;
    for(unsigned int i=0;i<Count;++i) {
        pos = position; 
        instr.push_back(disasm());
        printf("%4x:%s\n", pos, instr[i].c_str());
    }

    return instr;
}

std::string Disasm::disasm() {
    std::string inst;

    ovr.clear();
    ovr.setOverrides(position, memory);
    position += ovr.getOverrideCount();

    ubyte opByte = memory[position];
    Opcode opData = opcodes[opByte];

/*
   
    if(opData.enc != GRP_ENC)
        inst += opData.instr;

    if(opData.enc == ONE_BYTE_ENC || opData.enc == REG_REG_ENC) position++;

    if(opData.enc == MODRM_ENC || opData.enc == MODRM_MEM_ENC) {
        mod.decode(memory[position+1]);
        disasmModRM(TO_REGISTER(opByte), IS_WORD(opByte), opData.enc == MODRM_MEM_ENC , inst);
        position += mod.getModInstrSize(0);
    }
    if(opData.enc == MODRM_SEG_ENC) {
        mod.decode(memory[position+1]);
        disasmModRMSeg(TO_REGISTER(opByte), inst);
        position += mod.getModInstrSize(0);
    }
    if(opData.enc == IMM_ENC) {
        disasmImm(opData.lhs == IMM_IV, inst);
        position += (opData.lhs == IMM_IV)?3:2;
    }
    if(opData.enc == REG_IMM_ENC) {
        disasmRegImm(opData.rhs == IMM_IV, opData.lhs, inst);
        position += (opData.rhs == IMM_IV)?3:2;
    }
    if(opData.enc == JMP_ENC) {
        disasmJmp(opData.lhs == IMM_IV, inst);
        position += (opData.lhs == IMM_IV)?3:2;
    }
    if(opData.enc == MODRM_IMM_ENC) {
        mod.decode(memory[position+1]);
        disasmModRMImm(IS_WORD(opByte), false, inst);
        position += mod.getModInstrSize(IS_WORD(opByte)?2:1);        
    }
    if(opData.enc == GRP_ENC) {
        mod.decode(memory[position+1]);
        disasmGrp(opData, opByte, inst);
    }
    if(opData.enc == INVALID_ENC) {
        ++position; 
    }

*/

    if(opData.enc != GRP_ENC)
       inst += opData.instr;

    switch(opData.enc) {
  
        /* 0 Arguments, only opcode */
        case REG_REG_ENC:
        case ONE_BYTE_ENC:
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
            ++position;
            break;

        case RAW_SEG_RAW_OFF_ENC:
        case AXAL_SEG_OFF_ENC:
        case MODRM_ONE_ARG:
        case MODRM_ARG_ONE:
            assert(false);
        
    }

    return inst;

}

void Disasm::disasmImm(bool isWord, std::string& instr) {

    char buf[BUF_LEN];

    if(!isWord) 
        snprintf(buf, BUF_LEN, "%#x", GET_BYTE(1) );
    else 
        snprintf(buf, BUF_LEN, "%#x", GET_WORD(2, 1) );

    instr += " " + std::string(buf);
}

void Disasm::disasmJmp(bool isWord, std::string& instr) {

    char buf[BUF_LEN];

    if(!isWord) {
        sbyte rel = GET_BYTE(1) + position + 0x2;
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
        disasmModRMImm(IS_WORD(opbyte), op.rhs == IMM_IV, instr);
        position += mod.getModInstrSize(op.rhs == IMM_IV?2:1);
    }

    if(_op.enc == MODRM_ARG_ONE || _op.enc == MODRM_ONE_ARG) {
        disasmModRMOne(IS_WORD(opbyte), op.rhs, instr);
        position += mod.getModInstrSize(0); 
    }
    
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
        instr = overrides.first + instr + " " + lhs + ", " + overrides.second + rhs + getModRMDisplacement(); 
    else 
        instr = overrides.first + instr + " " + overrides.second + lhs + getModRMDisplacement() + ", " + rhs;  
    

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
        instr = overrides.first + instr + " " + lhs + ", " + overrides.second + rhs + getModRMDisplacement(); 
    else 
        instr = overrides.first + instr + " " + overrides.second + lhs + getModRMDisplacement() + ", " + rhs;  
}

void Disasm::disasmModRMImm(bool isWord, bool isImmWord, std::string& instr) {
 
    char buf[BUF_LEN] = {0};
    
    std::string lhs;
    auto overrides = getOverrides();

    if(!isWord || !isImmWord) 
        sprintf(buf, "%#x", memory[position+mod.getDisplacementSize(1)+1]);
    else if(isWord || isImmWord) {
        uword imm = memory[position+mod.getDisplacementSize(1)+2] << 8 | memory[position+mod.getDisplacementSize(1)+1];
        sprintf(buf, "%#x", imm);
    }

    if(mod.mode != REGISTER_MODE)
        lhs = mrm[mod.mode.to_ulong()][mod.rm.to_ulong()];
    else
        lhs = regs[isWord][mod.rm.to_ulong()];

    instr = overrides.first + instr + " " + overrides.second + lhs + getModRMDisplacement() + ", " + buf;
}

void Disasm::disasmRegImm(bool isWord, Operand lhs, std::string& instr) {
    
    char buf[BUF_LEN] = {0};
    
    if(!isWord) 
        snprintf(buf, BUF_LEN, "%#x", GET_BYTE(1) );
    else 
        snprintf(buf, BUF_LEN, "%#x", GET_WORD(2, 1) );

    instr += " " + regsHelper[lhs] + ", " + std::string(buf);

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
        sprintf(buf, "%c%#x]", (value<0)?'-':'+', value<0?-value:value);
    }

    if(mod.mode == WORD_DISPLACEMENT) {
        int16_t value = GET_WORD(3, 2); 
        sprintf(buf, "%c%#x]", (value<0)?'-':'+', value<0?-value:value);
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

