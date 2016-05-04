#include <memlayout.h>

# vectors.S sends all traps here.
.text
.globl __alltraps
__alltraps:
    # push registers to build a trap frame
    # therefore make the stack look like a struct trapframe
    addi  $sp, -60
    sw    0($sp),  $sp
    sw    4($sp),  $t0
    sw    8($sp),  $t1
    sw    12($sp), $t2
    sw    16($sp), $t3
    sw    20($sp), $t4
    sw    24($sp), $s0
    sw    28($sp), $s1
    sw    32($sp), $s2
    sw    36($sp), $s3
    sw    40($sp), $s4
    sw    44($sp), $fp
    sw    48($sp), $gp
    sw    52($sp), $at
    sw    56($sp), $lr

    lih   $t1, %hi(trap)
    addiu $t1, $t1, %lo(trap)
    call  $t1

.globl __trapret
__trapret:

    # restore registers from stack
    addi  $sp, -60
    sw    $t0,  4($sp)
    sw    $t1,  8($sp)
    sw    $t2,  12($sp)
    sw    $t3,  16($sp)
    sw    $t4,  20($sp)
    sw    $s0,  24($sp)
    sw    $s1,  28($sp)
    sw    $s2,  32($sp)
    sw    $s3,  36($sp)
    sw    $s4,  40($sp)
    sw    $fp,  44($sp)
    sw    $gp,  48($sp)
    sw    $at,  52($sp)
    sw    $lr,  56($sp)
    sw    $sp,  0($sp)
    addi  $sp, 60+4 # including error code

    iret
