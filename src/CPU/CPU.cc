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
    /* IPReal is IP with skipped override bytes */
    IPReal = getAbs(regs.cs, regs.ip) + ovrd.getOverrideCount();
}

/* Just execute */

void CPU::execute() {

    switch(memory[IPReal])
    {
        case 0x00:	//00 ADD Eb Gb 
            addEbGb();
        break;
        case 0x01:	//01 ADD Ev Gv 
            addEvGv();
        break;
        case 0x02:	//02 ADD Gb Eb 
            addGbEb();
        break;
        case 0x03:	//03 ADD Gv Ev 
            addGvEv();
        break;
        case 0x04:	//04 ADD AL Ib 
            addAlIb();
        break;
        case 0x05:	//05 ADD eAX Iv 
            addAxIv();
        break;
        case 0x06:	//06 PUSH ES 
            pushEs();
        break;
        case 0x07:	//07 POP ES 
            popEs();
        break;
        case 0x08:	//08 OR Eb Gb 
            orEbGb();
        break;
        case 0x09:	//09 OR Ev Gv 
            orEvGv();
        break;
        case 0x0A:	//0A OR Gb Eb 
            orGbEb();
        break;
        case 0x0B:	//0B OR Gv Ev 
            orGvEv();
        break;
        case 0x0C:	//0C OR AL Ib 
            orAlIb();
        break;
        case 0x0D:	//0D OR eAX Iv 
            orAxIv();
        break;
        case 0x0E:	//0E PUSH CS 
            pushCs();
        break;
        case 0x0F:	//0F -- 
            assert(false);
        break;
        case 0x10:	//10 ADC Eb Gb 
            adcEbGb();
        break;
        case 0x11:	//11 ADC Ev Gv 
            adcEvGv();
        break;
        case 0x12:	//12 ADC Gb Eb 
            adcGbEb();
        break;
        case 0x13:	//13 ADC Gv Ev 
            adcGvEv();
        break;
        case 0x14:	//14 ADC AL Ib 
            adcAlIb();
        break;
        case 0x15:	//15 ADC eAX Iv 
            adcAxIv();
        break;
        case 0x16:	//16 PUSH SS 
            pushSs();
        break;
        case 0x17:	//17 POP SS 
            popSs();
        break;
        case 0x18:	//18 SBB Eb Gb 
            sbbEbGb();
        break;
        case 0x19:	//19 SBB Ev Gv 
            sbbEvGv();  
        break;
        case 0x1A:	//1A SBB Gb Eb 
            sbbGbEb();
        break;
        case 0x1B:	//1B SBB Gv Ev 
            sbbGvEv();
        break;
        case 0x1C:	//1C SBB AL Ib 
            sbbAlIb();
        break;
        case 0x1D:	//1D SBB eAX Iv 
            sbbAxIv();
        break;
        case 0x1E:	//1E PUSH DS 
            pushDs();
        break;
        case 0x1F:	//1F POP DS 
            popDs();
        break;
        case 0x20:	//20 AND Eb Gb 
            andEbGb();
        break;
        case 0x21:	//21 AND Ev Gv 
            andEvGv();
        break;
        case 0x22:	//22 AND Gb Eb 
            andGbEb();
        break;
        case 0x23:	//23 AND Gv Ev 
            andGvEv();
        break;
        case 0x24:	//24 AND AL Ib 
            andAlIb();
        break;
        case 0x25:	//25 AND eAX Iv 
            andAxIv();
        break;
        case 0x26:	//26 ES: 
            assert(false);
        break;
        case 0x27:	//27 DAA 
            daa();
        break;
        case 0x28:	//28 SUB Eb Gb 
            subEbGb();
        break;
        case 0x29:	//29 SUB Ev Gv 
            subEvGv();
        break;
        case 0x2A:	//2A SUB Gb Eb 
            subGbEb();
        break;
        case 0x2B:	//2B SUB Gv Ev 
            subGvEv();
        break;
        case 0x2C:	//2C SUB AL Ib 
            subAlIb();
        break;
        case 0x2D:	//2D SUB eAX Iv 
            subAxIv();
        break;
        case 0x2E:	//2E CS: 
            assert(false);
        break;
        case 0x2F:	//2F DAS 
            das();
        break;
        case 0x30:	//30 XOR Eb Gb 
            xorEbGb(); 
        break;
        case 0x31:	//31 XOR Ev Gv 
            xorEvGv();
        break;
        case 0x32:	//32 XOR Gb Eb 
            xorGbEb();
        break;
        case 0x33:	//33 XOR Gv Ev 
            xorGvEv();
        break;
        case 0x34:	//34 XOR AL Ib 
            xorAlIb();
        break;
        case 0x35:	//35 XOR eAX Iv 
            xorAxIv();
        break;
        case 0x36:	//36 SS: 
            assert(false);
        break;
        case 0x37:	//37 AAA 
            aaa();
        break;
        case 0x38:	//38 CMP Eb Gb 
            cmpEbGb();
        break;
        case 0x39:	//39 CMP Ev Gv 
            cmpEvGv();
        break;
        case 0x3A:	//3A CMP Gb Eb 
            cmpGbEb();
        break;
        case 0x3B:	//3B CMP Gv Ev 
            cmpGvEv();
        break;
        case 0x3C:	//3C CMP AL Ib 
            cmpAlIb();
        break;
        case 0x3D:	//3D CMP eAX Iv 
            cmpAxIv();
        break;
        case 0x3E:	//3E DS: 
            assert(false);
        break;
        case 0x3F:	//3F AAS 
            aas();
        break;
        case 0x40:	//40 INC eAX 
            incAx();
        break;
        case 0x41:	//41 INC eCX 
            incCx();
        break;
        case 0x42:	//42 INC eDX 
            incDx();
        break;
        case 0x43:	//43 INC eBX 
            incBx();
        break;
        case 0x44:	//44 INC eSP 
            incSp();
        break;
        case 0x45:	//45 INC eBP 
            incBp();    
        break;
        case 0x46:	//46 INC eSI 
            incSi();
        break;
        case 0x47:	//47 INC eDI 
            incDi();
        break;
        case 0x48:	//48 DEC eAX 
            decAx();
        break;
        case 0x49:	//49 DEC eCX 
            decCx(); 
        break;
        case 0x4A:	//4A DEC eDX 
            decDx();
        break;
        case 0x4B:	//4B DEC eBX 
            decBx();
        break;
        case 0x4C:	//4C DEC eSP 
            decSp();
        break;
        case 0x4D:	//4D DEC eBP 
            decBp();
        break;
        case 0x4E:	//4E DEC eSI 
            decSi();
        break;
        case 0x4F:	//4F DEC eDI 
            decDi();
        break;
        case 0x50:	//50 PUSH eAX 
            pushAx();
        break;
        case 0x51:	//51 PUSH eCX 
            pushCx();
        break;
        case 0x52:	//52 PUSH eDX 
            pushDx();
        break;
        case 0x53:	//53 PUSH eBX 
            pushBx();
        break;
        case 0x54:	//54 PUSH eSP 
            pushSp();
        break;
        case 0x55:	//55 PUSH eBP 
            pushBp();
        break;
        case 0x56:	//56 PUSH eSI 
            pushSi();
        break;
        case 0x57:	//57 PUSH eDI 
            pushDi();
        break;
        case 0x58:	//58 POP eAX 
            popAx();
        break;
        case 0x59:	//59 POP eCX 
            popCx();
        break;
        case 0x5A:	//5A POP eDX 
            popDx();
        break;
        case 0x5B:	//5B POP eBX 
            popBx();
        break;
        case 0x5C:	//5C POP eSP 
            popSp();
        break;
        case 0x5D:	//5D POP eBP 
            popBp();
        break;
        case 0x5E:	//5E POP eSI 
            popSi();
        break;
        case 0x5F:	//5F POP eDI 
            popDi();
        break;
        case 0x60:	//60 -- 
            assert(false);
        break;
        case 0x61:	//61 -- 
            assert(false);
        break;
        case 0x62:	//62 -- 
            assert(false);
        break;
        case 0x63:	//63 -- 
            assert(false);
        break;
        case 0x64:	//64 -- 
            assert(false);
        break;
        case 0x65:	//65 -- 
            assert(false);
        break;
        case 0x66:	//66 -- 
            assert(false);
        break;
        case 0x67:	//67 -- 
            assert(false);
        break;
        case 0x68:	//68 -- 
            assert(false);
        break;
        case 0x69:	//69 -- 
            assert(false);
        break;
        case 0x6A:	//6A -- 
            assert(false);
        break;
        case 0x6B:	//6B -- 
            assert(false);
        break;
        case 0x6C:	//6C -- 
            assert(false);
        break;
        case 0x6D:	//6D -- 
            assert(false);
        break;
        case 0x6E:	//6E -- 
            assert(false);
        break;
        case 0x6F:	//6F -- 
        break;
        case 0x70:	//70 JO Jb 
            jo();
        break;
        case 0x71:	//71 JNO Jb 
            jno();
        break;
        case 0x72:	//72 JB Jb 
            jb();
        break;
        case 0x73:	//73 JNB Jb 
            jnb();
        break;
        case 0x74:	//74 JZ Jb 
            jz();
        break;
        case 0x75:	//75 JNZ Jb 
            jnz();
        break;
        case 0x76:	//76 JBE Jb 
            jbe();
        break;
        case 0x77:	//77 JA Jb 
            ja();
        break;
        case 0x78:	//78 JS Jb 
            js();
        break;
        case 0x79:	//79 JNS Jb 
            jns();
        break;
        case 0x7A:	//7A JPE Jb 
            jpe();
        break;
        case 0x7B:	//7B JPO Jb 
            jpo();
        break;
        case 0x7C:	//7C JL Jb 
            jl();
        break;
        case 0x7D:	//7D JGE Jb 
            jge();
        break;
        case 0x7E:	//7E JLE Jb 
            jle();
        break;
        case 0x7F:	//7F JG Jb 
            jg();
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
            regs.ip = (ubyte) ((memory.getRawData<ubyte, 1, 0>(IPReal) + 2) + regs.ip);
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
            regs.flags[CF] = true;
            regs.ip++;
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
        case DS_OVR:
            return regs.ds;
        case CS_OVR:
            return regs.cs;
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

    /* Raw coded address */
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
            }
            else {
                lhs_b = byteRegs[mod.rm.to_ulong()];
                rhs_b = byteRegs[mod.reg.to_ulong()];
            }
        }
        
        else {
           
            if(isWord) {
                lhs_w = wordRegs[mod.reg.to_ulong()];
                rhs_w = wordRegs[mod.rm.to_ulong()];
            }
            else {
                lhs_b = byteRegs[mod.reg.to_ulong()];
                rhs_b = byteRegs[mod.rm.to_ulong()];
            }
        }
        
        return;
    }


    if(toReg) {
        if(isWord) {
            lhs_w = wordRegs[mod.reg.to_ulong()];
            rhs_w = (uword*) &memory[getAbsoluteAddressModRM()]; 
        }
        else {
            lhs_b = byteRegs[mod.reg.to_ulong()];
            rhs_b = &memory[getAbsoluteAddressModRM()];
        }
        return;
    }
    else {
        if(isWord) {
            lhs_w = (uword*) &memory[getAbsoluteAddressModRM()];
            rhs_w = wordRegs[mod.reg.to_ulong()];
        }
        else {
            lhs_b = &memory[getAbsoluteAddressModRM()];
            rhs_b = byteRegs[mod.reg.to_ulong()];
        }
        return;
    }

}



