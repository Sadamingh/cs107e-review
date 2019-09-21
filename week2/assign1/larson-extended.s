DELAY: .word 0x7fff
.equ BRIGHT, 0b1
.equ BRIGHT2, 0b111
.equ BRIGHT3, 0b11111

// this controls how many GPIO we output, value should be 2 ~ 8
// corresponding GPIO 20 ~ 27
.equ OUTPUT_NUM, 8

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

mov r4, #1 // loop counter
mov r5, #20  // current GPIO
mov r6, #1 // current GPIO increment delta

loop:

// update current
ldr r8, DELAY
and r3, r4, r8
teq r3, r8
bne loop_main

add r5, r6

// > max
teq r5, #(20 + OUTPUT_NUM)
moveq r5, #(20 + OUTPUT_NUM - 2)
moveq r6, #-1

// < min
teq r5, #19
moveq r5, #21
moveq r6, #1

loop_main:

ldr r0, SET0

// set current GPIO bright
mov r1, #1
mov r1, r1, lsl r5
and r3, r4, #BRIGHT
teq r3, #BRIGHT
streq r1, [r0]

// set current+1 bright2
loop_plus_1:
add r1, r5, #1
teq r1, #(20 + OUTPUT_NUM)
beq loop_minus_1
mov r2, #1
mov r2, r2, lsl r1
and r3, r4, #BRIGHT2
teq r3, #BRIGHT2
streq r2, [r0]

// set current+2 bright3
loop_plus_2:
add r1, r5, #2
teq r1, #(20 + OUTPUT_NUM)
beq loop_minus_1
mov r2, #1
mov r2, r2, lsl r1
and r3, r4, #BRIGHT3
teq r3, #BRIGHT3
streq r2, [r0]

// set current-1 bright2
loop_minus_1:
sub r1, r5, #1
teq r1, #19
beq loop_clear
mov r2, #1
mov r2, r2, lsl r1
and r3, r4, #BRIGHT2
teq r3, #BRIGHT2
streq r2, [r0]

// set current-2 bright3
loop_minus_2:
sub r1, r5, #2
teq r1, #19
beq loop_clear
mov r2, #1
mov r2, r2, lsl r1
and r3, r4, #BRIGHT3
teq r3, #BRIGHT3
streq r2, [r0]

loop_clear:

ldr r0, CLR0
mov r2, #20

loop_clear_do:

mov r1, #1
mov r1, r1, lsl r2
str r1, [r0]
add r2, #1
teq r2, #(20 + OUTPUT_NUM)
bne loop_clear_do

add r4, #1 // inc loop counter

b loop

FSEL2: .word 0x20200008
CLR0:  .word 0x20200028
SET0:  .word 0x2020001C
