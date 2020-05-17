CFLAGS = -I$(CS107E)/include -g -Wall -Og -std=c99 -ffreestanding -mpoke-function-name -mapcs-frame -fno-omit-frame-pointer
LDFLAGS = -nostdlib -T memmap -L/usr/local/Cellar/arm-none-eabi/4.8.3-cs170e/lib/gcc/arm-none-eabi/4.8.3 -L$(CS107E)/lib

common_objects := start.o cstart.o gpio.o timer.o strings.o uart.o printf.o backtrace.o malloc.o assert.o

.PRECIOUS: %.elf

app.bin := apps/uart_shell.bin
test.bin := tests/test_keyboard.bin

default: $(app.bin) $(test.bin)

install: $(app.bin)
	rpi-install.py -p $<

test: $(test.bin)
	rpi-install.py -p $<

%.bin: %.elf
	arm-none-eabi-objcopy $< -O binary $@

%.elf: %.o start.o cstart.o shell.o keyboard.o
	arm-none-eabi-gcc $(LDFLAGS) $^ -lgcc -lpi -o $@

%.o: %.c Makefile
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

%.o: %.s Makefile
	arm-none-eabi-as $< -o $@
