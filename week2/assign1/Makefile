all : larson.bin larson-extended.bin brightness.bin

%.bin: %.o
	arm-none-eabi-objcopy $< -O binary $@

%.o: %.s
	arm-none-eabi-as $< -o $@

clean:
	rm -f *.o *.bin *~

.PHONY: all
