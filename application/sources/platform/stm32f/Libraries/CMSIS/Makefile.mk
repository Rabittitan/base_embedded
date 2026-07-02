
CFLAGS += -I./sources/platform/stm32f
CFLAGS += -I./sources/platform/stm32f/Libraries/CMSIS/CM3/CoreSupport
CFLAGS += -I./sources/platform/stm32f/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x
CFLAGS += -I./sources/platform/stm32f/Libraries/STM32F10x_StdPeriph_Driver/inc

CPPFLAGS += -I./sources/platform/stm32f
CPPFLAGS += -I./sources/platform/stm32f/Libraries/CMSIS/CM3/CoreSupport
CPPFLAGS += -I./sources/platform/stm32f/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x
CPPFLAGS += -I./sources/platform/stm32f/Libraries/STM32F10x_StdPeriph_Driver/inc


SOURCES += sources/platform/stm32f/Libraries/CMSIS/CM3/CoreSupport/core_cm3.c

VPATH += sources/platform/stm32f
