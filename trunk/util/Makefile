# Makefile
# S11-G22
# 2011/05/18

CC=m68k-elf-gcc
CXX=m68k-elf-g++
CFLAGS=-Wall -m5307 -pipe -nostdlib
LD=m68k-elf-gcc
AS=m68k-elf-as
AR=m68k-elf-ar
ARFLAGS=

util.o: util.c util.h
	$(CC) $(CFLAGS) -c util.c 

.PHONY: clean
clean:
	rm -f *.o *.s19 *.bin
