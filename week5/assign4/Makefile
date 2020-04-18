CFLAGS = -I$(CS107E)/include -g -Wall -Og -std=c99 -ffreestanding -mpoke-function-name -mapcs-frame -fno-omit-frame-pointer
LDFLAGS = -nostdlib -T memmap -L/usr/local/Cellar/arm-none-eabi/4.8.3-cs170e/lib/gcc/arm-none-eabi/4.8.3

.PRECIOUS: %.elf

app.bin = apps/heapclient.bin
test.bin = tests/test_backtrace_malloc.bin

all: $(app.bin) $(test.bin)

install: $(app.bin)
	rpi-install.py -p $<

test: $(test.bin)
	rpi-install.py -p $<

app.bin: $(app.bin)

test.bin: $(test.bin)

test.bin.gdb: test.bin
	arm-none-eabi-gdb $(subst .bin,.elf,$(test.bin))

%.bin: %.elf
	arm-none-eabi-objcopy $< -O binary $@

%.elf: %.o start.o cstart.o gpio.o timer.o strings.o printf.o uart.o backtrace.o malloc.o
	arm-none-eabi-gcc $(LDFLAGS) $^ -lgcc -o $@
	arm-none-eabi-objdump -D $@ > $@.list

%.o: %.c Makefile
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

%.o: %.s Makefile
	arm-none-eabi-as $< -o $@
