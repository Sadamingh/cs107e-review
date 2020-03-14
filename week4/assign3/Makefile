CFLAGS = -I$(CS107E)/include -Wall -Og -std=c99 -ffreestanding 
LDFLAGS = -nostdlib -T memmap

.PRECIOUS: %.elf

all: apps/print_pinout.bin

%.bin: %.elf
	arm-none-eabi-objcopy $< -O binary $@

%.elf: %.o start.o cstart.o printf.o strings.o gpio.o timer.o uart.o
	arm-none-eabi-gcc $(LDFLAGS) $^ -o $@

%.o: %.c
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

%.o: %.s
	arm-none-eabi-as $< -o $@

install: apps/print_pinout.bin
	rpi-install.py -p $<
.PHONY: install

test: tests/test_strings_printf.bin
	rpi-install.py $<
.PHONY: test

clean:
	rm -f *.o *.bin *.elf
