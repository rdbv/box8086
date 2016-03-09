#include "CPU.hpp"

/* Return pointer to memory used by emulator */

Memory* CPU::getMemory() {
    return &memory;
}

/* Return actual state of registers */

Registers CPU::getRegisters() {
    return regs;
}

/* 
 * Do a hardware step,
 * This is NOT Trap Flag step
 */

void CPU::step() {
    fetch();
    execute();
}

/* 
 * Clear helper structs, check/set overrides,
 * And set helper IPReal to byte with skipped override bytes
 */

void CPU::fetch() {
    mod.clear();
    ovrd.clear();
    ovrd.setOverrides(getAbs(regs.cs, regs.ip), &memory[0]);
    IPReal = getAbs(regs.cs, regs.ip) + ovrd.getOverrideCount();
}

/* Just execute */
void CPU::execute() {

    switch(memory[IPReal])
    {
        case 0x00:	//00 ADD Eb Gb 
            mod.decode(memory[IPReal+1]);
            setOperands<false, false>();
            tmp_w = ((uword) *lhs_b) + ((uword) *rhs_b);
            *lhs_b += *rhs_b;
            //setFlagsStandard<ubyte, true>(*lhs_b);
            incIP(mod.getModInstrSize(ovrd.getOverrideCount()));
        break;
        case 0x01:	//01 ADD Ev Gv 
            mod.decode(memory[IPReal+1]);
            setOperands<false, true>();
            tmp_d = ((dword) *lhs_w) + ((dword) *rhs_w);
            *lhs_w += *rhs_w;
            //setFlags<uword, true>(*lhs_w);
            incIP(mod.getModInstrSize(ovrd.getOverrideCount()));
        break;
        case 0x02:	//02 ADD Gb Eb 
            mod.decode(memory[IPReal+1]);
            setOperands<true, false>();
            tmp_w = ((uword) *lhs_b) + ((uword) *rhs_b);
            *lhs_b += *rhs_b;
            //setFlags<ubyte, true>(*lhs_b);
            incIP(mod.getModInstrSize(ovrd.getOverrideCount()));
        break;
        case 0x03:	//03 ADD Gv Ev 
            mod.decode(memory[IPReal+1]);
            setOperands<true, true>();
            tmp_d = ((dword) *lhs_w) + ((dword) *rhs_w);
            *lhs_w += *rhs_w;
            //setFlags<uword, true>(*lhs_w);
            incIP(mod.getModInstrSize(ovrd.getOverrideCount()));
        break;
        case 0x04:	//04 ADD AL Ib 
        break;
        case 0x05:	//05 ADD eAX Iv 
        break;
        case 0x06:	//06 PUSH ES 
        break;
        case 0x07:	//07 POP ES 
        break;
        case 0x08:	//08 OR Eb Gb 
        break;
        case 0x09:	//09 OR Ev Gv 
        break;
        case 0x0A:	//0A OR Gb Eb 
        break;
        case 0x0B:	//0B OR Gv Ev 
        break;
        case 0x0C:	//0C OR AL Ib 
        break;
        case 0x0D:	//0D OR eAX Iv 
        break;
        case 0x0E:	//0E PUSH CS 
        break;
        case 0x0F:	//0F -- 
        break;
        case 0x10:	//10 ADC Eb Gb 
        break;
        case 0x11:	//11 ADC Ev Gv 
        break;
        case 0x12:	//12 ADC Gb Eb 
        break;
        case 0x13:	//13 ADC Gv Ev 
        break;
        case 0x14:	//14 ADC AL Ib 
        break;
        case 0x15:	//15 ADC eAX Iv 
        break;
        case 0x16:	//16 PUSH SS 
        break;
        case 0x17:	//17 POP SS 
        break;
        case 0x18:	//18 SBB Eb Gb 
        break;
        case 0x19:	//19 SBB Ev Gv 
        break;
        case 0x1A:	//1A SBB Gb Eb 
        break;
        case 0x1B:	//1B SBB Gv Ev 
        break;
        case 0x1C:	//1C SBB AL Ib 
        break;
        case 0x1D:	//1D SBB eAX Iv 
        break;
        case 0x1E:	//1E PUSH DS 
        break;
        case 0x1F:	//1F POP DS 
        break;
        case 0x20:	//20 AND Eb Gb 
        break;
        case 0x21:	//21 AND Ev Gv 
        break;
        case 0x22:	//22 AND Gb Eb 
        break;
        case 0x23:	//23 AND Gv Ev 
        break;
        case 0x24:	//24 AND AL Ib 
        break;
        case 0x25:	//25 AND eAX Iv 
        break;
        case 0x26:	//26 ES: 
        break;
        case 0x27:	//27 DAA 
        break;
        case 0x28:	//28 SUB Eb Gb 
        break;
        case 0x29:	//29 SUB Ev Gv 
        break;
        case 0x2A:	//2A SUB Gb Eb 
        break;
        case 0x2B:	//2B SUB Gv Ev 
        break;
        case 0x2C:	//2C SUB AL Ib 
        break;
        case 0x2D:	//2D SUB eAX Iv 
        break;
        case 0x2E:	//2E CS: 
        break;
        case 0x2F:	//2F DAS 
        break;
        case 0x30:	//30 XOR Eb Gb 
        break;
        case 0x31:	//31 XOR Ev Gv 
        break;
        case 0x32:	//32 XOR Gb Eb 
        break;
        case 0x33:	//33 XOR Gv Ev 
        break;
        case 0x34:	//34 XOR AL Ib 
        break;
        case 0x35:	//35 XOR eAX Iv 
        break;
        case 0x36:	//36 SS: 
        break;
        case 0x37:	//37 AAA 
        break;
        case 0x38:	//38 CMP Eb Gb 
        break;
        case 0x39:	//39 CMP Ev Gv 
        break;
        case 0x3A:	//3A CMP Gb Eb 
        break;
        case 0x3B:	//3B CMP Gv Ev 
        break;
        case 0x3C:	//3C CMP AL Ib 
        break;
        case 0x3D:	//3D CMP eAX Iv 
        break;
        case 0x3E:	//3E DS: 
        break;
        case 0x3F:	//3F AAS 
        break;
        case 0x40:	//40 INC eAX 
        break;
        case 0x41:	//41 INC eCX 
        break;
        case 0x42:	//42 INC eDX 
        break;
        case 0x43:	//43 INC eBX 
        break;
        case 0x44:	//44 INC eSP 
        break;
        case 0x45:	//45 INC eBP 
        break;
        case 0x46:	//46 INC eSI 
        break;
        case 0x47:	//47 INC eDI 
        break;
        case 0x48:	//48 DEC eAX 
        break;
        case 0x49:	//49 DEC eCX 
        break;
        case 0x4A:	//4A DEC eDX 
        break;
        case 0x4B:	//4B DEC eBX 
        break;
        case 0x4C:	//4C DEC eSP 
        break;
        case 0x4D:	//4D DEC eBP 
        break;
        case 0x4E:	//4E DEC eSI 
        break;
        case 0x4F:	//4F DEC eDI 
        break;
        case 0x50:	//50 PUSH eAX 
        break;
        case 0x51:	//51 PUSH eCX 
        break;
        case 0x52:	//52 PUSH eDX 
        break;
        case 0x53:	//53 PUSH eBX 
        break;
        case 0x54:	//54 PUSH eSP 
        break;
        case 0x55:	//55 PUSH eBP 
        break;
        case 0x56:	//56 PUSH eSI 
        break;
        case 0x57:	//57 PUSH eDI 
        break;
        case 0x58:	//58 POP eAX 
        break;
        case 0x59:	//59 POP eCX 
        break;
        case 0x5A:	//5A POP eDX 
        break;
        case 0x5B:	//5B POP eBX 
        break;
        case 0x5C:	//5C POP eSP 
        break;
        case 0x5D:	//5D POP eBP 
        break;
        case 0x5E:	//5E POP eSI 
        break;
        case 0x5F:	//5F POP eDI 
        break;
        case 0x60:	//60 -- 
        break;
        case 0x61:	//61 -- 
        break;
        case 0x62:	//62 -- 
        break;
        case 0x63:	//63 -- 
        break;
        case 0x64:	//64 -- 
        break;
        case 0x65:	//65 -- 
        break;
        case 0x66:	//66 -- 
        break;
        case 0x67:	//67 -- 
        break;
        case 0x68:	//68 -- 
        break;
        case 0x69:	//69 -- 
        break;
        case 0x6A:	//6A -- 
        break;
        case 0x6B:	//6B -- 
        break;
        case 0x6C:	//6C -- 
        break;
        case 0x6D:	//6D -- 
        break;
        case 0x6E:	//6E -- 
        break;
        case 0x6F:	//6F -- 
        break;
        case 0x70:	//70 JO Jb 
        break;
        case 0x71:	//71 JNO Jb 
        break;
        case 0x72:	//72 JB Jb 
        break;
        case 0x73:	//73 JNB Jb 
        break;
        case 0x74:	//74 JZ Jb 
        break;
        case 0x75:	//75 JNZ Jb 
        break;
        case 0x76:	//76 JBE Jb 
        break;
        case 0x77:	//77 JA Jb 
        break;
        case 0x78:	//78 JS Jb 
        break;
        case 0x79:	//79 JNS Jb 
        break;
        case 0x7A:	//7A JPE Jb 
        break;
        case 0x7B:	//7B JPO Jb 
        break;
        case 0x7C:	//7C JL Jb 
        break;
        case 0x7D:	//7D JGE Jb 
        break;
        case 0x7E:	//7E JLE Jb 
        break;
        case 0x7F:	//7F JG Jb 
        break;
        case 0x80:	//80 GRP1 Eb Ib 
        break;
        case 0x81:	//81 GRP1 Ev Iv 
        break;
        case 0x82:	//82 GRP1 Eb Ib 
        break;
        case 0x83:	//83 GRP1 Ev Ib 
        break;
        case 0x84:	//84 TEST Gb Eb 
        break;
        case 0x85:	//85 TEST Gv Ev 
        break;
        case 0x86:	//86 XCHG Gb Eb 
        break;
        case 0x87:	//87 XCHG Gv Ev 
        break;
        case 0x88:	//88 MOV Eb Gb 
        break;
        case 0x89:	//89 MOV Ev Gv 
        break;
        case 0x8A:	//8A MOV Gb Eb 
        break;
        case 0x8B:	//8B MOV Gv Ev 
        break;
        case 0x8C:	//8C MOV Ew Sw 
        break;
        case 0x8D:	//8D LEA Gv M 
        break;
        case 0x8E:	//8E MOV Sw Ew 
        break;
        case 0x8F:	//8F POP Ev 
        break;
        case 0x90:	//90 NOP 
            regs.ip += ovrd.getOverrideCount() + 1; 
        break;
        case 0x91:	//91 XCHG eCX eAX 
        break;
        case 0x92:	//92 XCHG eDX eAX 
        break;
        case 0x93:	//93 XCHG eBX eAX 
        break;
        case 0x94:	//94 XCHG eSP eAX 
        break;
        case 0x95:	//95 XCHG eBP eAX 
        break;
        case 0x96:	//96 XCHG eSI eAX 
        break;
        case 0x97:	//97 XCHG eDI eAX 
        break;
        case 0x98:	//98 CBW 
        break;
        case 0x99:	//99 CWD 
        break;
        case 0x9A:	//9A CALL Ap 
        break;
        case 0x9B:	//9B WAIT 
        break;
        case 0x9C:	//9C PUSHF 
        break;
        case 0x9D:	//9D POPF 
        break;
        case 0x9E:	//9E SAHF 
        break;
        case 0x9F:	//9F LAHF 
        break;
        case 0xA0:	//A0 MOV AL Ob 
        break;
        case 0xA1:	//A1 MOV eAX Ov 
        break;
        case 0xA2:	//A2 MOV Ob AL 
        break;
        case 0xA3:	//A3 MOV Ov eAX 
        break;
        case 0xA4:	//A4 MOVSB 
        break;
        case 0xA5:	//A5 MOVSW 
        break;
        case 0xA6:	//A6 CMPSB 
        break;
        case 0xA7:	//A7 CMPSW 
        break;
        case 0xA8:	//A8 TEST AL Ib 
        break;
        case 0xA9:	//A9 TEST eAX Iv 
        break;
        case 0xAA:	//AA STOSB 
        break;
        case 0xAB:	//AB STOSW 
        break;
        case 0xAC:	//AC LODSB 
        break;
        case 0xAD:	//AD LODSW 
        break;
        case 0xAE:	//AE SCASB 
        break;
        case 0xAF:	//AF SCASW 
        break;
        case 0xB0:	//B0 MOV AL Ib 
            regs.al = memory.getRawData<ubyte, 1, 0>(IPReal);
            incIP(2);
        break;
        case 0xB1:	//B1 MOV CL Ib 
            regs.cl = memory.getRawData<ubyte, 1, 0>(IPReal);
            incIP(2);
        break;
        case 0xB2:	//B2 MOV DL Ib 
            regs.dl = memory.getRawData<ubyte, 1, 0>(IPReal);
            incIP(2);
        break;
        case 0xB3:	//B3 MOV BL Ib 
            regs.bl = memory.getRawData<ubyte, 1, 0>(IPReal);
            incIP(2);
        break;
        case 0xB4:	//B4 MOV AH Ib 
            regs.ah = memory.getRawData<ubyte, 1, 0>(IPReal);
            incIP(2);
        break;
        case 0xB5:	//B5 MOV CH Ib 
            regs.ch = memory.getRawData<ubyte, 1, 0>(IPReal);
            incIP(2);
        break;
        case 0xB6:	//B6 MOV DH Ib 
            regs.dh = memory.getRawData<ubyte, 1, 0>(IPReal);
            incIP(2); 
        break;
        case 0xB7:	//B7 MOV BH Ib 
            regs.bh = memory.getRawData<ubyte, 1, 0>(IPReal);
            incIP(2);
        break;
        case 0xB8:	//B8 MOV eAX Iv 
            regs.ax = memory.getRawData<uword, 2, 1>(IPReal);
            incIP(3); 
        break;
        case 0xB9:	//B9 MOV eCX Iv 
            regs.cx = memory.getRawData<uword, 2, 1>(IPReal);
            incIP(3);
        break;
        case 0xBA:	//BA MOV eDX Iv 
            regs.dx = memory.getRawData<uword, 2, 1>(IPReal);
            incIP(3);
        break;
        case 0xBB:	//BB MOV eBX Iv 
            regs.bx = memory.getRawData<uword, 2, 1>(IPReal);
            incIP(3);
        break;
        case 0xBC:	//BC MOV eSP Iv 
            regs.sp = memory.getRawData<uword, 2, 1>(IPReal);
            incIP(3); 
        break;
        case 0xBD:	//BD MOV eBP Iv 
            regs.bp = memory.getRawData<uword, 2, 1>(IPReal);
            incIP(3);
        break;
        case 0xBE:	//BE MOV eSI Iv 
            regs.si = memory.getRawData<uword, 2, 1>(IPReal);
            incIP(3); 
        break;
        case 0xBF:	//BF MOV eDI Iv 
            regs.di = memory.getRawData<uword, 2, 1>(IPReal);
            incIP(3); 
        break;
        case 0xC0:	//C0 -- 
        break;
        case 0xC1:	//C1 -- 
        break;
        case 0xC2:	//C2 RET Iw 
        break;
        case 0xC3:	//C3 RET 
        break;
        case 0xC4:	//C4 LES Gv Mp 
        break;
        case 0xC5:	//C5 LDS Gv Mp 
        break;
        case 0xC6:	//C6 MOV Eb Ib 
        break;
        case 0xC7:	//C7 MOV Ev Iv 
        break;
        case 0xC8:	//C8 -- 
        break;
        case 0xC9:	//C9 -- 
        break;
        case 0xCA:	//CA RETF Iw 
        break;
        case 0xCB:	//CB RETF 
        break;
        case 0xCC:	//CC INT 3 
        break;
        case 0xCD:	//CD INT Ib 
        break;
        case 0xCE:	//CE INTO 
        break;
        case 0xCF:	//CF IRET 
        break;
        case 0xD0:	//D0 GRP2 Eb 1 
        break;
        case 0xD1:	//D1 GRP2 Ev 1 
        break;
        case 0xD2:	//D2 GRP2 Eb CL 
        break;
        case 0xD3:	//D3 GRP2 Ev CL 
        break;
        case 0xD4:	//D4 AAM I0 
        break;
        case 0xD5:	//D5 AAD I0 
        break;
        case 0xD6:	//D6 -- 
        break;
        case 0xD7:	//D7 XLAT 
        break;
        case 0xD8:	//D8 -- 
        break;
        case 0xD9:	//D9 -- 
        break;
        case 0xDA:	//DA -- 
        break;
        case 0xDB:	//DB -- 
        break;
        case 0xDC:	//DC -- 
        break;
        case 0xDD:	//DD -- 
        break;
        case 0xDE:	//DE -- 
        break;
        case 0xDF:	//DF -- 
        break;
        case 0xE0:	//E0 LOOPNZ Jb 
        break;
        case 0xE1:	//E1 LOOPZ Jb 
        break;
        case 0xE2:	//E2 LOOP Jb 
        break;
        case 0xE3:	//E3 JCXZ Jb 
        break;
        case 0xE4:	//E4 IN AL Ib 
        break;
        case 0xE5:	//E5 IN eAX Ib 
        break;
        case 0xE6:	//E6 OUT Ib AL 
        break;
        case 0xE7:	//E7 OUT Ib eAX 
        break;
        case 0xE8:	//E8 CALL Jv 
        break;
        case 0xE9:	//E9 JMP Jv 
            regs.ip = static_cast<uword>((memory.getRawData<uword, 2, 1>(IPReal) + 3) + regs.ip); 
        break;
        case 0xEA:	//EA JMP Ap 
        break;
        case 0xEB:	//EB JMP Jb 
            //regs.ip = (uword) ((memory.getRawData<ubyte, 1, 0>(IPReal) + 2) + regs.ip);
        break;
        case 0xEC:	//EC IN AL DX 
        break;
        case 0xED:	//ED IN eAX DX 
        break;
        case 0xEE:	//EE OUT DX AL 
        break;
        case 0xEF:	//EF OUT DX eAX 
        break;
        case 0xF0:	//F0 LOCK 
        break;
        case 0xF1:	//F1 -- 
        break;
        case 0xF2:	//F2 REPNZ 
        break;
        case 0xF3:	//F3 REPZ 
        break;
        case 0xF4:	//F4 HLT 
        break;
        case 0xF5:	//F5 CMC 
        break;
        case 0xF6:	//F6 GRP3a Eb 
        break;
        case 0xF7:	//F7 GRP3b Ev 
        break;
        case 0xF8:	//F8 CLC 
        break;
        case 0xF9:	//F9 STC 
        break;
        case 0xFA:	//FA CLI 
        break;
        case 0xFB:	//FB STI 
        break;
        case 0xFC:	//FC CLD 
        break;
        case 0xFD:	//FD STD 
        break;
        case 0xFE:	//FE GRP4 Eb 
        break;
        case 0xFF:	//FF GRP5 Ev 
        break;
    };

}

