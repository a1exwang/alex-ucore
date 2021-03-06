#include <defs.h>
#include <mmu.h>
#include <memlayout.h>
#include <clock.h>
#include <trap.h>
#include <x86.h>
#include <stdio.h>
#include <assert.h>
#include <console.h>
#include <kdebug.h>

#define TICK_NUM 100

static void print_ticks() {
    cprintf("%d ticks\n",TICK_NUM);
#ifdef DEBUG_GRADE
    cprintf("End of Test.\n");
    panic("EOT: kernel seems ok.");
#endif
}

extern void *__alltraps;

/* idt_init - initialize IDT to each of the entry points in kern/trap/vectors.S */
void
interrupt_init(void) {
	asm volatile (
    "stiv   %0"
    :
    : "r" (__alltraps)
  )
}
//
// static const char *
// trapname(int trapno) {
//     static const char * const excnames[] = {
//         "Divide error",
//         "Debug",
//         "Non-Maskable Interrupt",
//         "Breakpoint",
//         "Overflow",
//         "BOUND Range Exceeded",
//         "Invalid Opcode",
//         "Device Not Available",
//         "Double Fault",
//         "Coprocessor Segment Overrun",
//         "Invalid TSS",
//         "Segment Not Present",
//         "Stack Fault",
//         "General Protection",
//         "Page Fault",
//         "(unknown trap)",
//         "x87 FPU Floating-Point Error",
//         "Alignment Check",
//         "Machine-Check",
//         "SIMD Floating-Point Exception"
//     };
//
//     if (trapno < sizeof(excnames)/sizeof(const char * const)) {
//         return excnames[trapno];
//     }
//     if (trapno >= IRQ_OFFSET && trapno < IRQ_OFFSET + 16) {
//         return "Hardware Interrupt";
//     }
//     return "(unknown trap)";
// }

//
// static const char *IA32flags[] = {
//     "CF", NULL, "PF", NULL, "AF", NULL, "ZF", "SF",
//     "TF", "IF", "DF", "OF", NULL, NULL, "NT", NULL,
//     "RF", "VM", "AC", "VIF", "VIP", "ID", NULL, NULL,
// };

// void
// print_trapframe(struct trapframe *tf) {
//     cprintf("trapframe at %p\n", tf);
//     print_regs(&tf->tf_regs);
//     cprintf("  ds   0x----%04x\n", tf->tf_ds);
//     cprintf("  es   0x----%04x\n", tf->tf_es);
//     cprintf("  fs   0x----%04x\n", tf->tf_fs);
//     cprintf("  gs   0x----%04x\n", tf->tf_gs);
//     cprintf("  trap 0x%08x %s\n", tf->tf_trapno, trapname(tf->tf_trapno));
//     cprintf("  err  0x%08x\n", tf->tf_err);
//     cprintf("  eip  0x%08x\n", tf->tf_eip);
//     cprintf("  cs   0x----%04x\n", tf->tf_cs);
//     cprintf("  flag 0x%08x ", tf->tf_eflags);
//
//     int i, j;
//     for (i = 0, j = 1; i < sizeof(IA32flags) / sizeof(IA32flags[0]); i ++, j <<= 1) {
//         if ((tf->tf_eflags & j) && IA32flags[i] != NULL) {
//             cprintf("%s,", IA32flags[i]);
//         }
//     }
//     cprintf("IOPL=%d\n", (tf->tf_eflags & FL_IOPL_MASK) >> 12);
//
//     if (!trap_in_kernel(tf)) {
//         cprintf("  esp  0x%08x\n", tf->tf_esp);
//         cprintf("  ss   0x----%04x\n", tf->tf_ss);
//     }
// }

// void
// print_regs(struct pushregs *regs) {
//     cprintf("  edi  0x%08x\n", regs->reg_edi);
//     cprintf("  esi  0x%08x\n", regs->reg_esi);
//     cprintf("  ebp  0x%08x\n", regs->reg_ebp);
//     cprintf("  oesp 0x%08x\n", regs->reg_oesp);
//     cprintf("  ebx  0x%08x\n", regs->reg_ebx);
//     cprintf("  edx  0x%08x\n", regs->reg_edx);
//     cprintf("  ecx  0x%08x\n", regs->reg_ecx);
//     cprintf("  eax  0x%08x\n", regs->reg_eax);
// }

static uint32_t tick_count = 0;
//
// /* trap_dispatch - dispatch based on what type of trap occurred */
// static void
// trap_dispatch(struct trapframe *tf) {
//     char c;
//
//     switch (tf->tf_trapno) {
//     case IRQ_OFFSET + IRQ_TIMER:
//         /* LAB1 YOUR CODE : STEP 3 */
//         /* handle the timer interrupt */
//         /* (1) After a timer interrupt, you should record this event using a global variable (increase it), such as ticks in kern/driver/clock.c
//          * (2) Every TICK_NUM cycle, you can print some info using a funciton, such as print_ticks().
//          * (3) Too Simple? Yes, I think so!
//          */
// 		tick_count++;
// 		if (tick_count % TICK_NUM == 0) {
// 			print_ticks();
// 		}
//         break;
//     case IRQ_OFFSET + IRQ_COM1:
//         c = cons_getc();
//         cprintf("serial [%03d] %c\n", c, c);
//         break;
//     case IRQ_OFFSET + IRQ_KBD:
//         c = cons_getc();
//         cprintf("kbd [%03d] %c\n", c, c);
//         break;
//     //LAB1 CHALLENGE 1 : YOUR CODE you should modify below codes.
//     case T_SWITCH_TOU:
//     case T_SWITCH_TOK:
//         panic("T_SWITCH_** ??\n");
//         break;
//     case IRQ_OFFSET + IRQ_IDE1:
//     case IRQ_OFFSET + IRQ_IDE2:
//         /* do nothing */
//         break;
//     default:
//         // in kernel, it must be a mistake
//         if ((tf->tf_cs & 3) == 0) {
//             print_trapframe(tf);
//             panic("unexpected trap in kernel.\n");
//         }
//     }
// }

/* *
 * trap - handles or dispatches an exception/interrupt. if and when trap() returns,
 * the code in kern/trap/trapentry.S restores the old CPU state saved in the
 * trapframe and then uses the iret instruction to return from the exception.
 * */
void
trap(struct trapframe *tf) {
    // dispatch based on what type of trap occurred
    trap_dispatch(tf);
}
