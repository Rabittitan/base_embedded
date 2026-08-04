# Tên module
NAME_MODULE = ak-base-kit-stm32f103-boot

# Đường dẫn tools
GCC_PATH = $(HOME)/workspace/Tools/gcc-arm-none-eabi-10.3-2021.10
PROGRAMER_PATH = $(HOME)/workspace/Tools/bin

# SOC layout
FLASH_BASE_ADDR_VAL = 0x08000000
SOC_FLASH_BYTES      = 65536
SOC_SRAM_BYTES       = 20480
SOC_NAME             = STM32F103C8T6

# Bootloader cấu hình
BOOT_START_ADDR_VAL  = 0x08000000

BOOT_SIZE_VAL        = 12288
APP_START_ADDR_VAL   = 0x08003000
APP_ADDR_OPTION      = -DAPP_START_ADDR=$(APP_START_ADDR_VAL) -DBOOT_SIZE=$(BOOT_SIZE_VAL)

# Console
SYS_CONSOLE_BAUDRATE = 115200
CONSOLE_BAUDRATE_DEF = -DSYS_CONSOLE_BAUDRATE=$(SYS_CONSOLE_BAUDRATE)

# Tối ưu
OPTIMIZE_OPTION = -g -Os

# Include path (phải đặt trước khi include các .mk)
CFLAGS   += -I./sources/platform/stm32f
CFLAGS   += -I./sources/platform/stm32f/Libraries/STM32F10x_StdPeriph_Driver/inc
CFLAGS   += -I./sources/platform/stm32f/Libraries/CMSIS/CM3/CoreSupport
CFLAGS   += -I./sources/platform/stm32f/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x
CPPFLAGS += $(CFLAGS)   # CPP dùng chung include path

# Include các module cần thiết
include sources/platform/stm32f/Libraries/STM32F10x_StdPeriph_Driver/Makefile.mk
include sources/platform/stm32f/Libraries/CMSIS/Makefile.mk

# Linker script
LDFILE = sources/platform/stm32f/ak.ld

# Thư mục build
OBJECTS_DIR = build_$(NAME_MODULE)
TARGET = $(OBJECTS_DIR)/$(NAME_MODULE).axf

# Nguồn bắt buộc cho bootloader
SOURCES     += sources/platform/stm32f/platform.c
SOURCES     += sources/platform/stm32f/system.c
SOURCES     += sources/platform/stm32f/sys_cfg.c
SOURCES     += sources/platform/stm32f/io_cfg.c
SOURCES     += sources/platform/stm32f/system_stm32f10x.c
SOURCES_CPP += sources/platform/stm32f/mini_cpp.cpp
SOURCES_ASM += sources/platform/stm32f/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/arm/startup_stm32f10x_md.s
SOURCES_CPP += sources/main.cpp
SOURCES     += sources/sys/syscalls.c
LDFLAGS += -nostartfiles
# Loại bỏ tiền tố ./ nếu có
SOURCES     := $(patsubst ./%,%,$(SOURCES))
SOURCES_CPP := $(patsubst ./%,%,$(SOURCES_CPP))
SOURCES_ASM := $(patsubst ./%,%,$(SOURCES_ASM))

# Danh sách object
OBJECTS  = $(SOURCES:%.c=$(OBJECTS_DIR)/%.o)
OBJECTS += $(SOURCES_CPP:%.cpp=$(OBJECTS_DIR)/%.o)
OBJECTS += $(SOURCES_ASM:%.s=$(OBJECTS_DIR)/%.o)

# Thư viện GCC
LIBC = $(GCC_PATH)/arm-none-eabi/lib/thumb/v7-m/nofp/libc.a
LIBM = $(GCC_PATH)/arm-none-eabi/lib/thumb/v7-m/nofp/libm.a
LIBSTDCPP_NANO = $(GCC_PATH)/arm-none-eabi/lib/thumb/v7-m/nofp/libstdc++_nano.a
LIBGCC = $(GCC_PATH)/lib/gcc/arm-none-eabi/10.3.1/thumb/v7-m/nofp/libgcc.a

LIB_PATH  = -L$(GCC_PATH)/arm-none-eabi/lib/thumb/v7-m/nofp
LIB_PATH += -L$(GCC_PATH)/lib/gcc/arm-none-eabi/10.3.1/thumb/v7-m/nofp

# Compiler
CC   = $(GCC_PATH)/bin/arm-none-eabi-gcc
CPP  = $(GCC_PATH)/bin/arm-none-eabi-g++
LD   = $(GCC_PATH)/bin/arm-none-eabi-ld
OBJCOPY = $(GCC_PATH)/bin/arm-none-eabi-objcopy

# Flags chung
CPU = -mthumb -march=armv7-m
FPU = -mfloat-abi=soft

COMMON_FLAGS  = $(CPU) $(FPU) -ffunction-sections -fdata-sections -MD -Wall
COMMON_FLAGS += -DUSE_STDPERIPH_DRIVER -DSTM32F10X_MD -DSTM32F_PLATFORM_BOOT
COMMON_FLAGS += $(APP_ADDR_OPTION) $(CONSOLE_BAUDRATE_DEF) $(OPTIMIZE_OPTION)
COMMON_FLAGS += -DLOGIN_PRINT_EN -DSYS_PRINT_EN

CFLAGS   += $(COMMON_FLAGS) -std=c99
CPPFLAGS += $(COMMON_FLAGS) -std=c++11 -fno-rtti -fno-exceptions -fno-use-cxa-atexit

LDFLAGS  = -Map=$(OBJECTS_DIR)/$(PROJECT).map --gc-sections $(LIB_PATH)
LDFLAGS += $(LIBC) $(LIBM) $(LIBSTDCPP_NANO) $(LIBGCC)

# Build
all: create $(TARGET)

create:
	@mkdir -p $(OBJECTS_DIR)

$(TARGET): $(OBJECTS)
	@$(LD) --entry reset_handler -T $(LDFILE) $(LDFLAGS) -o $@ $^
	@$(OBJCOPY) -O binary $@ $(@:.axf=.bin)
	@$(ARM_SIZE) $@

$(OBJECTS_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "~ CC $@"
	@$(CC) $(CFLAGS) -o $@ $<

$(OBJECTS_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	@echo "~ CXX $@"
	@$(CPP) $(CPPFLAGS) -o $@ $<

$(OBJECTS_DIR)/%.o: %.s
	@mkdir -p $(dir $@)
	@echo "~ AS $@"
	@$(AS) $(CFLAGS) -o $@ $<

.PHONY: flash
flash: all
	$(PROGRAMER_PATH)/STM32_Programmer.sh -c port=SWD -w $(TARGET:.axf=.bin) $(BOOT_START_ADDR_VAL) -rst

.PHONY: clean
clean:
	rm -rf $(OBJECTS_DIR)