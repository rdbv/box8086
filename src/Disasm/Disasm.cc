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
    return inst;

}

void Disasm::disasmGrp(Opcode& op, ubyte& opbyte, std::string& instr) {

    Opcode _op = grp[op.group][mod.reg.to_ulong()];
   
    instr += _op.instr;

    if(_op.enc == MODRM_IMM_ENC) {
        disasmModRMImm(IS_WORD(opbyte), op.rhs == IMM_IV, instr);
        position += mod.getModInstrSize(op.rhs == IMM_IV?2:1);
    }

    if(_op.enc == MODRM_ARG_ONE || MODRM_ONE_ARG) {
       
        int selectorRhs = -1;
        if(_op.enc == MODRM_ARG_ONE){
            if(opbyte == 0xd0 || opbyte == 0xd1)
                selectorRhs = 0;
            else
                selectorRhs = 1;
        }
        disasmModRMOne(IS_WORD(opbyte), selectorRhs, instr); 
        position += mod.getModInstrSize(0); 
    }
    
}

void Disasm::disasmImm(bool isWord, std::string& instr) {

    char buf[32] = {0};
    if(!isWord) sprintf(buf, "%#x", memory[position+1]);
    else sprintf(buf, "%#x", memory[position+2] << 8 | memory[position+1]);

    instr += " " + std::string(buf);
}

void Disasm::disasmJmp(bool isWord, std::string& instr) {

    char buf[32] = {0};
    if(!isWord) {
        sbyte rel = memory[position+1] + position + 0x2;
        sprintf(buf, "%#x", (uword) rel);
    }
    else {
        sword rel = (memory[position+2] << 8 | memory[position+1]) + position + 0x3;
        sprintf(buf, "%#x", (uword) rel);
    }
    
    instr += " " + std::string(buf);
}

void Disasm::disasmModRM(bool toRegister, bool isWord, bool isMemOperation, std::string& instr) {

    std::string lhs, rhs; 
    auto overrides = getOverrides();
    
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
  
    char buf[32] = {0};
    std::string lhs, rhs;
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

    rhs = buf;
    
    instr = overrides.first + instr + " " + overrides.second + lhs + getModRMDisplacement() + ", " + rhs;
    
}

void Disasm::disasmRegImm(bool isWord, Operand lhs, std::string& instr) {
    
    char buf[32] = {0};
    if(!isWord) sprintf(buf, "%#x", memory[position+1]);
    else sprintf(buf, "%#x", memory[position+2]<<8|memory[position+1]);

    instr += " " + regsHelper[lhs] + ", " + std::string(buf);

}

void Disasm::disasmModRMOne(bool isWord, int rhsSelector, std::string& instr) {

    std::string lhs, rhs;

    auto overrides = getOverrides();

    if(mod.mode == REGISTER_MODE)
        lhs = regs[isWord][mod.rm.to_ulong()];
    else
        lhs = mrm[mod.mode.to_ulong()][mod.rm.to_ulong()];

    
    if(rhsSelector == 0)
        rhs = ", 1";
    if(rhsSelector == 1)
        rhs = ", cl";

    instr = overrides.first + instr + " " + lhs + getModRMDisplacement() + rhs;

}

std::string Disasm::getModRMDisplacement() {

    char buf[32] = {0};

    if(mod.mode == NO_DISPLACEMENT && mod.rm == UWORD) {
        int16_t value = 0; 
        value = memory[position+3] << 8 | memory[position+2];     
        sprintf(buf, "%#x]", (uint16_t)value); 
        return buf;
    }

    if(mod.mode == BYTE_DISPLACEMENT) {
        int8_t value = memory[position+2];
        sprintf(buf, "%c%#x]", (value<0)?'-':'+', value<0?-value:value);
    }

    if(mod.mode == WORD_DISPLACEMENT) {
        int16_t value = memory[position+3] << 8 | memory[position+2];
        sprintf(buf, "%c%#x]", (value<0)?'-':'+', value<0?-value:value);
    }

    return buf;
}

std::pair<std::string, std::string> Disasm::getOverrides() {
    std::string overridesBegin, overridesEnd;
    if(ovr.repOverride != 0) overridesBegin = opcodes[ovr.repOverride].instr; 
    if(ovr.regOverride != 0) overridesEnd = opcodes[ovr.regOverride].instr;
    return std::pair<std::string, std::string>(overridesBegin, overridesEnd);
}

