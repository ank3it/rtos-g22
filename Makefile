# Makefile
# S11-G22
# 2011/05/18

CC=m68k-elf-gcc
CXX=m68k-elf-g++
#CFLAGS= -Wall -m5307 -pipe -nostdlib
#CFLAGS= -Wall -m5307 -pipe -nostdlib -D_DEBUG_
#CFLAGS= -Wall -m5307 -pipe -nostdlib -D_CFSERVER_
CFLAGS= -Wall -m5307 -pipe -nostdlib -D_CFSERVER_ -D_DEBUG_
LD=m68k-elf-gcc
AS=m68k-elf-as
AR=m68k-elf-ar
ARFLAGS=
OBJCPY=m68k-elf-objcopy
ASM=shared/start.s
LDFLAGS = -Tshared/rtx.ld -Wl,-Map=main.map

OBJS=dbug/dbug.o memory/memory.o main.o

# Note, GCC builds things in order, it's important to put yhe
#  ASM first, so that it is located at the beginning of our program.
main.s19: $(OBJS) 
	$(CC) $(CFLAGS) $(LDFLAGS) -o main.bin $(ASM) $(OBJS) 
	$(OBJCPY) --output-format=srec main.bin main.s19

memory/memory.o: memory/memory.c memory/memory.h
	make -C memory/ CFLAGS='$(CFLAGS)'

dbug/dbug.o: dbug/dbug.c dbug/dbug.h
	make -C dbug/ CFLAGS='$(CFLAGS)'

.PHONY: clean
clean:
	rm -f *.s19 *.o *.bin *.map memory/*.o dbug/*.o
