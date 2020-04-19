CFLAGS = -I$(CS107E)/include -g -Wall -Og -std=c99 -ffreestanding -mpoke-function-name -mapcs-frame -fno-omit-frame-pointer
LDFLAGS = -nostdlib -T memmap -L/usr/local/Cellar/arm-none-eabi/4.8.3-cs170e/lib/gcc/arm-none-eabi/4.8.3

common_objects = start.o cstart.o gpio.o timer.o strings.o uart.o backtrace.o malloc.o assert.o

.PRECIOUS: %.elf

app.bin = apps/heapclient.bin
test.bin = tests/test_backtrace_malloc.bin

all: $(app.bin) $(test.bin)

app.bin: $(app.bin)

install: $(app.bin)
	rpi-install.py -p $<
.PHONY: install

test: $(test.bin)
	rpi-install.py -p $<
.PHONY: test

test.bin: $(test.bin)

test.bin.gdb: $(subst .bin,.gdb.bin,$(test.bin))
	arm-none-eabi-gdb $(subst .bin,.elf, $<)

%.bin: %.elf
	arm-none-eabi-objcopy $< -O binary $@

%.gdb.elf: %.gdb.o $(common_objects) printf.gdb.o
	arm-none-eabi-gcc $(LDFLAGS) $^ -lgcc -o $@
	arm-none-eabi-objdump -D $@ > $@.list

%.elf: %.o $(common_objects) printf.o
	arm-none-eabi-gcc $(LDFLAGS) $^ -lgcc -o $@
	arm-none-eabi-objdump -D $@ > $@.list

%.gdb.o: %.c Makefile
	arm-none-eabi-gcc -DGDB_DEBUG=1 $(CFLAGS) -c $< -o $@

%.o: %.c Makefile
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

%.o: %.s Makefile
	arm-none-eabi-as $< -o $@