/* Increment IP */
void CPU::incIP(unsigned int steps) {
    regs.ip += steps; 
}

/* Generate psy. address */
dword CPU::getAbs(uword seg, uword off) {
   
    dword absAddr = (seg << 4) + off;
  
    /* If overflow in 20-bit address, then wrap around */
    if(absAddr >= DEFAULT_MEMORY_SIZE)
        return absAddr % DEFAULT_MEMORY_SIZE;
    else
        return absAddr;

}

template<typename T, unsigned int size>
void CPU::push(T val) {
    regs.sp -= size;
    T* topPtr = (T*) &memory[getAbs(regs.ss, regs.sp)];
    *topPtr = val;
}

template<typename T, unsigned int size>
T CPU::pop() {
    T val = *((T*) &memory[getAbs(regs.ss, regs.sp)]);
    regs.sp += size;
    return val;
}

/* Set carry, sign, zero, parity flags */

template<typename T, bool withCarry>
void CPU::flagCSZP(T val) {

    const bool isWord = std::is_same<T, sword>() || std::is_same<T, uword>();
     
    regs.flags[ZF] = val == 0;

    if(isWord) {
      
        if(withCarry) {
            regs.flags[CF] = tmp_d & 0xffff0000;
        }

        regs.flags[SF] = val & 0x8000;
        regs.flags[PF] = parity[val & 0xff];
    }
    else {
        
        if(withCarry) {
            regs.flags[CF] = tmp_w & 0xff00;
        } 

        regs.flags[SF] = val & 0x80;
        regs.flags[PF] = parity[val];
    }


}

