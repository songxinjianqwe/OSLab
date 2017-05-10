##################################################
# Makefile
##################################################

# Entry point of Orange'S
# It must have the same value with 'KernelEntryPointPhyAddr' in load.inc!
ENTRYPOINT	= 0x30400

# Offset of entry point in kernel file
# It depends on ENTRYPOINT
ENTRYOFFSET	=   0x400

# Programs, flags, etc.
ASM		= nasm
DASM		= ndisasm
CC		= gcc
LD		= ld
ASMBFLAGS	= -I boot/include/
ASMKFLAGS	= -I include/ -f elf32
CFLAGS		= -I include/ -c -fno-builtin
LDFLAGS		= -m elf_i386 -s -Ttext $(ENTRYPOINT)

# This Program
ORANGESBOOT	= boot/boot.bin boot/loader.bin
ORANGESKERNEL	= kernel.bin
OBJS		= kernel/kernel.o #kernel/start.o lib/kliba.o lib/string.o
DASMOUTPUT	= kernel.bin.asm

IMG:=a.img
FLOPPY:=/mnt/floppy/

.PHONY : everything

everything : $(ORANGESBOOT) $(ORANGESKERNEL)

clean :
	rm -f $(OBJS) $(ORANGESBOOT) $(ORANGESKERNEL)

boot/boot.bin : boot/boot.asm boot/include/load.inc boot/include/fat12hdr.inc
	$(ASM) $(ASMBFLAGS) -o $@ $<

boot/loader.bin : boot/loader.asm boot/include/load.inc \
			boot/include/fat12hdr.inc boot/include/pm.inc
	$(ASM) $(ASMBFLAGS) -o $@ $<

$(ORANGESKERNEL) : $(OBJS)
	$(LD) $(LDFLAGS) -o $(ORANGESKERNEL) $(OBJS)

kernel/kernel.o : kernel/kernel.asm
	$(ASM) $(ASMKFLAGS) -o $@ $<


