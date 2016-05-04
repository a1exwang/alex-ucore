asm(".global _start");
asm("_start:");

// clear all registers
asm("li $t0, 0");
asm("li $t1, 0");
asm("li $t2, 0");
asm("li $t3, 0");
asm("li $t4, 0");
asm("li $s0, 0");
asm("li $s1, 0");
asm("li $s2, 0");
asm("li $s3, 0");
asm("li $s4, 0");
asm("li $fp, 0");

// load sp = 128M - 4M
asm("li $t1, 0x0 ");
asm("lih $t1, 0x07c0");
asm("addi $sp, $t1, 0");

// call main
asm("lih $t1, %hi(cmain)");
asm("addiu  $t1, $t1, %lo(cmain)");
asm("call  $t1");

// print 'bye'
asm(".global halt");
asm("halt:");
asm("li $t0, 0");
asm("li $t1, 0x42");
asm("bout $t0, $t1");
asm("li $t1, 0x79");
asm("bout $t0, $t1");
asm("li $t1, 0x65");
asm("bout $t0, $t1");
asm("li $t1, 0x0a");
asm("bout $t0, $t1");
asm("halt");