/* Logic operations always clear CF and OF */

template<typename T>
void CPU::flagLogic(T val) {
    flagCSZP<T, false>(val);
    regs.flags[CF] = 0;
    regs.flags[OF] = 0;
}


// 00
void CPU::addEbGb() {       
    mod.decode(memory[IPReal+1]);
    setOperands<false, false>();
    tmp_w = (uword) *lhs_b + (uword) *rhs_b;
    
    regs.flags[OF] = ((tmp_w ^ *lhs_b) & (tmp_w ^ *rhs_b) & 0x80) == 0x80;
    regs.flags[AF] = ((*lhs_b ^ *rhs_b ^ tmp_w) & 0x10 ) == 0x10;

    *lhs_b += *rhs_b;
    flagCSZP<ubyte>(*lhs_b);
    incIP(mod.getModInstrSize(ovrd.getOverrideCount()));
}

// 01
void CPU::addEvGv() {
    mod.decode(memory[IPReal+1]);
    setOperands<false, true>();
    tmp_d = ((dword) *lhs_w) + ((dword) *rhs_w);

    regs.flags[OF] = ((tmp_d ^ *lhs_w) & (tmp_d ^ *rhs_w) & 0x8000) == 0x8000;
    regs.flags[AF] = ((*lhs_w ^ *rhs_w ^ tmp_d) & 0x10 ) == 0x10;

    *lhs_w += *rhs_w;
    flagCSZP<uword>(*lhs_w);
    incIP(mod.getModInstrSize(ovrd.getOverrideCount()));
}

// 02
void CPU::addGbEb() {
    mod.decode(memory[IPReal+1]);
    setOperands<true, false>();
    tmp_w = ((uword) *lhs_b) + ((uword) *rhs_b);

    regs.flags[OF] = ((tmp_w ^ *lhs_b) & (tmp_w ^ *rhs_b) & 0x80) == 0x80;
    regs.flags[AF] = ((*lhs_b ^ *rhs_b ^ tmp_w) & 0x10 ) == 0x10;

    *lhs_b += *rhs_b;
    flagCSZP<ubyte>(*lhs_b); 
    incIP(mod.getModInstrSize(ovrd.getOverrideCount()));
}

// 03
void CPU::addGvEv() {
    mod.decode(memory[IPReal+1]);
    setOperands<true, true>();
    tmp_d = ((dword) *lhs_w) + ((dword) *rhs_w);

    regs.flags[OF] = ((tmp_d ^ *lhs_w) & (tmp_d ^ *rhs_w) & 0x8000) == 0x8000;
    regs.flags[AF] = ((*lhs_w ^ *rhs_w ^ tmp_d) & 0x10 ) == 0x10;

    *lhs_w += *rhs_w;
    flagCSZP<uword>(*lhs_w); 
    incIP(mod.getModInstrSize(ovrd.getOverrideCount()));
}

// 04
void CPU::addAlIb() {
    uint8_t val = memory.getRawData<ubyte, 1, 0>(IPReal);
    tmp_w = ((uword) regs.al) + ((uword) val);

    regs.flags[OF] = ((tmp_w ^ regs.al) & (tmp_w ^ val) & 0x80) == 0x80;
    regs.flags[AF] = ((regs.al ^ val ^ tmp_w) & 0x10 ) == 0x10;

    regs.al += val;
    flagCSZP<ubyte>(regs.al);
    incIP(2);
}

// 05 
void CPU::addAxIv() {
    uint16_t val = memory.getRawData<uword, 2, 1>(IPReal);
    tmp_d = ((dword) regs.ax) + ((dword) val);

    regs.flags[OF] = ((tmp_d ^ regs.ax) & (tmp_d ^ val) & 0x8000) == 0x8000;
    regs.flags[AF] = ((regs.ax ^ val ^ tmp_d) & 0x10 ) == 0x10;

    regs.ax += val;
    flagCSZP<uword>(regs.ax);
    incIP(3);
}

// 06
void CPU::pushEs() {
    push<uword, 2>(regs.es);
    incIP(1);
}

// 07
void CPU::popEs() {
    regs.es = pop<uword, 2>();
    incIP(1);
}

// 08
void CPU::orEbGb() {
    mod.decode(memory[IPReal+1]);
    setOperands<false, false>();
    *lhs_b |= *rhs_b;
    flagLogic<ubyte>(*lhs_b);
    incIP(mod.getModInstrSize(ovrd.getOverrideCount()));
}

// 09
void CPU::orEvGv() {
    mod.decode(memory[IPReal+1]);
    setOperands<false, true>();
    *lhs_w |= *rhs_w;
    flagLogic<uword>(*lhs_w);
    incIP(mod.getModInstrSize(ovrd.getOverrideCount()));
}

// 0A
void CPU::orGbEb() {
    mod.decode(memory[IPReal+1]);
    setOperands<true, false>();
    *lhs_b |= *rhs_b;
    flagLogic<ubyte>(*lhs_b);
    incIP(mod.getModInstrSize(ovrd.getOverrideCount()));
}

// 0B
void CPU::orGvEv() {
    mod.decode(memory[IPReal+1]);
    setOperands<true, true>();
    *lhs_w |= *rhs_w;
    flagLogic<ubyte>(*lhs_w);
    incIP(mod.getModInstrSize(ovrd.getOverrideCount()));
}

// 0C
void CPU::orAlIb() {
    uint8_t val = memory.getRawData<ubyte, 1, 0>(IPReal);
    regs.al |= val;
    flagLogic<ubyte>(regs.al);
    incIP(2);
}

// 0D
void CPU::orAxIv() {
    uint16_t val = memory.getRawData<uword, 2, 1>(IPReal);
    regs.ax |= val;
    flagLogic<uword>(regs.ax);
    incIP(3);
}

// 0E
void CPU::pushCs() {
    push<uword, 2>(regs.cs);
    incIP(1);
}

// 10
void CPU::adcEbGb() {
    mod.decode(memory[IPReal+1]);
    setOperands<false, false>();
    tmp_w = (uword) *lhs_b + (uword) *rhs_b + regs.flags[CF];

    regs.flags[OF] = ((tmp_w ^ *lhs_b) & (tmp_w ^ *rhs_b) & 0x80) == 0x80;
    regs.flags[AF] = ((*lhs_b ^ *rhs_b ^ tmp_w) & 0x10) == 0x10;

    *lhs_b = *lhs_b + *rhs_b + regs.flags[CF];
    flagCSZP<ubyte>(*lhs_b);
    incIP(mod.getModInstrSize(ovrd.getOverrideCount()));
}

