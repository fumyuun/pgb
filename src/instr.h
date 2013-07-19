#ifndef INSTR_H
#define INSTR_H

#include "cpu.h"

/* Set flag y in CPU x */
#define set_flag(x,y) {\
    (x)->reg_f |= (y);\
}
/* Unset flag y in CPU x */
#define unset_flag(x,y) {\
    (x)->reg_f &= ~(y);\
}

/* Set or unset flag y conditionally in CPU x if condition z holds */
#define cset_flag(x,y,z) {\
    if((z)) {set_flag(x,y);}\
    else    {unset_flag(x,y);}\
}

/* ld reg8, n */
/* args: LDRN(cpu, reg_x) */
#define LDRN(x,y) {\
    ds = mem_read((x)->mem, (x)->reg_pc++);\
    (x)->y = ds;\
    (x)->cycles = 8;\
}

/* ld reg8, reg8 */
/* args: LDRN(cpu, reg_x, reg_y) */
#define LDRR(x,y,z) {\
    (x)->y = (x)->z;\
    (x)->cycles = 4;\
}

/* ld reg8, (reg16) */
/* args: LDR_RR_(cpu, reg_x, reg_y) */
#define LDR_RR_(x,y,z) {\
    ds = mem_read((x)->mem, (x)->z);\
    (x)->y = ds;\
    (x)->cycles = 8;\
}

/* ld (reg16), reg8 */
/* args: LD_RR_R(cpu, reg_x, reg_y) */
#define LD_RR_R(x,y,z) {\
    mem_write((x)->mem, (x)->y, (x)->z);\
    (x)->cycles = 8;\
}

/* ld reg8, (nn) */
/* args: LDR_NN_(cpu, reg_x) */
#define LDR_NN_(x,y) {\
    dl.r8.l = mem_read((x)->mem, (x)->reg_pc++);\
    dl.r8.h = mem_read((x)->mem, (x)->reg_pc++);\
    ds      = mem_read((x)->mem, dl.r16);\
    (x)->y = ds;\
    (x)->cycles = 16;\
}

/* ld (nn), reg8 */
/* args: LD_NN_R(cpu, reg_x) */
#define LD_NN_R(x,y) {\
    dl.r8.l = mem_read((x)->mem, (x)->reg_pc++);\
    dl.r8.h = mem_read((x)->mem, (x)->reg_pc++);\
    mem_write((x)->mem, dl.r16, (x)->y);\
    (x)->cycles = 16;\
}

/* ld (HL), n */
/* args: LD_HL_N(cpu) */
#define LD_HL_N(x) {\
    ds = mem_read((x)->mem, (x)->reg_pc++);\
    mem_write((x)->mem, (x)->reg_hl, ds);\
    (x)->cycles = 12;\
}

/* ld A, (C) */
/* args: LDA_C_(cpu) */
#define LDA_C_(x) {\
    ds = mem_read((x)->mem, 0xFF00 + (x)->reg_c);\
    (x)->reg_a = ds;\
    (x)->cycles = 8;\
}

/* ld (C), A */
/* args: LD_C_A(cpu) */
#define LD_C_A(x) {\
    mem_write((x)->mem, 0xFF00 + (x)->reg_c, (x)->reg_a);\
    (x)->cycles = 8;\
}

/* ldd A, (HL) */
/* args: LDDA_HL_(cpu) */
#define LDDA_HL_(x) {\
    LDR_RR_((x), reg_a, reg_hl--);\
    (x)->cycles = 8;\
}

/* ldd (HL), A */
/* args: LDD_HL_A(cpu) */
#define LDD_HL_A(x) {\
    LD_RR_R((x), reg_hl--, reg_a);\
    (x)->cycles = 8;\
}

/* ldi A, (HL) */
/* args: LDIA_HL_(cpu) */
#define LDIA_HL_(x) {\
    LDR_RR_(x, reg_a, reg_hl++);\
    (x)->cycles = 8;\
}

