export GDK ?= /opt/toolchains/mars/m68k-elf
ROM_NAME := hello-controller

# extra include path for the shared common lib (not overwritten by makefile.gen)
EXTRA_FLAGS := -I../../inc
override SRC_C := $(wildcard *.c) $(wildcard src/*.c)
override SRC_C := $(filter-out src/rom_header.c,$(SRC_C))
override SRC_C += ../../src/Controller.c

include $(GDK)/makefile.gen

run:
	kega-fusion out/hello-controller.bin