/* If instruction had overrided segment, return it */

uword CPU::getOverridedSegmentValue() {

    auto modSelector = mod.rm.to_ulong();
  
    /* If mode is BP* then segment register is SS not DS */
    if( modSelector == BP_SI || 
        modSelector == BP_DI ||
        modSelector == BP )
        return regs.ss;

    switch(ovrd.regOverride) {
        case 0:
            return 0;
        case CS_OVR:
            return regs.cs;
        case DS_OVR:
            return regs.ds;
        case ES_OVR:
            return regs.es;
        case SS_OVR:
            return regs.ss;
        default:
            assert(false);
    }

}

/* 
 * Get absolute address in memory from ModRM byte,
 * mov [bx+si], al | assuming bx=5, si = 5, return = 10
 */

dword CPU::getAbsoluteAddressModRM() {
    
    assert(mod.mode != REGISTER_MODE);
    
    uword absAddr = 0;

    if(mod.mode == NO_DISPLACEMENT && mod.rm == UWORD)
        return getAbs(getOverridedSegmentValue(), memory.getRawData<uword, 3, 2>(IPReal));

    switch(mod.rm.to_ulong()) {
        
        case BX_SI:
            absAddr = regs.bx + regs.si;
        break;
        
        case BX_DI:
            absAddr = regs.bx + regs.di;
        break;
        
        case BP_SI:
            absAddr = regs.bp + regs.si;
        break;
        
        case BP_DI:
            absAddr = regs.bp + regs.di;
        break;
        
        case SI:
            absAddr = regs.si;
        break;
        
        case DI:
            absAddr = regs.di;
        break;
        
        case BP:
            absAddr = regs.bp;
        break;
        
        case BX:
            absAddr = regs.bx;
        break;
        
    }
    
    if(mod.mode == BYTE_DISPLACEMENT) absAddr += memory.getRawData<sbyte, 2, 0>(IPReal); 
    if(mod.mode == WORD_DISPLACEMENT) absAddr += memory.getRawData<sword, 3, 2>(IPReal);

    return getAbs(getOverridedSegmentValue(), absAddr);

}