/* ldi (HL), A */
/* args: LDI_HL_A(cpu) */
#define LDI_HL_A(x) {\
    LD_RR_R(x, reg_hl++, reg_a);\
    (x)->cycles = 8;\
}

/* ldh A, (n) */
/* args: LDHA_N_(cpu) */
#define LDHA_N_(x) {\
    ds = mem_read((x)->mem, (x)->reg_pc++);\
    ds = mem_read((x)->mem, 0xFF00 + ds);\
    (x)->reg_a = ds;\
    (x)->cycles = 12;\
}

/* ldh (n), A */
/* args: LDH_N_A(cpu) */
#define LDH_N_A(x) {\
    ds = mem_read((x)->mem, (x)->reg_pc++);\
    mem_write((x)->mem, 0xFF00 + ds, (x)->reg_a);\
    (x)->cycles = 12;\
}

/* ld reg16, nn */
/* args: LDRRNN(cpu, reg_x) */
#define LDRRNN(x, y) {\
    dl.r8.l = mem_read((x)->mem, (x)->reg_pc++);\
    dl.r8.h = mem_read((x)->mem, (x)->reg_pc++);\
    (x)->y = dl.r16;\
    (x)->cycles = 12;\
}

/* ld SP, HL */
/* args: LDSPHL(cpu) */
#define LDSPHL(x) {\
    (x)->reg_sp = (x)->reg_hl;\
    (x)->cycles = 8;\
}

/* ld HL, SP+n */
/* args: LDHLSPN(cpu) */
#define LDHLSPN(x) {\
    ds = mem_read((x)->mem, (x)->reg_pc++);\
    (x)->reg_hl = ds + (x)->reg_sp;\
    unset_flag(x, FLAG_Z);\
    unset_flag(x, FLAG_N);\
    cset_flag(x, FLAG_C, ((x)->reg_hl & 0x100));\
    cset_flag(x, FLAG_H, ((x)->reg_hl & 0x10));\
    (x)->cycles = 12;\
}

/* ld (nn), SP */
/* args: LD_NN_SP(cpu) */
#define LD_NN_SP(x) {\
    dl.r8.l = mem_read((x)->mem, (x)->reg_pc++);\
    dl.r8.h = mem_read((x)->mem, (x)->reg_pc++);\
    mem_write((x)->mem, dl.r16, (x)->reg_sp);\
    (x)->cycles = 20;\
}

/* push reg16 */
/* args: PUSH(cpu, reg_x) */
#define PUSH(x,y) {\
    dl.r16 = (x)->y;\
    mem_write((x)->mem, --((x)->reg_sp), dl.r8.h);\
    mem_write((x)->mem, --((x)->reg_sp), dl.r8.l);\
    (x)->cycles = 16;\
}

/* pop reg16 */
/* args: POP(cpu, reg_x) */
#define POP(x,y) {\
    dl.r8.l = mem_read((x)->mem, ((x)->reg_sp)++);\
    dl.r8.h = mem_read((x)->mem, ((x)->reg_sp)++);\
    (x)->y = dl.r16;\
    (x)->cycles = 16;\
}

/* add x, y */
/* args: ADD(cpu, x, y) */
#define ADD(x,y,z) {\
    temp16 = y + z;\
    cset_flag(x, FLAG_Z, ((temp16 & 0xFF) == 0));\
    unset_flag(x, FLAG_N);\
    cset_flag(x, FLAG_H, ((temp16 & 0x10) == 0x10));\
    cset_flag(x, FLAG_C, ((temp16 & 0x100) == 0x100));\
    y = (temp16 & 0xFF);\
}

/* add reg8, reg8 */
/* args: ADDRR(cpu, reg_x, reg_y) */
#define ADDRR(x,y,z) {\
    ADD((x), (x)->y, (x)->z);\
    (x)->cycles = 4;\
}

