include sources/platform/stm32f/Libraries/STM32F10x_StdPeriph_Driver/Makefile.mk
include sources/platform/stm32f/Libraries/CMSIS/Makefile.mk
include sources/platform/stm32f/arduino/Makefile.mk

LDFILE = sources/platform/stm32f/ak.ld

CFLAGS += -I./sources/platform/stm32f

CPPFLAGS += -I./sources/platform/stm32f

VPATH += sources/platform/stm32f

# C source files
SOURCES += sources/platform/stm32f/platform.c
SOURCES += sources/platform/stm32f/system.c
SOURCES += sources/platform/stm32f/sys_cfg.c
SOURCES += sources/platform/stm32f/io_cfg.c
SOURCES += sources/platform/stm32f/system_stm32f10x.c

# C++ source files
SOURCES_CPP += sources/platform/stm32f/mini_cpp.cpp

# ASM source files
SOURCES_ASM += sources/platform/stm32f/startup_stm32f10x_md.s