/* Get reference to top stack element */

uword* CPU::getStackTopPtr() {
    uword* stackTop = (uword*) &memory[getAbs(regs.ss, regs.sp)];
    return stackTop;
}

template<bool toReg, bool isWord>
void CPU::setOperands() {

    /* Weird quirk */
    if(mod.mode == REGISTER_MODE) {
        
        if(!toReg) {
            if(isWord) {
                lhs_w = wordRegs[mod.rm.to_ulong()];
                rhs_w = wordRegs[mod.reg.to_ulong()];
                lhs_buf_w = *lhs_w;
                rhs_buf_w = *rhs_w;
            }
            else {
                lhs_b = byteRegs[mod.rm.to_ulong()];
                rhs_b = byteRegs[mod.reg.to_ulong()];
                lhs_buf_b = *lhs_b;
                rhs_buf_b = *rhs_b;
            }
        }
        
        else {
           
            if(isWord) {
                lhs_w = wordRegs[mod.reg.to_ulong()];
                rhs_w = wordRegs[mod.rm.to_ulong()];
                lhs_buf_w = *lhs_w;
                rhs_buf_w = *rhs_w;
            }
            else {
                lhs_b = byteRegs[mod.reg.to_ulong()];
                rhs_b = byteRegs[mod.rm.to_ulong()];
                lhs_buf_b = *lhs_b;
                rhs_buf_b = *rhs_b;
            }
        }
        
        return;
    }


    if(toReg) {
        if(isWord) {
            lhs_w = wordRegs[mod.reg.to_ulong()];
            rhs_w = (uword*) &memory[getAbsoluteAddressModRM()]; 
            lhs_buf_w = *lhs_w;
            rhs_buf_w = *rhs_w;
        }
        else {
            lhs_b = byteRegs[mod.reg.to_ulong()];
            rhs_b = &memory[getAbsoluteAddressModRM()];
            lhs_buf_b = *lhs_b;
            rhs_buf_b = *rhs_b;
        }
        return;
    }


    else {
        if(isWord) {
            lhs_w = (uword*) &memory[getAbsoluteAddressModRM()];
            rhs_w = wordRegs[mod.reg.to_ulong()];
            lhs_buf_w = *lhs_w;
            rhs_buf_w = *rhs_w;
        }
        else {
            lhs_b = &memory[getAbsoluteAddressModRM()];
            rhs_b = byteRegs[mod.reg.to_ulong()];
            lhs_buf_b = *lhs_b;
            rhs_buf_b = *rhs_b;
        }
        return;
    }

}
// c-s for adc, add, 
// n-c-s for sbb, sub, 

