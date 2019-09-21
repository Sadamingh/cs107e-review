CFLAGS = -g -Wall -O2 -std=c99 -ffreestanding

all : blink.bin

%.bin: %.o
	arm-none-eabi-objcopy $< -O binary $@

%.o: %.c
	arm-none-eabi-gcc $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o *.bin *~

.PHONY: all clean

.PRECIOUS: %.o
