# Makefile
# S11-G22
# 2011/05/22

CC=m68k-elf-gcc
CXX=m68k-elf-g++
#CFLAGS=-Wall -m5307 -pipe -nostdlib
#CFLAGS=-Wall -m5307 -pipe -nostdlib -D_DEBUG_ 
#CFLAGS=-Wall -m5307 -pipe -nostdlib -D_CFSERVER_
CFLAGS=-Wall -m5307 -pipe -nostdlib -D_CFSERVER_ -D_DEBUG_
LD=m68k-elf-gcc
AS=m68k-elf-as
AR=m68k-elf-ar
ARFLAGS=
OBJCPY=m68k-elf-objcopy
ASM=../shared/start.s
LDFLAGS=-T../shared/rtx.ld -Wl

OBJS=../dbug/dbug.o process.o

# Note, GCC builds things in order, it's important to put yhe
#  ASM first, so that it is located at the beginning of our program.
process.s19: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o process.bin $(ASM) $(OBJS) 
	$(OBJCPY) --output-format=srec process.bin process.s19

dbug.o: ../dbug/dbug.c ../dbug/dbug.h
	make -C ../dbug CFLAGS='$(CFLAGS)'

.PHONY: clean
clean:
	rm -f *.s19 *.o *.bin *.lst *.map ../dbug/dbug.o
