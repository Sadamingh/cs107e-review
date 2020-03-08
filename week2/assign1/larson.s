.equ DELAY, 0x100000

// this controls how many GPIO we output
// 2 ~ 8
.equ OUTPUT_NUM, 8

// support GPIO 20 ~ 27
ldr r0, FSEL2

mov r1, #0
mov r2, #1
mov r3, #OUTPUT_NUM

config_output:

orr r1, r1, r2
str r1, [r0]
mov r2, r2, lsl #3
subs r3, #1
bne config_output

mov r3, #20 // current GPIO
mov r4, #1 // delta

loop:

// set current GPIO
ldr r0, SET0
mov r1, #1
mov r1, r1, lsl r3
str r1, [r0]

// delay
mov r2, #DELAY
wait1:
    subs r2, #1
    bne wait1

// clear current GPIO
ldr r0, CLR0
str r1, [r0]

add r3, r4

// > max
teq r3, #(20 + OUTPUT_NUM)
moveq r3, #(20 + OUTPUT_NUM - 2)
moveq r4, #-1

// < min
teq r3, #19
moveq r3, #21
moveq r4, #1

b loop

FSEL2: .word 0x20200008
SET0:  .word 0x2020001C
CLR0:  .word 0x20200028
