include sources/platform/stm32f/arduino/cores/stm32/Makefile.mk

# header path
CPPFLAGS	+= -I./sources/platform/stm32f/arduino/cores

# sources path
VPATH += sources/platform/stm32f/arduino/cores

# cpp files
SOURCES_CPP += sources/platform/stm32f/arduino/cores/wiring_digital.cpp
SOURCES_CPP += sources/platform/stm32f/arduino/cores/wiring_shift.cpp
SOURCES_CPP += sources/platform/stm32f/arduino/cores/Print.cpp
SOURCES_CPP += sources/platform/stm32f/arduino/cores/Stream.cpp
SOURCES_CPP += sources/platform/stm32f/arduino/cores/WString.cpp
SOURCES_CPP += sources/platform/stm32f/arduino/cores/WMath.cpp
SOURCES_CPP += sources/platform/stm32f/arduino/cores/itoa.cpp
SOURCES_CPP += sources/platform/stm32f/arduino/cores/IPAddress.cpp
SOURCES_CPP += sources/platform/stm32f/arduino/cores/HardwareSerial.cpp
SOURCES_CPP += sources/platform/stm32f/arduino/cores/HardwareSerial2.cpp
