all : flash

CH32V003FUN=../ch32v003fun
TARGET:=ws2812_circle

include ../ch32v003fun/ch32v003fun.mk

flash : $(TARGET).elf
	gdb-multiarch -x '../../.gdbinit' -ex 'load' -ex 'detach' -ex 'quit' $(TARGET).elf

clean : cv_clean


