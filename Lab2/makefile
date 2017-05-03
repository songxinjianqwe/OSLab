Main : my_print.o FAT12Reader.o  
# Target executable file name is Main, it needs some objective files
	gcc -m32 FAT12Reader.o my_print.o -o Main  
# link all the objective files to the executable

my_print.o :my_print.asm  
# first objective file need to be created through the following orders
# The names in the same row with objective file is the files that dependency files, source files usually
	nasm -f elf32 my_print.asm  -o my_print.o
# make order 
FAT12Reader.o:FAT12Reader.c 
	gcc -c -m32 FAT12Reader.c -o FAT12Reader.o
clean:
# use `make clean` order will clean all the objective files and target executable file
	rm -rf *.o Main