// 11
void CPU::adcEvGv() {
    mod.decode(memory[IPReal+1]);
    setOperands<false, true>();
    tmp_d = *lhs_w + *rhs_w;

    regs.flags[OF] = ((tmp_d ^ *lhs_w) & (tmp_d ^ *rhs_w) & 0x8000) == 0x8000;
    regs.flags[AF] = ((*lhs_w ^ *rhs_w ^ tmp_d) & 0x10) == 0x10;

    *lhs_w = *lhs_w + *rhs_w + regs.flags[CF];
    flagCSZP<uword>(*lhs_w);
    incIP(mod.getModInstrSize(ovrd.getOverrideCount()));
}

// 12
void CPU::adcGbEb() {
    mod.decode(memory[IPReal+1]);
    setOperands<true, false>();
    tmp_w = *lhs_b + *rhs_b;

    regs.flags[OF] = ((tmp_w ^ *lhs_b) & (tmp_w ^ *rhs_b) & 0x80) == 0x80;
    regs.flags[AF] = ((*lhs_b ^ *rhs_b ^ tmp_w) & 0x10) == 0x10;

    *lhs_b = *lhs_b + *rhs_b + regs.flags[CF];
    flagCSZP<ubyte>(*lhs_b);
    incIP(mod.getModInstrSize(ovrd.getOverrideCount()));
}

// 13
void CPU::adcGvEv() {
    mod.decode(memory[IPReal+1]);
    setOperands<true, true>();
    tmp_d = *lhs_w + *rhs_w;

    regs.flags[OF] = ((tmp_d ^ *lhs_w) & (tmp_d ^ *rhs_w) & 0x8000) == 0x8000;
    regs.flags[AF] = ((*lhs_w ^ *rhs_w ^ tmp_d) & 0x10) == 0x10;

    *lhs_w = *lhs_w + *rhs_w + regs.flags[CF];
    flagCSZP<uword>(*lhs_w);
    incIP(mod.getModInstrSize(ovrd.getOverrideCount()));

}

// 14
void CPU::adcAlIb() {
    uint8_t val = memory.getRawData<ubyte, 1, 0>(IPReal);
    tmp_w = regs.al + val + regs.flags[CF];

    regs.flags[OF] = ((tmp_w ^ regs.al) & (tmp_w ^ val) & 0x80) == 0x80;
    regs.flags[AF] = ((regs.al ^ val ^ tmp_w) & 0x10) == 0x10;

    regs.al = regs.al + val + regs.flags[CF];
    flagCSZP<ubyte>(regs.al);
    incIP(2);
}

// 15
void CPU::adcAxIv() {
    uint16_t val = memory.getRawData<uword, 2, 1>(IPReal);
    tmp_d = regs.ax + val + regs.flags[CF];

    regs.flags[OF] = ((tmp_d ^ regs.ax) & (tmp_d ^ val) & 0x8000) == 0x8000;
    regs.flags[AF] = ((regs.ax ^ val ^ tmp_d) & 0x10) == 0x10;

    regs.ax = regs.ax + val + regs.flags[CF];
    flagCSZP<uword>(regs.ax);
    incIP(3);
}

// 16
void CPU::pushSs() {
    push<uword, 2>(regs.ss);
}

// 17
void CPU::popSs() {
    regs.ss = pop<uword, 2>();
}

// 18

void CPU::sbbEbGb() {
    mod.decode(memory[IPReal+1]);
    setOperands<false, false>();
    tmp_w = *lhs_b - (*rhs_b + regs.flags[CF]);

    regs.flags[OF] = ((tmp_w ^ *lhs_b) & (*lhs_b ^ *rhs_b) & 0x80);
    regs.flags[AF] = ((*lhs_b ^ *rhs_b ^ tmp_w) & 0x10);

    *lhs_b = *lhs_b - (*rhs_b + regs.flags[CF]);
    flagCSZP<ubyte>(*lhs_b);
    incIP(mod.getModInstrSize(ovrd.getOverrideCount()));
}

// 19
void CPU::sbbEvGv() {
    mod.decode(memory[IPReal+1]);
    setOperands<false, true>();
    tmp_d = *lhs_w - (*rhs_w + regs.flags[CF]);

    regs.flags[OF] = ((tmp_d ^ *lhs_w) & (*lhs_w ^ *rhs_w) & 0x8000);
    regs.flags[AF] = ((*lhs_w ^ *rhs_w ^ tmp_d) & 0x10);

    *lhs_w = *lhs_w - (*rhs_w + regs.flags[CF]);
    flagCSZP<uword>(*lhs_w);
    incIP(mod.getModInstrSize(ovrd.getOverrideCount()));
}

// 1A
void CPU::sbbGbEb() {
    mod.decode(memory[IPReal+1]);
    setOperands<true, false>();
    tmp_w = *lhs_b - (*rhs_b + regs.flags[CF]);

    regs.flags[OF] = ((tmp_w ^ *lhs_b) & (*lhs_b ^ *rhs_b) & 0x80);
    regs.flags[AF] = ((*lhs_b ^ *rhs_b ^ tmp_w) & 0x10);

    *lhs_b = *lhs_b - (*rhs_b + regs.flags[CF]);
    flagCSZP<ubyte>(*lhs_b);
    incIP(mod.getModInstrSize(ovrd.getOverrideCount()));
}

// 1B
void CPU::sbbGvEv() {
    mod.decode(memory[IPReal+1]);
    setOperands<true, true>();
    tmp_d = *lhs_w - (*rhs_w + regs.flags[CF]);

    regs.flags[OF] = ((tmp_d ^ *lhs_w) & (*lhs_w ^ *rhs_w) & 0x8000);
    regs.flags[AF] = ((*lhs_w ^ *rhs_w ^ tmp_d) & 0x10);

    *lhs_w = *lhs_w - (*rhs_w + regs.flags[CF]);
    flagCSZP<uword>(*lhs_w);
    incIP(mod.getModInstrSize(ovrd.getOverrideCount()));
}