/* add reg8, (RR) */
/* args: ADDR_RR_(cpu, reg_x, reg_y) */
#define ADDR_RR_(x,y,z) {\
    ds = mem_read((x)->mem, (x)->z);\
    ADD((x), (x)->y, ds);\
    (x)->cycles = 8;\
}

/* add reg8, n */
/* args: ADDRN(cpu, reg_x) */
#define ADDRN(x,y) {\
    dl.r8.h = mem_read((x)->mem, (x)->reg_pc++);\
    dl.r8.l = mem_read((x)->mem, (x)->reg_pc++);\
    ds = mem_read((x)->mem, dl.r16);\
    ADD((x), (x)->y, ds);\
    (x)->cycles = 8;\
}

/* adc x, y */
/* args: ADC(cpu, x, y) */
#define ADC(x,y,z) {\
    temp16 = y + z + (((x)->reg_f & FLAG_C) == FLAG_C ? 1 : 0);\
    cset_flag(x, FLAG_Z, ((temp16 & 0xFF) == 0));\
    unset_flag(x, FLAG_N);\
    cset_flag(x, FLAG_H, ((temp16 & 0x10) == 0x10));\
    cset_flag(x, FLAG_C, ((temp16 & 0x100) == 0x100));\
    y = (temp16 & 0xFF);\
}

/* adc reg8, reg8 */
/* args: ADCRR(cpu, reg_x, reg_y) */
#define ADCRR(x,y,z) {\
    ADC((x), (x)->y, (x)->z);\
    (x)->cycles = 4;\
}

/* adc reg8, (RR) */
/* args: ADCR_RR_(cpu, reg_x, reg_y) */
#define ADCR_RR_(x,y,z) {\
    ds = mem_read((x)->mem, (x)->z);\
    ADC((x), (x)->y, ds);\
    (x)->cycles = 8;\
}

/* adc reg8, n */
/* args: ADCRN(cpu, reg_x) */
#define ADCRN(x,y) {\
    dl.r8.h = mem_read((x)->mem, (x)->reg_pc++);\
    dl.r8.l = mem_read((x)->mem, (x)->reg_pc++);\
    ds = mem_read((x)->mem, dl.r16);\
    ADC((x), (x)->y, ds);\
    (x)->cycles = 8;\
}

/* sub x, y */
/* args: SUB(cpu, x, y) */
/* TODO: confirm carry calculations */
#define SUB(x,y,z) {\
    temp16 = y - z;\
    cset_flag(x, FLAG_Z, ((temp16 & 0xFF) == 0));\
    set_flag(x, FLAG_N);\
    cset_flag(x, FLAG_H, ((temp16 & 0x10) == 0x10));\
    cset_flag(x, FLAG_C, ((temp16 & 0x100) == 0x100));\
    y = (temp16 & 0xFF);\
}

/* sub reg8, reg8 */
/* args: SUBRR(cpu, reg_x, reg_y) */
#define SUBRR(x,y,z) {\
    SUB((x), (x)->y, (x)->z);\
    (x)->cycles = 4;\
}

/* sub reg8, (RR) */
/* args: SUBR_RR_(cpu, reg_x, reg_y) */
#define SUBR_RR_(x,y,z) {\
    ds = mem_read((x)->mem, (x)->z);\
    SUB((x), (x)->y, ds);\
    (x)->cycles = 8;\
}

/* sub reg8, n */
/* args: SUBRN(cpu, reg_x) */
#define SUBRN(x,y) {\
    dl.r8.h = mem_read((x)->mem, (x)->reg_pc++);\
    dl.r8.l = mem_read((x)->mem, (x)->reg_pc++);\
    ds = mem_read((x)->mem, dl.r16);\
    SUB((x), (x)->y, ds);\
    (x)->cycles = 8;\
}

