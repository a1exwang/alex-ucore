#include "cprintf.h"

void cmain(void) {

  const char *message = "Alex-Machine: (THU.CST) os is loading ...";
  cprintf("%s\n\n", message);

  //print_kerninfo();

  memory_init();              // init physical memory management
  interrupt_init();           // init interrupt descriptor table
  clock_init();               // init clock interrupt

  // enable interrupt
  int iflag;
  asm volatile (
    "sti    %0"
    : "=r" (iflag)
  )

  while (1);
bad:
    cprintf("PANIC!!");

    /* do nothing */
    while (1);
}
