/*
 * Init_Stack.S
 *
 *  Created on: Oct 10, 2023
 *      Author: PC
 */
     .syntax unified
    .arch armv7-m

/* Init .data and .bss sections */
    .thumb_func
    .align 2
    .globl   Init_Stack
    .weak    Init_Stack
    .type    Init_Stack, %function
Init_Stack:
    ldr     r0,=__StackTop
    mov     r13,r0
    bx      LR


