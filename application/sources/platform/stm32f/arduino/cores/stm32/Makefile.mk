#stm32 libs path
CPPFLAGS	+= -I./sources/platform/stm32f/Libraries/STM32F10x_StdPeriph_Driver/inc
CPPFLAGS	+= -I./sources/platform/stm32f/Libraries/CMSIS/Device/ST/STM32F10x/Include
CPPFLAGS	+= -I./sources/platform/stm32f/Libraries/CMSIS/Include

CFLAGS		+= -I./sources/platform/stm32f/arduino/cores/stm32
CPPFLAGS	+= -I./sources/platform/stm32f/arduino/cores/stm32

VPATH += sources/platform/stm32f/arduino/cores/stm32

SOURCES += sources/platform/stm32f/arduino/cores/stm32/dtostrf.c
SOURCES += sources/platform/stm32f/arduino/cores/stm32/hooks.c