// 1C
void CPU::sbbAlIb() {
    uint8_t val = memory.getRawData<ubyte, 1, 0>(IPReal);
    tmp_w = regs.al - ( val + regs.flags[CF] );

    regs.flags[OF] = ((tmp_w ^ regs.al) & (regs.al ^ val) & 0x80);
    regs.flags[AF] = ((regs.al ^ val ^ tmp_w) & 0x10);
   
    regs.al = regs.al - ( val + regs.flags[CF] );
    flagCSZP<ubyte>(regs.al);
    incIP(2);
}

// 1D
void CPU::sbbAxIv() {
    uint16_t val = memory.getRawData<uword, 2, 1>(IPReal);
    tmp_d = regs.ax - ( val + regs.flags[CF] );

    regs.flags[OF] = ((tmp_d ^ regs.ax) & (regs.ax ^ val) & 0x80);
    regs.flags[AF] = ((regs.ax ^ val ^ tmp_d) & 0x10);

    regs.ax = regs.ax - ( val + regs.flags[CF] );
    flagCSZP<uword>(regs.ax);
    incIP(3);
}

// 1E
void CPU::pushDs() {
    push<uword, 2>(regs.ds);
}

// 1F
void CPU::popDs() {
    regs.ds = pop<uword, 2>();
}

// 20
void CPU::andEbGb() {
    mod.decode(memory[IPReal+1]);
    setOperands<false, false>();
    tmp_w = *lhs_b & *rhs_b;

    *lhs_b &= *rhs_b;
    flagLogic<ubyte>(*lhs_b);
    incIP(mod.getModInstrSize(ovrd.getOverrideCount()));
}

// 21
void CPU::andEvGv() {
    mod.decode(memory[IPReal+1]);
    setOperands<false, true>();
    tmp_d = *lhs_w & *rhs_w;

    *lhs_w &= *rhs_w;
    flagLogic<ubyte>(*lhs_w);
    incIP(mod.getModInstrSize(ovrd.getOverrideCount()));
}

// 22
void CPU::andGbEb() {
    mod.decode(memory[IPReal+1]);
    setOperands<true, false>();
    tmp_w = *lhs_b & *rhs_b;

    *lhs_b &= *rhs_b;
    flagLogic<ubyte>(*lhs_b);
    incIP(mod.getModInstrSize(ovrd.getOverrideCount()));
}

// 23
void CPU::andGvEv() {
    mod.decode(memory[IPReal+1]);
    setOperands<true, true>();
    tmp_d = *lhs_w & *rhs_w;

    *lhs_w &= *rhs_w;
    flagLogic<ubyte>(*lhs_w);
    incIP(mod.getModInstrSize(ovrd.getOverrideCount()));
}

// 24
void CPU::andAlIb() {
    uint8_t val = memory.getRawData<ubyte, 1, 0>(IPReal);
    tmp_w = regs.al & val;

    regs.al &= val;
    flagLogic<ubyte>(regs.al);
    incIP(2);
}

// 25
void CPU::andAxIv() {
    uint16_t val = memory.getRawData<uword, 2, 1>(IPReal);
    tmp_d = regs.ax & val;

    regs.ax &= val;
    flagLogic<ubyte>(regs.ax);
    incIP(3);
}

// 27
void CPU::daa() {
    if( ((regs.al & 0xf) > 9) || regs.flags[AF] ) {
        tmp_w = (regs.al + 6);
        regs.al = tmp_w & 255;
        regs.flags[CF] = tmp_w & 0xFF00;
        regs.flags[AF] = true;
    }

    if( (regs.al > 0x9f) || regs.flags[CF] ) {
        regs.al += 0x60;
        regs.flags[CF] = true;
    }

    regs.al &= 255;
    flagCSZP<ubyte, false>(regs.al);
    incIP(1); 
}

// 28
void CPU::subEbGb() {
    mod.decode(memory[IPReal+1]);
    setOperands<false, false>();
    tmp_w = *lhs_b - *rhs_b;

    regs.flags[OF] = ((tmp_w ^ *lhs_b) & (*lhs_b ^ *rhs_b) & 0x80);
    regs.flags[AF] = ((*lhs_b ^ *rhs_b ^ tmp_w) & 0x10);

    *lhs_b -= *rhs_b;
    flagCSZP<ubyte>(*lhs_b);
    incIP(mod.getModInstrSize(ovrd.getOverrideCount()));
}

// 29
void CPU::subEvGv() {
    mod.decode(memory[IPReal+1]);
    setOperands<false, true>();
    tmp_d = *lhs_w - *rhs_w;

    regs.flags[OF] = ((tmp_d ^ *lhs_w) & (*lhs_w ^ *rhs_w) & 0x8000);
    regs.flags[AF] = ((*lhs_w ^ *rhs_w ^ tmp_d) & 0x10);

    *lhs_w -= *rhs_w;
    flagCSZP<uword>(*lhs_w);
    incIP(mod.getModInstrSize(ovrd.getOverrideCount()));
}

// 2A
void CPU::subGbEb() {
    mod.decode(memory[IPReal+1]);
    setOperands<true, false>();
    tmp_w = *lhs_b - *rhs_b;

    regs.flags[OF] = ((tmp_w ^ *lhs_b) & (*lhs_b ^ *rhs_b) & 0x80);
    regs.flags[AF] = ((*lhs_b ^ *rhs_b ^ tmp_w) & 0x10);

    *lhs_b -= *rhs_b;
    flagCSZP<ubyte>(*lhs_b);
    incIP(mod.getModInstrSize(ovrd.getOverrideCount()));
}

// 2B
void CPU::subGvEv() {
    mod.decode(memory[IPReal+1]);
    setOperands<true, true>();
    tmp_d = *lhs_w - *rhs_w;

    regs.flags[OF] = ((tmp_d ^ *lhs_w) & (*lhs_w ^ *rhs_w) & 0x8000);
    regs.flags[AF] = ((*lhs_w ^ *rhs_w ^ tmp_d) & 0x10);

    *lhs_w -= *rhs_w;
    flagCSZP<uword>(*lhs_w);
    incIP(mod.getModInstrSize(ovrd.getOverrideCount()));
}


// 2C
void CPU::subAlIb() {
    uint8_t val = memory.getRawData<ubyte, 1, 0>(IPReal);
    tmp_w = regs.al - val;

    regs.flags[OF] = ((tmp_w ^ regs.al) & (regs.al ^ val) & 0x80);
    regs.flags[AF] = ((regs.al ^ val ^ tmp_w) & 0x10);

    regs.al -= val;
    flagCSZP<ubyte>(regs.al);
    incIP(2);
}

