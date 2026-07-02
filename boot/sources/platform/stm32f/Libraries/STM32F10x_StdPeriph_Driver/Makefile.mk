
CFLAGS += -I./sources/platform/stm32f/Libraries/STM32F10x_StdPeriph_Driver/inc

# Source search path
VPATH += ./sources/platform/stm32f/Libraries/STM32F10x_StdPeriph_Driver/src

# SPL source files
SOURCES += ./sources/platform/stm32f/Libraries/STM32F10x_StdPeriph_Driver/src/misc.c
SOURCES += ./sources/platform/stm32f/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_adc.c
SOURCES += ./sources/platform/stm32f/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_dac.c
SOURCES += ./sources/platform/stm32f/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_exti.c
SOURCES += ./sources/platform/stm32f/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_flash.c
SOURCES += ./sources/platform/stm32f/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_gpio.c
SOURCES += ./sources/platform/stm32f/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_i2c.c
SOURCES += ./sources/platform/stm32f/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_iwdg.c
SOURCES += ./sources/platform/stm32f/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_pwr.c
SOURCES += ./sources/platform/stm32f/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_rcc.c
SOURCES += ./sources/platform/stm32f/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_rtc.c
SOURCES += ./sources/platform/stm32f/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_spi.c
SOURCES += ./sources/platform/stm32f/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_tim.c
SOURCES += ./sources/platform/stm32f/Libraries/STM32F10x_StdPeriph_Driver/src/stm32f10x_usart.c