/* sbc x, y */
/* args: SBC(cpu, x, y) */
/* TODO: Confirm carry calculation */
#define SBC(x,y,z) {\
    temp16 = y - z - (((x)->reg_f & FLAG_C) == FLAG_C ? 1 : 0);\
    cset_flag(x, FLAG_Z, ((temp16 & 0xFF) == 0));\
    set_flag(x, FLAG_N);\
    cset_flag(x, FLAG_H, ((temp16 & 0x10) == 0x10));\
    cset_flag(x, FLAG_C, ((temp16 & 0x100) == 0x100));\
    y = (temp16 & 0xFF);\
}

/* sbc reg8, reg8 */
/* args: SBCRR(cpu, reg_x, reg_y) */
#define SBCRR(x,y,z) {\
    SBC((x), (x)->y, (x)->z);\
    (x)->cycles = 4;\
}

/* sbc reg8, (RR) */
/* args: SBCR_RR_(cpu, reg_x, reg_y) */
#define SBCR_RR_(x,y,z) {\
    ds = mem_read((x)->mem, (x)->z);\
    SBC((x), (x)->y, ds);\
    (x)->cycles = 8;\
}

/* sbc reg8, n */
/* args: SBCRN(cpu, reg_x) */
#define SBCRN(x,y) {\
    dl.r8.h = mem_read((x)->mem, (x)->reg_pc++);\
    dl.r8.l = mem_read((x)->mem, (x)->reg_pc++);\
    ds = mem_read((x)->mem, dl.r16);\
    SBC((x), (x)->y, ds);\
    (x)->cycles = 8;\
}

/* xor x, y */
/* args: XOR(cpu, x, y) */
#define XOR(x,y,z) {\
    y = y ^ z;\
    cset_flag(x, FLAG_Z, ((y & 0xFF) == 0x00));\
    unset_flag(x, FLAG_N);\
    unset_flag(x, FLAG_H);\
    unset_flag(x, FLAG_C);\
}

/* xor reg8, reg8 */
/* args: XORRR(cpu, reg_x, reg_y) */
#define XORRR(x,y,z) {\
    XOR((x), (x)->y, (x)->z);\
    (x)->cycles = 4;\
}

/* cp a, reg8 */
/* args: CPR(cpu, reg_x) */
#define CPR(x,y) {\
    temp8 = (x)->reg_a;\
    SUB((x), (x)->reg_a, (x)->y);\
    (x)->reg_a = temp8;\
    (x)->cycles = 4;\
}

/* cp a, (HL) */
/* args: CPR_HL_(cpu) */
#define CPR_HL_(x) {\
    ds = mem_read((x)->mem, (x)->reg_hl);\
    temp8 = (x)->reg_a;\
    SUB((x), (x)->reg_a, ds);\
    (x)->reg_a = temp8;\
    (x)->cycles = 8;\
}

/* cp a, n */
/* args: CPN(cpu) */
#define CPN(x) {\
    ds = mem_read((x)->mem, (x)->reg_pc++);\
    temp8 = (x)->reg_a;\
    SUB((x), (x)->reg_a, ds);\
    (x)->reg_a = temp8;\
    (x)->cycles = 8;\
}

/* inc reg8 */
/* args: INC(cpu, reg_x) */
/* NOTE: unsure if FLAG_H is correct */
#define INC(x,y) {\
    (x)->y++;\
    cset_flag((x), FLAG_Z, ((x)->y == 0x00));\
    unset_flag((x), FLAG_N);\
    unset_flag((x), FLAG_H);\
    (x)->cycles = 4;\
}

/* dec reg8 */
/* args: DEC(cpu, reg_x) */
/* NOTE: unsure if FLAG_H is correct */
#define DEC(x,y) {\
    (x)->y--;\
    cset_flag((x), FLAG_Z, ((x)->y == 0x00));\
    set_flag((x), FLAG_N);\
    unset_flag((x), FLAG_H);\
    (x)->cycles = 4;\
}

