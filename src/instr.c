#include "main.h"
#include "instr.h"

void print_dbg(cpu_t *cpu, uint16_t pc, uint8_t instr);

void cpu_tick(cpu_t *cpu)
{
    uint8_t instr, einstr, ds, temp8;
    uint16_t instr_pc, einstr_pc, temp16;
    r16_t dl;
    instr_pc = cpu->reg_pc++;
    instr = mem_read(cpu->mem, instr_pc);
#ifdef DEBUG
    #ifdef NCURSES
        mvprintw(0, 160, "[0x%X] Execute 0x%X     ", instr_pc, instr);
    #else
        printf("\n[0x%X] Execute 0x%X ", instr_pc, instr);
    #endif
#endif
    switch(instr)
    {
        case 0x06:  LDRN(cpu, reg_b);   break;
        case 0x0E:  LDRN(cpu, reg_c);   break;
        case 0x16:  LDRN(cpu, reg_d);   break;
        case 0x1E:  LDRN(cpu, reg_e);   break;
        case 0x26:  LDRN(cpu, reg_h);   break;
        case 0x2E:  LDRN(cpu, reg_l);   break;
        
        case 0x40:  LDRR(cpu, reg_b, reg_b);    break;
        case 0x41:  LDRR(cpu, reg_b, reg_c);    break;
        case 0x42:  LDRR(cpu, reg_b, reg_d);    break;
        case 0x43:  LDRR(cpu, reg_b, reg_e);    break;
        case 0x44:  LDRR(cpu, reg_b, reg_h);    break;
        case 0x45:  LDRR(cpu, reg_b, reg_l);    break;
        case 0x46:  LDR_RR_(cpu, reg_b, reg_hl);break;
        
        case 0x48:  LDRR(cpu, reg_c, reg_b);    break;
        case 0x49:  LDRR(cpu, reg_c, reg_c);    break;
        case 0x4A:  LDRR(cpu, reg_c, reg_d);    break;
        case 0x4B:  LDRR(cpu, reg_c, reg_e);    break;
        case 0x4C:  LDRR(cpu, reg_c, reg_h);    break;
        case 0x4D:  LDRR(cpu, reg_c, reg_l);    break;
        case 0x4E:  LDR_RR_(cpu, reg_c, reg_hl);break;
        
        case 0x50:  LDRR(cpu, reg_d, reg_b);    break;
        case 0x51:  LDRR(cpu, reg_d, reg_c);    break;
        case 0x52:  LDRR(cpu, reg_d, reg_d);    break;
        case 0x53:  LDRR(cpu, reg_d, reg_e);    break;
        case 0x54:  LDRR(cpu, reg_d, reg_h);    break;
        case 0x55:  LDRR(cpu, reg_d, reg_l);    break;
        case 0x56:  LDR_RR_(cpu, reg_d, reg_hl);break;
            
        case 0x58:  LDRR(cpu, reg_e, reg_b);    break;
        case 0x59:  LDRR(cpu, reg_e, reg_c);    break;
        case 0x5A:  LDRR(cpu, reg_e, reg_d);    break;
        case 0x5B:  LDRR(cpu, reg_e, reg_e);    break;
        case 0x5C:  LDRR(cpu, reg_e, reg_h);    break;
        case 0x5D:  LDRR(cpu, reg_e, reg_l);    break;
        case 0x5E:  LDR_RR_(cpu, reg_e, reg_hl);break;
            
        case 0x60:  LDRR(cpu, reg_h, reg_b);    break;
        case 0x61:  LDRR(cpu, reg_h, reg_c);    break;
        case 0x62:  LDRR(cpu, reg_h, reg_d);    break;
        case 0x63:  LDRR(cpu, reg_h, reg_e);    break;
        case 0x64:  LDRR(cpu, reg_h, reg_h);    break;
        case 0x65:  LDRR(cpu, reg_h, reg_l);    break;
        case 0x66:  LDR_RR_(cpu, reg_h, reg_hl);break;
            
        case 0x68:  LDRR(cpu, reg_l, reg_b);    break;
        case 0x69:  LDRR(cpu, reg_l, reg_c);    break;
        case 0x6A:  LDRR(cpu, reg_l, reg_d);    break;
        case 0x6B:  LDRR(cpu, reg_l, reg_e);    break;
        case 0x6C:  LDRR(cpu, reg_l, reg_h);    break;
        case 0x6D:  LDRR(cpu, reg_l, reg_l);    break;
        case 0x6E:  LDR_RR_(cpu, reg_l, reg_hl);break;
                 
        case 0x70:  LD_RR_R(cpu, reg_hl, reg_b);break;
        case 0x71:  LD_RR_R(cpu, reg_hl, reg_c);break;
        case 0x72:  LD_RR_R(cpu, reg_hl, reg_d);break;
        case 0x73:  LD_RR_R(cpu, reg_hl, reg_e);break;
        case 0x74:  LD_RR_R(cpu, reg_hl, reg_h);break;
        case 0x75:  LD_RR_R(cpu, reg_hl, reg_l);break;
        case 0x36:  LD_HL_N(cpu);               break;
            
        case 0x7F:  LDRR(cpu, reg_a, reg_a);    break;
        case 0x78:  LDRR(cpu, reg_a, reg_b);    break;
        case 0x79:  LDRR(cpu, reg_a, reg_c);    break;
        case 0x7A:  LDRR(cpu, reg_a, reg_d);    break;
        case 0x7B:  LDRR(cpu, reg_a, reg_e);    break;
        case 0x7C:  LDRR(cpu, reg_a, reg_h);    break;
        case 0x7D:  LDRR(cpu, reg_a, reg_l);    break;
        case 0x0A:  LDR_RR_(cpu, reg_a, reg_bc);break;
        case 0x1A:  LDR_RR_(cpu, reg_a, reg_de);break;
        case 0x7E:  LDR_RR_(cpu, reg_a, reg_hl);break;
        case 0xFA:  LDR_NN_(cpu, reg_a);        break;
        case 0x3E:  LDRN(cpu, reg_a);           break;
        
        case 0x47:  LDRR(cpu, reg_b, reg_a);    break;
        case 0x4F:  LDRR(cpu, reg_c, reg_a);    break;
        case 0x57:  LDRR(cpu, reg_d, reg_a);    break;
        case 0x5F:  LDRR(cpu, reg_e, reg_a);    break;
        case 0x67:  LDRR(cpu, reg_h, reg_a);    break;
        case 0x6F:  LDRR(cpu, reg_l, reg_a);    break;
        case 0x02:  LD_RR_R(cpu, reg_bc, reg_a);break;
        case 0x12:  LD_RR_R(cpu, reg_de, reg_a);break;
        case 0x77:  LD_RR_R(cpu, reg_hl, reg_a);break;
        case 0xEA:  LD_NN_R(cpu, reg_a);        break;
        
        case 0xF2:  LDA_C_(cpu);                break;
        case 0xE2:  LD_C_A(cpu);                break;
        
        case 0x3A:  LDDA_HL_(cpu);              break;
        case 0x32:  LDD_HL_A(cpu);              break;
        case 0x2A:  LDIA_HL_(cpu);              break;
        case 0x22:  LDI_HL_A(cpu);              break;
        
        case 0xE0:  LDH_N_A(cpu);               break;
        case 0xF0:  LDHA_N_(cpu);               break;
        
        case 0x01:  LDRRNN(cpu, reg_bc);        break;
        case 0x11:  LDRRNN(cpu, reg_de);        break;
        case 0x21:  LDRRNN(cpu, reg_hl);        break;
        case 0x31:  LDRRNN(cpu, reg_sp);        break;
        case 0xF9:  LDSPHL(cpu);                break;
        case 0xF8:  LDHLSPN(cpu);               break;
        case 0x08:  LD_NN_SP(cpu);              break;
        
        case 0xF5:  PUSH(cpu, reg_af);          break;
        case 0xC5:  PUSH(cpu, reg_bc);          break;
        case 0xD5:  PUSH(cpu, reg_de);          break;
        case 0xE5:  PUSH(cpu, reg_hl);          break;

        case 0xF1:  POP(cpu, reg_af);           break;
        case 0xC1:  POP(cpu, reg_bc);           break;
        case 0xD1:  POP(cpu, reg_de);           break;
        case 0xE1:  POP(cpu, reg_hl);           break;
        
        case 0x87:  ADDRR(cpu, reg_a, reg_a);   break;
        case 0x80:  ADDRR(cpu, reg_a, reg_b);   break;
        case 0x81:  ADDRR(cpu, reg_a, reg_c);   break;
        case 0x82:  ADDRR(cpu, reg_a, reg_d);   break;
        case 0x83:  ADDRR(cpu, reg_a, reg_e);   break;
        case 0x84:  ADDRR(cpu, reg_a, reg_h);   break;
        case 0x85:  ADDRR(cpu, reg_a, reg_l);   break;
        case 0x86:  ADDR_RR_(cpu, reg_a, reg_hl);break;
        case 0xC6:  ADDRN(cpu, reg_a);          break;
        
        case 0x8F:  ADCRR(cpu, reg_a, reg_a);   break;
        case 0x88:  ADCRR(cpu, reg_a, reg_b);   break;
        case 0x89:  ADCRR(cpu, reg_a, reg_c);   break;
        case 0x8A:  ADCRR(cpu, reg_a, reg_d);   break;
        case 0x8B:  ADCRR(cpu, reg_a, reg_e);   break;
        case 0x8C:  ADCRR(cpu, reg_a, reg_h);   break;
        case 0x8D:  ADCRR(cpu, reg_a, reg_l);   break;
        case 0x8E:  ADCR_RR_(cpu, reg_a, reg_hl);break;
        case 0xCE:  ADCRN(cpu, reg_a);          break;
        
        case 0x97:  SUBRR(cpu, reg_a, reg_a);   break;
        case 0x90:  SUBRR(cpu, reg_a, reg_b);   break;
        case 0x91:  SUBRR(cpu, reg_a, reg_c);   break;
        case 0x92:  SUBRR(cpu, reg_a, reg_d);   break;
        case 0x93:  SUBRR(cpu, reg_a, reg_e);   break;
        case 0x94:  SUBRR(cpu, reg_a, reg_h);   break;
        case 0x95:  SUBRR(cpu, reg_a, reg_l);   break;
        case 0x96:  SUBR_RR_(cpu, reg_a, reg_hl);break;
        case 0xD6:  SUBRN(cpu, reg_a);          break; /* Opcode unconfirmed */
        
        case 0x9F:  SBCRR(cpu, reg_a, reg_a);   break;
        case 0x98:  SBCRR(cpu, reg_a, reg_b);   break;
        case 0x99:  SBCRR(cpu, reg_a, reg_c);   break;
        case 0x9A:  SBCRR(cpu, reg_a, reg_d);   break;
        case 0x9B:  SBCRR(cpu, reg_a, reg_e);   break;
        case 0x9C:  SBCRR(cpu, reg_a, reg_h);   break;
        case 0x9D:  SBCRR(cpu, reg_a, reg_l);   break;
        case 0x9E:  SBCR_RR_(cpu, reg_a, reg_hl);break;
        case 0xDE:  SBCRN(cpu, reg_a);          break; /* Opcode unconfirmed */
        
        
        case 0xAF:  XORRR(cpu, reg_a, reg_a);   break;
        
        case 0xBF:  CPR(cpu, reg_a);            break;
        case 0xB8:  CPR(cpu, reg_b);            break;
        case 0xB9:  CPR(cpu, reg_c);            break;
        case 0xBA:  CPR(cpu, reg_d);            break;
        case 0xBB:  CPR(cpu, reg_e);            break;
        case 0xBC:  CPR(cpu, reg_h);            break;
        case 0xBD:  CPR(cpu, reg_l);            break;
        case 0xBE:  CPR_HL_(cpu);               break;
        case 0xFE:  CPN(cpu);                   break;
        
        case 0x3C:  INC(cpu, reg_a);            break;
        case 0x04:  INC(cpu, reg_b);            break;
        case 0x0C:  INC(cpu, reg_c);            break;
        case 0x14:  INC(cpu, reg_d);            break;
        case 0x1C:  INC(cpu, reg_e);            break;
        case 0x24:  INC(cpu, reg_h);            break;
        case 0x2C:  INC(cpu, reg_l);            break;
        /* TODO INC (HL) */
        
        case 0x3D:  DEC(cpu, reg_a);            break;
        case 0x05:  DEC(cpu, reg_b);            break;
        case 0x0D:  DEC(cpu, reg_c);            break;
        case 0x15:  DEC(cpu, reg_d);            break;
        case 0x1D:  DEC(cpu, reg_e);            break;
        case 0x25:  DEC(cpu, reg_h);            break;
        case 0x2D:  DEC(cpu, reg_l);            break;
        /* TODO DEC (HL) */
        
        case 0x03:  INC16(cpu, reg_bc);         break;
        case 0x13:  INC16(cpu, reg_de);         break;
        case 0x23:  INC16(cpu, reg_hl);         break;
        case 0x33:  INC16(cpu, reg_sp);         break;
		/* TODO DEC 16   */
        
        case 0x17:  RLA(cpu);                   break;
        
        case 0x18:  JR(cpu);                    break;
        
        case 0x20:  JRCC(cpu, CC_NZ);           break;
        case 0x28:  JRCC(cpu, CC_Z);            break;
        case 0x30:  JRCC(cpu, CC_NC);           break;
        case 0x38:  JRCC(cpu, CC_C);            break;
        
        case 0xCD:  CALL(cpu);                  break;
        
        case 0xC9:  RET(cpu);                   break;
        
        case 0xCB: /* Extended ALU Operations */
            einstr_pc = cpu->reg_pc++;
            einstr = mem_read(cpu->mem, einstr_pc);
            #ifdef DEBUG
            #ifdef NCURSES
                mvprintw(0, 165, "-0x%X", einstr);
            #else
                printf("- 0x%X", einstr);
            #endif
            #endif
            switch(einstr)
            {
                case 0x11:  RL(cpu, reg_c);     break;
                default:
                    /* check for BIT op */
                    /* -x-- -xxx are relevant for the operation */
                    switch(einstr & 0x47)
                    {
                        /* --xx x--- are relevant as argument */
                        case 0x47:  BITR(cpu, ((einstr & 0x38) >> 3), reg_a);   break;
                        case 0x40:  BITR(cpu, ((einstr & 0x38) >> 3), reg_b);   break;
                        case 0x41:  BITR(cpu, ((einstr & 0x38) >> 3), reg_c);   break;
                        case 0x42:  BITR(cpu, ((einstr & 0x38) >> 3), reg_d);   break;
                        case 0x43:  BITR(cpu, ((einstr & 0x38) >> 3), reg_e);   break;
                        case 0x44:  BITR(cpu, ((einstr & 0x38) >> 3), reg_h);   break;
                        case 0x45:  BITR(cpu, ((einstr & 0x38) >> 3), reg_l);   break;
                        case 0x46:  BITR_RR_(cpu, ((einstr & 0x38) >> 3), reg_hl);break;
                        
                        default:
                            #ifndef NCURSES
                                print_dbg(cpu, einstr_pc, einstr);
                            #endif
                            *cpu->run = 0;
                            break;
                    }
                    break;
            }
            break;

        default:
            #ifndef NCURSES
                print_dbg(cpu, instr_pc, instr);
            #endif
            *cpu->run = 0;
            break;
    }
}

void print_dbg(cpu_t *cpu, uint16_t pc, uint8_t instr)
{
    uint16_t i;
    fprintf(stderr, "[0x%X] Missing instruction 0x%X!\n", pc, instr);
    for(i = ((pc - 0x10) < 0 ? 0 : (pc - 0x10)); 
        i < (pc + 0x10); ++i) {
        if(i == pc)
            fprintf(stderr, "[0x%X] ", mem_read(cpu->mem, i));
        else
            fprintf(stderr, " 0x%X  ", mem_read(cpu->mem, i));
    }
}