template<typename T, bool checkSignBit, bool isLogic>
void CPU::setFlags(T value) {

    const bool isWord = std::is_same<T, sword>() || std::is_same<T, uword>();
    regs.flags[ZF] = value == 0;

    if(!isWord) {
        regs.flags[PF] = parity[value];
        regs.flags[SF] = value & 0x80;
        regs.flags[CF] = tmp_w & 0xFF00; 
        
        if(checkSignBit) {
            regs.flags[OF] = ((tmp_w ^ lhs_buf_b) & (tmp_w ^ rhs_buf_b) & 0x80) == 0x80;
            if(!isLogic) 
                regs.flags[AF] = ((lhs_buf_b ^ rhs_buf_b ^ tmp_w) & 0x10) == 0x10;  
        } 
        else {
            
            regs.flags[OF] = ((tmp_w ^ lhs_buf_b) & (tmp_w ^ rhs_buf_b) & 0x80);
            if(!isLogic)
                regs.flags[AF] = ((lhs_buf_b ^ rhs_buf_b ^ tmp_w) & 0x10);  
        }
    
    }
    else {
        regs.flags[PF] = parity[value & 255];
        regs.flags[SF] = value & 0x8000;
        regs.flags[CF] = tmp_d & 0xFFFF0000;
        
        if(checkSignBit) {
            regs.flags[OF] = ((tmp_d ^ lhs_buf_w) & (tmp_d ^ rhs_buf_w) & 0x8000) == 0x8000; 
            if(!isLogic)
                regs.flags[AF] = ((lhs_buf_w ^ rhs_buf_w ^ tmp_d) & 0x10) == 0x10; 
        } 
        else {
            regs.flags[OF] = ((tmp_d ^ lhs_buf_w) & (tmp_d ^ rhs_buf_w) & 0x8000);
            if(!isLogic)
                regs.flags[AF] = ((lhs_buf_w ^ rhs_buf_w ^ tmp_d) & 0x10); 
        } 
    
    }

    if(isLogic) {
        regs.flags[OF] = 0;
        regs.flags[CF] = 0;
    }
    assert(false);
} 

