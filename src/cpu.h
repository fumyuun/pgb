#ifndef CPU_H
#define CPU_H

#include "mem.h"

typedef union
{
    struct
    {
#ifdef B_END
        uint8_t h, l;
#else
        uint8_t l, h;
#endif
    } r8;
    uint16_t r16;
} r16_t;

typedef enum
{
	B,
	C,
	D,
	E,
	H,
	L,
	A,
	F,
	_HL_,
	_C_,
	_BC_,
	_DE_
} reg8_e;

typedef enum
{
	AF,
	BC,
	DE,
	HL,
	SP,
	PC
} reg16_e;

typedef struct _cpu_t
{
    int *run;
    mem_t *mem;
    r16_t regs[6];
    int cycles;
} cpu_t;

#define reg_a   regs[AF].r8.h
#define reg_f   regs[AF].r8.l
#define reg_b   regs[BC].r8.h
#define reg_c   regs[BC].r8.l
#define reg_d   regs[DE].r8.h
#define reg_e   regs[DE].r8.l
#define reg_h   regs[HL].r8.h
#define reg_l   regs[HL].r8.l

#define reg_af  regs[AF].r16
#define reg_bc  regs[BC].r16
#define reg_de  regs[DE].r16
#define reg_hl  regs[HL].r16
#define reg_sp  regs[SP].r16
#define reg_pc  regs[PC].r16

#define FLAG_Z  0x80
#define FLAG_N  0x40
#define FLAG_H  0x20
#define FLAG_C  0x10

#define FLAG_I_JOYPAD   0x10
#define FLAG_I_SERIAL   0x08
#define FLAG_I_TIMER    0x04
#define FLAG_I_LCDSTAT  0x02
#define FLAG_I_VBLANK   0x01

#define CC_NZ   0x00
#define CC_Z    0x08
#define CC_NC   0x10
#define CC_C    0x28


/* Initialise the CPU structure */
int cpu_init(cpu_t *cpu, mem_t *mem, int *run);
void cpu_tick(cpu_t *cpu);

#endif

