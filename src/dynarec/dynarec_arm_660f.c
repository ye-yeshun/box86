#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <pthread.h>
#include <errno.h>

#include "debug.h"
#include "box86context.h"
#include "dynarec.h"
#include "emu/x86emu_private.h"
#include "emu/x86run_private.h"
#include "x86run.h"
#include "x86emu.h"
#include "box86stack.h"
#include "callback.h"
#include "emu/x86run_private.h"
#include "x86trace.h"
#include "dynablock.h"
#include "dynablock_private.h"
#include "dynarec_arm.h"
#include "dynarec_arm_private.h"
#include "arm_printer.h"

#include "dynarec_arm_helper.h"

uintptr_t dynarec660f(dynarec_arm_t* dyn, uintptr_t addr, int ninst, int* ok, int* need_epilog)
{
    uintptr_t ip = addr-2;
    uint8_t opcode = F8;
    uint8_t nextop, u8;
    int32_t i32, i32_;
    int16_t i16;
    uint16_t u16;
    uint8_t gd, ed, wback;
    uint8_t eb1, eb2;
    switch(opcode) {

        case 0x1F:
            INST_NAME("NOP (multibyte)");
            nextop = F8;
            FAKEED;
            break;
        
        case 0xA3:
            INST_NAME("BT Ew, Gw");
            nextop = F8;
            USEFLAG;
            GETGD;  // there is an AND below, to 32bits is the same
            GETEW(x1);
            AND_IMM8(x2, gd, 15);
            MOV_REG_LSR_REG(x1, ed, x2);
            AND_IMM8(x1, x1, 1);
            STR_IMM9(x1, xEmu, offsetof(x86emu_t, flags[F_CF]));
            break;
        case 0xA4:
        case 0xA5:
            nextop = F8;
            if(opcode==0xA4) {
                INST_NAME("SHLD Ew, Gw, Ib");
            } else {
                INST_NAME("SHLD Ew, Gw, CL");
                UXTB(x3, xECX, 0);
            }
            GETEWW(x12, x1);
            GETGW(x2);
            if(opcode==0xA4) {
                u8 = F8;
                MOVW(x3, u8);
            }
            CALL(shld16, x1, (wback?(1<<wback):0));
            EWBACKW(x1);
            UFLAGS(1);
            break;

        case 0xAC:
        case 0xAD:
            nextop = F8;
            if(opcode==0xAC) {
                INST_NAME("SHRD Ew, Gw, Ib");
            } else {
                INST_NAME("SHRD Ew, Gw, CL");
                UXTB(x3, xECX, 0);
            }
            GETEWW(x12, x1);
            GETGW(x2);
            if(opcode==0xAC) {
                u8 = F8;
                MOVW(x3, u8);
            }
            CALL(shrd16, x1, (wback?(1<<wback):0));
            EWBACKW(x1);
            UFLAGS(1);
            break;

        default:
            *ok = 0;
            DEFAULT;
    }
    return addr;
}