// 2D
void CPU::subAxIv() {
    uint16_t val = memory.getRawData<uword, 2, 1>(IPReal);
    tmp_d = regs.ax - val;

    regs.flags[OF] = ((tmp_d ^ regs.ax) & (regs.ax ^ val) & 0x8000);
    regs.flags[AF] = ((regs.ax ^ val ^ tmp_d) & 0x10);

    regs.ax -= val;
    flagCSZP<uword>(regs.ax);
    incIP(3);
}

// 2F
void CPU::das() {
    if( ((regs.al & 15) > 9) || regs.flags[AF] ) {
         tmp_w = regs.al - 6;
         regs.al = tmp_w & 255;
         regs.flags[CF] = tmp_w & 0xff00;
         regs.flags[AF] = true;
    }
    else 
        regs.flags[AF] = true;

    if( ((regs.al & 0xf0) > 0x90) || regs.flags[CF] ) {
        regs.al -= 0x60;
        regs.flags[CF] = true;
    }
    else
        regs.flags[CF] = false;

    flagCSZP<ubyte, false>(regs.al);
    incIP(1);
}


// 30
void CPU::xorEbGb() {
    mod.decode(memory[IPReal+1]);
    setOperands<false, false>();
    *lhs_b ^= *rhs_b;
    flagLogic<ubyte>(*lhs_b);
    incIP(mod.getModInstrSize(ovrd.getOverrideCount()));
}

// 31
void CPU::xorEvGv() {
    mod.decode(memory[IPReal+1]);
    setOperands<false, true>();
    *lhs_w ^= *rhs_w;
    flagLogic<uword>(*lhs_w);
    incIP(mod.getModInstrSize(ovrd.getOverrideCount()));
}

// 32
void CPU::xorGbEb() {
    mod.decode(memory[IPReal+1]);
    setOperands<true, false>();
    *lhs_b ^= *rhs_b;
    flagLogic<ubyte>(*lhs_b);
    incIP(mod.getModInstrSize(ovrd.getOverrideCount()));
}

// 33
void CPU::xorGvEv() {
    mod.decode(memory[IPReal+1]);
    setOperands<true, true>();
    *lhs_w ^= *rhs_w;
    flagLogic<uword>(*lhs_w);
    incIP(mod.getModInstrSize(ovrd.getOverrideCount()));
}

// 34
void CPU::xorAlIb() {
    uint8_t val = memory.getRawData<ubyte, 1, 0>(IPReal);
    regs.al ^= val;
    flagLogic<ubyte>(regs.al);
    incIP(2);
}

// 35
void CPU::xorAxIv() {
    uint16_t val = memory.getRawData<uword, 2, 1>(IPReal);
    regs.ax ^= val;
    flagLogic<uword>(regs.ax);
    incIP(3);
}

// 37
void CPU::aaa() {
    if( ((regs.al & 0xf) > 9) || regs.flags[AF] ) {
        regs.al += 6;
        regs.ah += 1;
        regs.flags[AF] = regs.flags[CF] = true;
    }
    else 
        regs.flags[AF] = regs.flags[CF] = false; 
    regs.al = regs.al & 0xf;
    incIP(1);
}

// 38
void CPU::cmpEbGb() {
    mod.decode(memory[IPReal+1]);
    setOperands<false, false>();
    tmp_w = *lhs_b - *rhs_b;

    regs.flags[OF] = ((tmp_w ^ *lhs_b) & (*lhs_b ^ *rhs_b) & 0x80);
    regs.flags[AF] = ((*lhs_b ^ *rhs_b ^ tmp_w) & 0x10);

    uint8_t tmp = *lhs_b - *rhs_b;
    flagCSZP<ubyte>(tmp);
    incIP(mod.getModInstrSize(ovrd.getOverrideCount()));
}

// 39
void CPU::cmpEvGv() {
    mod.decode(memory[IPReal+1]);
    setOperands<false, true>();
    tmp_d = *lhs_w - *rhs_w;

    regs.flags[OF] = ((tmp_d ^ *lhs_w) & (*lhs_w ^ *rhs_w) & 0x8000);
    regs.flags[AF] = ((*lhs_w ^ *rhs_w ^ tmp_d) & 0x10);

    uint16_t tmp = *lhs_w - *rhs_w;
    flagCSZP<uword>(tmp);
    incIP(mod.getModInstrSize(ovrd.getOverrideCount()));
}

// 3A
void CPU::cmpGbEb() {
    mod.decode(memory[IPReal+1]);
    setOperands<true, false>();
    tmp_w = *lhs_b - *rhs_b;

    regs.flags[OF] = ((tmp_w ^ *lhs_b) & (*lhs_b ^ *rhs_b) & 0x80);
    regs.flags[AF] = ((*lhs_b ^ *rhs_b ^ tmp_w) & 0x10);

    uint8_t tmp = *lhs_b - *rhs_b;
    flagCSZP<ubyte>(tmp);
    incIP(mod.getModInstrSize(ovrd.getOverrideCount()));
}


// 3B 
void CPU::cmpGvEv() {
    mod.decode(memory[IPReal+1]);
    setOperands<true, true>();
    tmp_d = *lhs_w - *rhs_w;

    regs.flags[OF] = ((tmp_d ^ *lhs_w) & (*lhs_w ^ *rhs_w) & 0x8000);
    regs.flags[AF] = ((*lhs_w ^ *rhs_w ^ tmp_d) & 0x10);

    uint16_t tmp = *lhs_w - *rhs_w;
    flagCSZP<uword>(tmp);
    incIP(mod.getModInstrSize(ovrd.getOverrideCount()));
}

// 3C
void CPU::cmpAlIb() {
    uint8_t val = memory.getRawData<ubyte, 1, 0>(IPReal);
    tmp_w = regs.al - val;

    regs.flags[OF] = ((tmp_w ^ regs.al) & (regs.al ^ val) & 0x80);
    regs.flags[AF] = ((regs.al ^ val ^ tmp_w) & 0x10);

    uint8_t tmp = regs.al - val;
    flagCSZP<ubyte>(tmp);
    incIP(2);
}

// 3D
void CPU::cmpAxIv() {
    uint16_t val = memory.getRawData<uword, 2, 1>(IPReal);
    tmp_d = regs.ax - val;

    regs.flags[OF] = ((tmp_d ^ regs.ax) & (regs.ax ^ val) & 0x8000);
    regs.flags[AF] = ((regs.ax ^ val ^ tmp_d) & 0x10);

    uint16_t tmp = regs.ax - val;
    flagCSZP<uword>(tmp);
    incIP(3); 
}

