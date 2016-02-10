#include "Disasm.hpp" 

/* Bind memory with code */

void Disasm::bindMemory(ubyte* mem) {
    
    assert(mem != nullptr);
    memory = mem;
}

std::vector<std::string> Disasm::disasmN(unsigned int AbsAddr, unsigned int Count) {
    assert(AbsAddr < DEFAULT_MEMORY_SIZE);

    std::vector<std::string> instr;
    position = AbsAddr;

    for(unsigned int i=0;i<Count;++i) {
        instr.push_back(disasm());
        printf("X:%s\n", instr[i].c_str());
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

    return inst;

}

void Disasm::disasmImm(bool isWord, std::string& instr) {

    char buf[32] = {0};
    if(!isWord) sprintf(buf, "%#x", memory[position+1]);
    else sprintf(buf, "%#x", memory[position+2] << 8 | memory[position+1]);

    instr += " " + std::string(buf);
}

void Disasm::disasmModRM(bool toRegister, bool isWord, bool isMemOperation, std::string& instr) {

    InstrString ins;
    auto overrides = getOverrides();
    
    if(mod.mode == REGISTER_MODE) {
        if(!toRegister) {
            ins.lhs = regs[isWord][mod.rm.to_ulong()];
            ins.rhs = regs[isWord][mod.reg.to_ulong()];
        }
        else {
            ins.lhs = regs[isWord][mod.reg.to_ulong()];
            ins.rhs = regs[isWord][mod.rm.to_ulong()];
        }
        instr = overrides.first + instr + " " + ins.lhs + ", " + ins.rhs; 
        return; 
    }

    
    bool memSideLeft = false;

    // lea ax, [bx] <- Mem operation, only to register
    if(toRegister || isMemOperation) {
        ins.lhs = regs[isWord][mod.reg.to_ulong()];
        ins.rhs = mrm[mod.mode.to_ulong()][mod.rm.to_ulong()];
    }
    else {
        ins.lhs = mrm[mod.mode.to_ulong()][mod.rm.to_ulong()];
        ins.rhs = regs[isWord][mod.reg.to_ulong()];
        memSideLeft = true;
    }


    if(!memSideLeft) 
        instr = overrides.first + instr + " " + ins.lhs + ", " + overrides.second + ins.rhs + getModRMDisplacement(); 
    else 
        instr = overrides.first + instr + " " + overrides.second + ins.lhs + getModRMDisplacement() + ", " + ins.rhs;  
    

}

void Disasm::disasmModRMSeg(bool toSegmentRegister, std::string& instr) {

    InstrString ins;
    auto overrides = getOverrides();

    if(mod.mode == REGISTER_MODE) {
        if(!toSegmentRegister) {
            ins.lhs = regs[true][mod.rm.to_ulong()];
            ins.rhs = regSeg[mod.reg.to_ulong()];
        }
        else {
            ins.lhs = regSeg[mod.reg.to_ulong()];
            ins.rhs = regs[true][mod.rm.to_ulong()];
        }
        instr = overrides.first + instr + " " + ins.lhs + ", " + ins.rhs; 
        return; 
    }
  
    bool memSideLeft = false;

    if(toSegmentRegister) {
        ins.lhs = regSeg[mod.reg.to_ulong()];
        ins.rhs = mrm[mod.mode.to_ulong()][mod.rm.to_ulong()]; 
    }
    else {
        ins.lhs = mrm[mod.mode.to_ulong()][mod.rm.to_ulong()];
        ins.rhs = regSeg[mod.reg.to_ulong()]; 
        memSideLeft = true; 
    }
   
    if(!memSideLeft) 
        instr = overrides.first + instr + " " + ins.lhs + ", " + overrides.second + ins.rhs + getModRMDisplacement(); 
    else 
        instr = overrides.first + instr + " " + overrides.second + ins.lhs + getModRMDisplacement() + ", " + ins.rhs;  
}

void Disasm::disasmRegImm(bool isWord, Operand lhs, std::string& instr) {
    
    char buf[32] = {0};
    if(!isWord) sprintf(buf, "%#x", memory[position+1]);
    else sprintf(buf, "%#x", memory[position+2]<<8|memory[position+1]);

    instr += " " + regsHelper[lhs] + ", " + std::string(buf);

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

