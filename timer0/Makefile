
# Makefile
# David Grant, 2004
# Irene Huang, 2011/01/04

CC=m68k-elf-gcc
CXX=m68k-elf-g++
CFLAGS=-Wall -m5307 -pipe -nostdlib
#CFLAGS=-Wall -m5307 -pipe -nostdlib -D_DEBUG_ 
#CFLAGS=-Wall -m5307 -pipe -nostdlib -D_CFSERVER_ -D_DEBUG_
LD=m68k-elf-gcc
AS=m68k-elf-as
AR=m68k-elf-ar
ARFLAGS=
OBJCPY=m68k-elf-objcopy
OBJDMP=m68k-elf-objdump
ASM=../shared/start.s ../uart1/serial_entry.s timer_entry.s
LDFLAGS=-T../shared/rtx.ld -Wl,-Map=timer.map

TIMER_OBJS=../uart1/uart1.o timer.o

# Note, GCC builds things in order, it's important to put yhe
#  ASM first, so that it is located at the beginning of our program.
timer.s19: timer_entry.s $(TIMER_OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o timer.bin $(ASM) $(TIMER_OBJS) 
	$(OBJCPY) --output-format=srec timer.bin timer.s19
	$(OBJDMP) -xdC timer.bin > timer.lst
	chmod u+x timer.s19

timer.o: timer.c
	$(CC) $(CFLAGS) -c timer.c

../uart1/uart1.o: ../uart1/uart1.c ../uart1/uart1.h
	make -C ../uart1/

.PHONY: clean
clean:
	rm -f *.s19 *.o *.bin *.lst *.map
	make clean -C ../uart1/
