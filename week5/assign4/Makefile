CFLAGS = -I$(CS107E)/include -g -Wall -Og -std=c99 -ffreestanding
LDFLAGS = -nostdlib -T memmap -L/usr/local/Cellar/arm-none-eabi/4.8.3-cs170e/lib/gcc/arm-none-eabi/4.8.3

.PRECIOUS: %.elf

app.bin = apps/heapclient.bin
test.bin = tests/test_backtrace_malloc.bin

all: $(app.bin) $(test.bin)

install: $(app.bin)
	rpi-install.py -p $<

test: $(test.bin)
	rpi-install.py -p $<

%.bin: %.elf
	arm-none-eabi-objcopy $< -O binary $@

%.elf: %.o start.o cstart.o gpio.o timer.o strings.o printf.o uart.o
	arm-none-eabi-gcc $(LDFLAGS) $^ -lgcc -o $@
	arm-none-eabi-objdump -D $@ > $@.list

%.o: %.c
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

%.o: %.s
	arm-none-eabi-as $< -o $@