void CPU::aas() {
    if(((regs.al & 0xf) > 9) || regs.flags[AF]) {
        regs.al -= 6;
        regs.ah -= 1;
        regs.flags[AF] = regs.flags[CF] = true;
    }
    else 
        regs.flags[AF] = regs.flags[CF] = false; 
    regs.al = regs.al & 0xf;
    incIP(1);
}

// 40
void CPU::incAx() {
    tmp_d = regs.ax + 1;

    regs.flags[OF] = ((tmp_d ^ regs.ax) & (tmp_d ^ 1) & 0x8000) == 0x8000;
    regs.flags[AF] = ((regs.ax ^ 1 ^ tmp_d) & 0x10) == 0x10;

    regs.ax += 1;
    flagCSZP<uword, false>(regs.ax);
    incIP(1);

}

// 41
void CPU::incCx() {
    tmp_d = regs.cx + 1;
    
    regs.flags[OF] = ((tmp_d ^ regs.cx) & (tmp_d ^ 1) & 0x8000) == 0x8000;
    regs.flags[AF] = ((regs.cx ^ 1 ^ tmp_d) & 0x10) == 0x10;

    regs.cx += 1;
    flagCSZP<uword, false>(regs.cx);
    incIP(1);
}

// 42
void CPU::incDx() {
    tmp_d = regs.dx + 1;

    regs.flags[OF] = ((tmp_d ^ regs.dx) & (tmp_d ^ 1) & 0x8000) == 0x8000;
    regs.flags[AF] = ((regs.dx ^ 1 ^ tmp_d) & 0x10) == 0x10;

    regs.dx += 1;
    flagCSZP<uword, false>(regs.dx);
    incIP(1);
}

// 43
void CPU::incBx() {
    tmp_d = regs.bx + 1;

    regs.flags[OF] = ((tmp_d ^ regs.bx) & (tmp_d ^ 1) & 0x8000) == 0x8000;
    regs.flags[AF] = ((regs.bx ^ 1 ^ tmp_d) & 0x10) == 0x10;

    regs.bx += 1;
    flagCSZP<uword, false>(regs.bx);
    incIP(1);
}

// 44
void CPU::incSp() {
    tmp_d = regs.sp + 1;

    regs.flags[OF] = ((tmp_d ^ regs.sp) & (tmp_d ^ 1) & 0x8000) == 0x8000;
    regs.flags[AF] = ((regs.sp ^ 1 ^ tmp_d) & 0x10) == 0x10;

    regs.sp += 1;
    flagCSZP<uword, false>(regs.sp);
    incIP(1);
}

// 45
void CPU::incBp() {
    tmp_d = regs.bp + 1;

    regs.flags[OF] = ((tmp_d ^ regs.bp) & (tmp_d ^ 1) & 0x8000) == 0x8000;
    regs.flags[AF] = ((regs.bp ^ 1 ^ tmp_d) & 0x10) == 0x10;

    regs.bp += 1;
    flagCSZP<uword, false>(regs.bp);
    incIP(1);
}

// 46
void CPU::incSi() {
    tmp_d = regs.si + 1;

    regs.flags[OF] = ((tmp_d ^ regs.si) & (tmp_d ^ 1) & 0x8000) == 0x8000;
    regs.flags[AF] = ((regs.si ^ 1 ^ tmp_d) & 0x10) == 0x10;

    regs.si += 1;
    flagCSZP<uword, false>(regs.si);
    incIP(1);
}

// 47
void CPU::incDi() {
    tmp_d = regs.di + 1;

    regs.flags[OF] = ((tmp_d ^ regs.di) & (tmp_d ^ 1) & 0x8000) == 0x8000;
    regs.flags[AF] = ((regs.di ^ 1 ^ tmp_d) & 0x10) == 0x10;

    regs.di += 1;
    flagCSZP<uword, false>(regs.di);
    incIP(1);
}

// 48
void CPU::decAx() {
    tmp_d = regs.ax - 1;

    regs.flags[OF] = ((tmp_d ^ regs.ax) & (tmp_d ^ 1) & 0x8000) == 0x8000;
    regs.flags[AF] = ((regs.ax ^ 1 ^ tmp_d) & 0x10) == 0x10;

    regs.ax -= 1;
    flagCSZP<uword, false>(regs.ax);
    incIP(1);

}

// 49
void CPU::decCx() {
    tmp_d = regs.cx - 1;
    
    regs.flags[OF] = ((tmp_d ^ regs.cx) & (tmp_d ^ 1) & 0x8000) == 0x8000;
    regs.flags[AF] = ((regs.cx ^ 1 ^ tmp_d) & 0x10) == 0x10;

    regs.cx -= 1;
    flagCSZP<uword, false>(regs.cx);
    incIP(1);
}

// 4A
void CPU::decDx() {
    tmp_d = regs.dx - 1;

    regs.flags[OF] = ((tmp_d ^ regs.dx) & (tmp_d ^ 1) & 0x8000) == 0x8000;
    regs.flags[AF] = ((regs.dx ^ 1 ^ tmp_d) & 0x10) == 0x10;

    regs.dx -= 1;
    flagCSZP<uword, false>(regs.dx);
    incIP(1);
}

// 4B
void CPU::decBx() {
    tmp_d = regs.bx - 1;

    regs.flags[OF] = ((tmp_d ^ regs.bx) & (tmp_d ^ 1) & 0x8000) == 0x8000;
    regs.flags[AF] = ((regs.bx ^ 1 ^ tmp_d) & 0x10) == 0x10;

    regs.bx -= 1;
    flagCSZP<uword, false>(regs.bx);
    incIP(1);
}

// 4C
void CPU::decSp() {
    tmp_d = regs.sp - 1;

    regs.flags[OF] = ((tmp_d ^ regs.sp) & (tmp_d ^ 1) & 0x8000) == 0x8000;
    regs.flags[AF] = ((regs.sp ^ 1 ^ tmp_d) & 0x10) == 0x10;

    regs.sp -= 1;
    flagCSZP<uword, false>(regs.sp);
    incIP(1);
}

// 4D
void CPU::decBp() {
    tmp_d = regs.bp - 1;

    regs.flags[OF] = ((tmp_d ^ regs.bp) & (tmp_d ^ 1) & 0x8000) == 0x8000;
    regs.flags[AF] = ((regs.bp ^ 1 ^ tmp_d) & 0x10) == 0x10;

    regs.bp -= 1;
    flagCSZP<uword, false>(regs.bp);
    incIP(1);
}