/* inc reg16 */
/* args: INC16(cpu, reg_xx) */
#define INC16(x,y) {\
    (x)->y++;\
    (x)->cycles = 4;\
}

/* rla */
/* args: RLA(cpu) */
#define RLA(x) {\
    temp8 = ((x)->reg_f & FLAG_C) == FLAG_C ? 0x01 : 0x00;\
    cset_flag((x), FLAG_C, (((x)->reg_a & 0x80) == 0x80));\
    (x)->reg_a = ((x)->reg_a << 1) | temp8;\
    cset_flag((x), FLAG_Z, ((x)->reg_a == 0x00));\
    unset_flag((x), FLAG_N);\
    unset_flag((x), FLAG_H);\
    (x)->cycles = 4;\
}

/* rl reg8 */
/* args: RL(cpu, reg_x) */
#define RL(x,y) {\
    temp8 = ((x)->reg_f & FLAG_C) == FLAG_C ? 0x01 : 0x00;\
    cset_flag((x), FLAG_C, (((x)->y & 0x80) == 0x80));\
    (x)->y = ((x)->y << 1) | temp8;\
    cset_flag((x), FLAG_Z, ((x)->y == 0x00));\
	unset_flag((x), FLAG_N);\
	unset_flag((x), FLAG_H);\
    (x)->cycles = 8;\
}

/* bit n, n */
/* args: BIT(cpu, n, x) */
#define BIT(x,y,z) {\
    cset_flag((x), FLAG_Z, ((0x01 << y) & z) == 0x00);\
    unset_flag((x), FLAG_N);\
    set_flag((x), FLAG_H);\
}

/* bit n, reg8 */
/* args: BITR(cpu, n, reg_x) */
#define BITR(x,y,z) {\
    BIT((x), y, (x)->z);\
    (x)->cycles = 8;\
}

/* bit n, (RR) */
/* args: BITR_RR_(cpu, n, reg_x) */
#define BITR_RR_(x,y,z) {\
    ds = mem_read((x)->mem, (x)->z);\
    BIT((x), y, ds);\
    (x)->cycles = 16;\
}

/* jr n */
/* args: JR(cpu) */
#define JR(x) {\
    ds = mem_read((x)->mem, (x)->reg_pc++);\
    (x)->reg_pc += (int8_t)ds;\
    (x)->cycles = 8;\
}

/* jr cc, n */
/* args: JR(cpu, cc) */
#define JRCC(x,y) {\
    ds = mem_read((x)->mem, (x)->reg_pc++);\
    if(((y) == CC_NZ && ((x)->reg_f & FLAG_Z) == 0x00)\
    || ((y) == CC_Z && ((x)->reg_f & FLAG_Z) == FLAG_Z)\
    || ((y) == CC_NC && ((x)->reg_f & FLAG_C) == 0x00)\
    || ((y) == CC_C && ((x)->reg_f & FLAG_C) == FLAG_C)) {\
        (x)->reg_pc += (int8_t)ds;\
        (x)->cycles = 8;\
    }\
}

/* call n */
/* args: CALL(cpu) */
#define CALL(x) {\
    dl.r8.l = mem_read((x)->mem, (x)->reg_pc++);\
    dl.r8.h = mem_read((x)->mem, (x)->reg_pc++);\
    mem_write((x)->mem, --((x)->reg_sp), (x)->regs[PC].r8.h);\
    mem_write((x)->mem, --((x)->reg_sp), (x)->regs[PC].r8.l);\
    (x)->reg_pc = dl.r16;\
    (x)->cycles = 12;\
}

/* ret */
/* args: RET(cpu) */
#define RET(x) {\
    dl.r8.l = mem_read((x)->mem, ((x)->reg_sp)++);\
    dl.r8.h = mem_read((x)->mem, ((x)->reg_sp)++);\
    (x)->reg_pc = dl.r16;\
    (x)->cycles = 8;\
}

#endif

