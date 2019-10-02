##########################################################################################################################
# File automatically-generated by tool: [projectgenerator] version: [3.3.0] date: [Thu Sep 19 11:35:54 ICT 2019]
##########################################################################################################################

# ------------------------------------------------
# Generic Makefile (based on gcc)
#
# ChangeLog :
#	2017-02-10 - Several enhancements + project update mode
#   2015-07-22 - first version
# ------------------------------------------------

######################################
# target
######################################
TARGET = atsh_fw

ifeq ($(OS),Windows_NT)
	OPENOCD_DIR ?= "D:\Projects\2016\tools\OpenOCD-0.9.0-Win32"
	OPENCM3_DIR ?= "D:\Projects\2016\tools\libopencm3"
	CC_PATH ?= "/tools/stm32/gcc/bin"
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Darwin)
		OPENOCD_DIR ?= "/Applications/GNU ARM Eclipse/OpenOCD/0.10.0-201610281609-dev"
		OPENCM3_DIR ?= /tools/stm32/libopencm3
		CC_PATH ?= /tools/stm32/gcc/bin
	endif
endif

#Check openocd install global
ISOCD_EXIST := $(shell command -v openocd 2> /dev/null)
ifndef ISOCD_EXIST
	OPENOCD    = $(OPENOCD_DIR)/bin/openocd
else
	OPENOCD    = openocd
endif



######################################
# building variables
######################################
# debug build?
DEBUG = 1
# optimization
OPT = -Og


#######################################
# paths
#######################################
# Build path
BUILD_DIR = build

######################################
# source
######################################
# C sources
C_SOURCES =  \
Src/main.c \
Src/gpio.c \
Src/blinkled.c \
Src/key.c \
Src/user_bitmap.c \
Src/logo.c \
Src/main_screen.c \
Src/info_screen.c \
Src/output.c \
Src/buzzer.c \
Src/i2c.c \
Src/spi.c \
Src/iwdg.c \
Src/usart.c \
Src/usb_device.c \
Src/usbd_conf.c \
Src/usbd_desc.c \
Src/usbd_cdc_if.c \
Src/stm32f1xx_it.c \
Src/stm32f1xx_hal_msp.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio_ex.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pcd.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pcd_ex.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_ll_usb.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_i2c.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_spi.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_iwdg.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_uart.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pwr.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash.c \
Drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash_ex.c \
Src/system_stm32f1xx.c \
Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c \
Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c \
Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c \
Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src/usbd_cdc.c \
u8g2/csrc/u8g2_bitmap.c \
u8g2/csrc/u8g2_box.c \
u8g2/csrc/u8g2_buffer.c \
u8g2/csrc/u8g2_circle.c \
u8g2/csrc/u8g2_cleardisplay.c \
u8g2/csrc/u8g2_d_memory.c \
u8g2/csrc/u8g2_d_setup.c \
u8g2/csrc/u8g2_font.c \
u8g2/csrc/u8g2_fonts.c \
u8g2/csrc/u8g2_hvline.c \
u8g2/csrc/u8g2_input_value.c \
u8g2/csrc/u8g2_intersection.c \
u8g2/csrc/u8g2_kerning.c \
u8g2/csrc/u8g2_line.c \
u8g2/csrc/u8g2_ll_hvline.c \
u8g2/csrc/u8g2_message.c \
u8g2/csrc/u8g2_polygon.c \
u8g2/csrc/u8g2_selection_list.c \
u8g2/csrc/u8g2_setup.c \
u8g2/csrc/u8x8_8x8.c \
u8g2/csrc/u8x8_byte.c \
u8g2/csrc/u8x8_cad.c \
u8g2/csrc/u8x8_d_a2printer.c \
u8g2/csrc/u8x8_d_st7920.c \
u8g2/csrc/u8x8_debounce.c \
u8g2/csrc/u8x8_display.c \
u8g2/csrc/u8x8_fonts.c \
u8g2/csrc/u8x8_gpio.c \
u8g2/csrc/u8x8_input_value.c \
u8g2/csrc/u8x8_message.c \
u8g2/csrc/u8x8_selection_list.c \
u8g2/csrc/u8x8_setup.c \
u8g2/csrc/u8x8_string.c \
u8g2/csrc/u8x8_u16toa.c \
u8g2/csrc/u8x8_u8toa.c 


# ASM sources
ASM_SOURCES =  \
startup_stm32f103xe.s


#######################################
# binaries
#######################################
PREFIX = arm-none-eabi-
# The gcc compiler bin path can be either defined in make command via GCC_PATH variable (> make GCC_PATH=xxx)
# either it can be added to the PATH environment variable.
ifdef GCC_PATH
CC = $(GCC_PATH)/$(PREFIX)gcc
AS = $(GCC_PATH)/$(PREFIX)gcc -x assembler-with-cpp
CP = $(GCC_PATH)/$(PREFIX)objcopy
SZ = $(GCC_PATH)/$(PREFIX)size
else
CC = $(PREFIX)gcc
AS = $(PREFIX)gcc -x assembler-with-cpp
CP = $(PREFIX)objcopy
SZ = $(PREFIX)size
endif
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S
 
#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m3

# fpu
# NONE for Cortex-M0/M0+/M3

# float-abi


# mcu
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

# macros for gcc
# AS defines
AS_DEFS = 

# C defines
C_DEFS =  \
-DUSE_HAL_DRIVER \
-DSTM32F103xE \
-DUSE_HAL_DRIVER \
-DSTM32F103xE


# AS includes
AS_INCLUDES = 

# C includes
C_INCLUDES =  \
-IInc \
-IDrivers/STM32F1xx_HAL_Driver/Inc \
-IDrivers/STM32F1xx_HAL_Driver/Inc/Legacy \
-IMiddlewares/ST/STM32_USB_Device_Library/Core/Inc \
-IMiddlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc \
-IDrivers/CMSIS/Device/ST/STM32F1xx/Include \
-IDrivers/CMSIS/Include \
-IDrivers/CMSIS/Include \
-Iu8g2/csrc



# compile gcc flags
ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS = $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif


# Generate dependency information
CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"


#######################################
# LDFLAGS
#######################################
# link script
LDSCRIPT = STM32F103RDTx_FLASH.ld

# libraries
LIBS = -lc -lm -lnosys -u _printf_float
LIBDIR = 
LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# default action: build all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin


#######################################
# build the application
#######################################
# list of objects
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	$(BIN) $< $@	
	
$(BUILD_DIR):
	mkdir $@		

flash:
	$(OPENOCD) 	-s $(OPENOCD_DIR)\
			   	-f interface/stlink-v2.cfg\
				-f target/stm32f1x_stlink.cfg\
		        -c init -c targets -c "reset halt" \
		        -c "flash write_image erase $(BUILD_DIR)/$(TARGET).hex" \
		        -c "verify_image $(BUILD_DIR)/$(TARGET).hex" \
		        -c "reset run" -c shutdown

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)
  
#######################################
# dependencies
#######################################
-include $(wildcard $(BUILD_DIR)/*.d)

# *** EOF ***