/* ADD, ADC, Logic operations */
template<typename T, bool checkSignBit, bool isLogic>
void CPU::setFlagsStandard(T value) {
    
    const bool isWord = std::is_same<T, sword>() || std::is_same<T, uword>();
    regs.flags[ZF] = value == 0;
    bool savedAF = regs.flags[AF];

    if(!isWord) {
        regs.flags[PF] = parity[value];
        regs.flags[SF] = value & 0x80;
        regs.flags[CF] = tmp_w & 0xFF00; 
        
        if(checkSignBit) {
            regs.flags[OF] = ((tmp_w ^ lhs_buf_b) & (tmp_w ^ rhs_buf_b) & 0x80) == 0x80;
            regs.flags[AF] = ((lhs_buf_b ^ rhs_buf_b ^ tmp_w) & 0x10) == 0x10;  
        }
        else {
            regs.flags[OF] = ((tmp_w ^ lhs_buf_b) & (tmp_w ^ rhs_buf_b) & 0x80);
            regs.flags[AF] = ((lhs_buf_b ^ rhs_buf_b ^ tmp_w) & 0x10);  
        }

    }
    else {
        regs.flags[PF] = parity[value & 255];
        regs.flags[SF] = value & 0x8000;
        regs.flags[CF] = tmp_d & 0xFFFF0000;
        
        if(checkSignBit) {
            regs.flags[OF] = ((tmp_d ^ lhs_buf_w) & (tmp_d ^ rhs_buf_w) & 0x8000) == 0x8000; 
            regs.flags[AF] = ((lhs_buf_w ^ rhs_buf_w ^ tmp_d) & 0x10) == 0x10; 
        }
        else {
            regs.flags[OF] = ((tmp_d ^ lhs_buf_w) & (tmp_d ^ rhs_buf_w) & 0x8000);
            regs.flags[AF] = ((lhs_buf_w ^ rhs_buf_w ^ tmp_d) & 0x10); 
        }

    }

    /* Logic operations always clear CF && OF */
    if(isLogic) {
        regs.flags[OF] = 0;
        regs.flags[CF] = 0;
        regs.flags[AF] = savedAF;
    }

}
