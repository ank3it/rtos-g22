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

OBJS=../dbug/dbug.o ../util/util.o  malloc.o

malloc.o: malloc.c malloc.h
	$(CC) $(CFLAGS) -c malloc.c

dbug.o: ../dbug/dbug.c ../dbug/dbug.h
	make -C ../dbug CFLAGS='$(CFLAGS)'

util.o: ../util/util.c ../util/util.h
	make -C ../util CFLAGS='$(CFLAGS)'

.PHONY: clean
clean:
	rm -f *.s19 *.o *.bin *.lst *.map
	make clean -C ../dbug/
	make clean -C ../util/
