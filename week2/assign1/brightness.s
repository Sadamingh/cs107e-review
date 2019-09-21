// set GPIO 20, 21, 22 to different brightness

.equ BRIGHT, 0b1
.equ BRIGHT2, 0b111
.equ BRIGHT3, 0b11111

// set GPIO 20, 21, 22 to output
ldr r0, FSEL2
mov r1, #0b001001001
str r1, [r0]

mov r2, #1 // loop counter

loop:

// set GPIO 20 high if r2 & BRIGHT == BRIGHT
ldr r0, SET0
mov r1, #(1 << 20)
and r3, r2, #BRIGHT
cmp r3, #BRIGHT
streq r1, [r0]

// set GPIO 21 high if r2 & BRIGHT2 = BRIGHT2
ldr r0, SET0
mov r1, #(1 << 21)
and r3, r2, #BRIGHT2
cmp r3, #BRIGHT2
streq r1, [r0]

// set GPIO 22 high if r2 & BRIGHT3 = BRIGHT3
ldr r0, SET0
mov r1, #(1 << 22)
and r3, r2, #BRIGHT3
cmp r3, #BRIGHT3
streq r1, [r0]

// set GPIO 20 low
ldr r0, CLR0
mov r1, #(1 << 20)
str r1, [r0]

// set GPIO 21 low
ldr r0, CLR0
mov r1, #(1 << 21)
str r1, [r0]

// set GPIO 22 low
ldr r0, CLR0
mov r1, #(1 << 22)
str r1, [r0]

add r2, #1

b loop

FSEL2: .word 0x20200008
CLR0:  .word 0x20200028
SET0:  .word 0x2020001C