// 4E
void CPU::decSi() {
    tmp_d = regs.si - 1;

    regs.flags[OF] = ((tmp_d ^ regs.si) & (tmp_d ^ 1) & 0x8000) == 0x8000;
    regs.flags[AF] = ((regs.si ^ 1 ^ tmp_d) & 0x10) == 0x10;

    regs.si -= 1;
    flagCSZP<uword, false>(regs.si);
    incIP(1);
}

// 4F
void CPU::decDi() {
    tmp_d = regs.di - 1;

    regs.flags[OF] = ((tmp_d ^ regs.di) & (tmp_d ^ 1) & 0x8000) == 0x8000;
    regs.flags[AF] = ((regs.di ^ 1 ^ tmp_d) & 0x10) == 0x10;

    regs.di -= 1;
    flagCSZP<uword, false>(regs.di);
    incIP(1);
}


// 50
void CPU::pushAx() {
    push<uword, 2>(regs.ax);
    incIP(1);
}

// 51
void CPU::pushCx() {
    push<uword, 2>(regs.cx);
    incIP(1);
}

// 52
void CPU::pushDx() {
    push<uword, 2>(regs.dx);
    incIP(1);
}

// 53
void CPU::pushBx() {
    push<uword, 2>(regs.bx);
    incIP(1);
}

// 54
void CPU::pushSp() {
    push<uword, 2>(regs.sp);
    incIP(1);
}

// 55
void CPU::pushBp() {
    push<uword, 2>(regs.bp);
    incIP(1);
}

// 56
void CPU::pushSi() {
    push<uword, 2>(regs.si);
    incIP(1);
}

// 57
void CPU::pushDi() {
    push<uword, 2>(regs.di);
    incIP(1);
}

// 58
void CPU::popAx() {
    regs.ax = pop<uword, 2>();
    incIP(1);
}

// 59
void CPU::popCx() {
    regs.cx = pop<uword, 2>();
    incIP(1);
}

// 5A
void CPU::popDx() {
    regs.dx = pop<uword, 2>();
    incIP(1);
}

// 5B
void CPU::popBx() {
    regs.bx = pop<uword, 2>();
    incIP(1);
}

// 5C
void CPU::popSp() {
    regs.sp = pop<uword, 2>();
    incIP(1);
}

// 5D
void CPU::popBp() {
    regs.bp = pop<uword, 2>();
    incIP(1);
}

// 5E
void CPU::popSi() {
    regs.si = pop<uword, 2>();
    incIP(1);
}

// 5F
void CPU::popDi() {
    regs.di = pop<uword, 2>();
    incIP(1);
}

// 70
void CPU::jo() {
    if(regs.flags[OF]) 
        regs.ip = IPReal = (ubyte) ((memory.getRawData<ubyte, 1, 0>(IPReal) + 2) + regs.ip);
}

// 71
void CPU::jno() {
    if(!regs.flags[OF])
        regs.ip = IPReal = (ubyte) ((memory.getRawData<ubyte, 1, 0>(IPReal) + 2) + regs.ip);
}

// 72
void CPU::jb() {
    if(regs.flags[CF])
        regs.ip = IPReal = (ubyte) ((memory.getRawData<ubyte, 1, 0>(IPReal) + 2) + regs.ip);
}

// 73
void CPU::jnb() {
    if(!regs.flags[CF])
        regs.ip = IPReal = (ubyte) ((memory.getRawData<ubyte, 1, 0>(IPReal) + 2) + regs.ip);
}

// 74
void CPU::jz() {
    if(regs.flags[ZF])
        regs.ip = IPReal = (ubyte) ((memory.getRawData<ubyte, 1, 0>(IPReal) + 2) + regs.ip);
}

// 75
void CPU::jnz() {
    if(!regs.flags[ZF])
        regs.ip = IPReal = (ubyte) ((memory.getRawData<ubyte, 1, 0>(IPReal) + 2) + regs.ip);
}

// 76
void CPU::jbe() {
    if(regs.flags[CF] || regs.flags[ZF])
        regs.ip = IPReal = (ubyte) ((memory.getRawData<ubyte, 1, 0>(IPReal) + 2) + regs.ip);
}

// 77
void CPU::ja() {
    if(!regs.flags[CF] && !regs.flags[ZF])
        regs.ip = IPReal = (ubyte) ((memory.getRawData<ubyte, 1, 0>(IPReal) + 2) + regs.ip);
}

// 78
void CPU::js() {
    if(regs.flags[SF])
        regs.ip = IPReal = (ubyte) ((memory.getRawData<ubyte, 1, 0>(IPReal) + 2) + regs.ip);
}

// 79
void CPU::jns() {
    if(!regs.flags[SF])
        regs.ip = IPReal = (ubyte) ((memory.getRawData<ubyte, 1, 0>(IPReal) + 2) + regs.ip);
}

// 7A
void CPU::jpe() {
    if(regs.flags[PF])
        regs.ip = IPReal = (ubyte) ((memory.getRawData<ubyte, 1, 0>(IPReal) + 2) + regs.ip);
}

// 7B
void CPU::jpo() {
    if(!regs.flags[PF])
        regs.ip = IPReal = (ubyte) ((memory.getRawData<ubyte, 1, 0>(IPReal) + 2) + regs.ip);
}

// 7C
void CPU::jl() {
    if(regs.flags[SF] != regs.flags[OF])
        regs.ip = IPReal = (ubyte) ((memory.getRawData<ubyte, 1, 0>(IPReal) + 2) + regs.ip);
}

// 7D
void CPU::jge() {
    if(regs.flags[SF] == regs.flags[OF])
        regs.ip = IPReal = (ubyte) ((memory.getRawData<ubyte, 1, 0>(IPReal) + 2) + regs.ip);
}

// 7E
void CPU::jle() {
    if( (regs.flags[ZF] != regs.flags[OF]) || (regs.flags[SF] != regs.flags[OF]) )
        regs.ip = IPReal = (ubyte) ((memory.getRawData<ubyte, 1, 0>(IPReal) + 2) + regs.ip);
}

// 7F
void CPU::jg() {
    if(!regs.flags[ZF] && regs.flags[SF] == regs.flags[OF])
        regs.ip = IPReal = (ubyte) ((memory.getRawData<ubyte, 1, 0>(IPReal) + 2) + regs.ip);
}

void CPU::grp1EbIb() {
    mod.decode(memory[IPReal+1]);

}

