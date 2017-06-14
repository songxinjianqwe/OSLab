cmake_minimum_required(VERSION 3.7)
project(SleepingBarber)

set(CMAKE_C_STANDARD 99)

set(SOURCE_FILES include/const.h include/global.h include/proc.h include/protect.h include/proto.h
        include/sconst.inc include/string.h include/type.h
        kernel/main.c kernel/proc.c kernel/protect.c kernel/start.c kernel/syscall.asm kernel/clock.c
        kernel/global.c kernel/i8259.c kernel/kernel.asm lib/klib.c lib/kliba.asm lib/string.asm
        boot/boot.asm boot/loader.asm boot/include/fat12hdr.inc boot/include/load.inc boot/include/pm.inc kernel/semphore.c include/semphore.h)

add_executable(SleepingBarber ${SOURCE_FILES})