#ifndef DEFS_HPP
#define DEFS_HPP

#include <cstdint>
#include <cassert>

/* 
 * Memory config 
 * 1 MB memory
 */
#define DEFAULT_MEMORY_SIZE 0x100000

/* Standard integers used in project */
typedef int8_t   sbyte;
typedef uint8_t  ubyte;
typedef int16_t  sword;
typedef uint16_t uword;
typedef int32_t  sdword;
typedef uint32_t dword;

/* Prefixes opcodes */
#define ES_OVR      0x26
#define SS_OVR      0x36
#define CS_OVR      0x2e
#define DS_OVR      0x3e
#define REPZ_OVR    0xf2
#define REPNZ_OVR   0xf3
#define LOCK_OVR    0xf0

/* Displacement mode of ModRM byte */
#define NO_DISPLACEMENT     0b00
#define BYTE_DISPLACEMENT   0b01
#define WORD_DISPLACEMENT   0b10
#define REGISTER_MODE       0b11

/* ModRM Stuff */
#define BX_SI       0b000
#define BX_DI       0b001
#define BP_SI       0b010
#define BP_DI       0b011
#define SI          0b100
#define DI          0b101
#define UWORD       0b110
#define BP          0b110
#define BX          0b111

#define AL_AX       0b000
#define CL_CX       0b001
#define DL_DX       0b010
#define BL_BX       0b011
#define AH_SP       0b100
#define CH_BP       0b101
#define DH_SI       0b110
#define BH_DI       0b111

#define ES AL_AX
#define CS CL_CX
#define SS DL_DX
#define DS BL_BX

/* Positions in FLAGS register */
#define CF 0
#define PF 2
#define AF 4
#define ZF 6
#define SF 7
#define TF 8
#define IF 9
#define DF 10
#define OF 11

#endif
