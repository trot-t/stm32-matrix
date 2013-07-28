# http://www.nabiltewolde.com/2011/10/using-stm32f4-discovery-board-in-osx.html
# http://nis-embedded.blogspot.com/2012/03/i-stm32f4-discovery-part5-make-full.html
TARGET:=matrix
TOOLCHAIN_PATH:=~/gcc-arm-none-eabi-4_6-2012q1/bin
#TOOLCHAIN_PATH:=~/sat/bin
TOOLCHAIN_PREFIX:=arm-none-eabi
OPTLVL:=2 # Optimization level, can be [0, 1, 2, 3, s].

ARM_MATH_CM4:=1

PROJECT_NAME:=$(notdir $(lastword $(CURDIR)))
TOP:=$(shell readlink -f "../../..")
DISCOVERY:=$(TOP)/Utilities/STM32F4-Discovery
STMLIB:=$(TOP)/Libraries
STD_PERIPH:=$(STMLIB)/STM32F4xx_StdPeriph_Driver
STARTUP:=
#$(STMLIB)/CMSIS/ST/STM32F4xx/Source/Templates/gcc_ride7
LINKER_SCRIPT=stm32_flash.ld 


INCLUDE=-I$(CURDIR)
INCLUDE+=-I$(STMLIB)/CMSIS/Include
INCLUDE+=-I$(STMLIB)/CMSIS/ST/STM32F4xx/Include
INCLUDE+=-I$(STD_PERIPH)/inc
INCLUDE+=-I$(DISCOVERY)
INCLUDE+=-I$(STMLIB)/CMSIS/DSP_Lib/Examples/Common/Include

# vpath is used so object files are written to the current directory instead
# of the same directory as their source files
vpath %.c $(DISCOVERY) $(STD_PERIPH)/src
vpath %.s $(STARTUP)

ASRC=startup_stm32f4xx.s

# Project Source Files
#SRC=stm32f4xx_it.c   # используем прерывания
SRC+=system_stm32f4xx.c
SRC+=main.c

# Discovery Source Files
SRC+=stm32f4_discovery.c

# Standard Peripheral Source Files
SRC+=stm32f4xx_rcc.c
SRC+=misc.c
SRC+=stm32f4xx_gpio.c
#SRC+=stm32f4xx_exti.c
#SRC+=stm32f4xx_syscfg.c

# math
#SRC+=arm_mult_f32.c
#SRC+=arm_add_f32.c
#SRC+=arm_sin_cos_f32.c
#SRC+=arm_sin_f32.c
#SRC+=arm_cos_f32.c
SRC+=arm_mat_init_f32.c
#SRC+=arm_mat_trans_f32.c
SRC+=arm_mat_mult_f32.c 
#SRC+=arm_mat_inverse_f32.c

SRC+=math_helper.c


CDEFS=-DSTM32F4XX -DARM_MATH_CM4 -D__FPU_PRESENT=1 -D__FPU_USED=1
CDEFS+=-DUSE_STDPERIPH_DRIVER

#MCUFLAGS=-mcpu=cortex-m4 -mthumb 
#MCUFLAGS=-mcpu=cortex-m4 -march=armv7e-m -mthumb -mlittle-endian -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb-interwork
MCUFLAGS=-mcpu=cortex-m4 -mhard-float -mthumb 

#COMMONFLAGS=-O$(OPTLVL) -g -Wall 
COMMONFLAGS=-O$(OPTLVL) -Wall 
###-Werror 
CFLAGS=$(COMMONFLAGS) $(MCUFLAGS) $(INCLUDE) $(CDEFS)

#LDLIBS=-lm
#LDFLAGS=$(COMMONFLAGS) -Wl,--gc-sections,-T$(LINKER_SCRIPT)

#LDFLAGS=$(COMMONFLAGS) -fno-exceptions -ffunction-sections -fdata-sections \
        -nostartfiles -Wl,-Map=demo.map -Wl,--gc-sections,-T$(LINKER_SCRIPT) \
	-Wl,-cref -mthumb -mcpu=cortex-m4 -march=armv7e-m -mfpu=fpv4-sp-d16 \
	-mfloat-abi=hard -mthumb-interwork
LDFLAGS=$(COMMONFLAGS) -fno-exceptions -ffunction-sections -fdata-sections \
	-nostartfiles -Wl,-Map=demo.map -Wl,--gc-sections,-T$(LINKER_SCRIPT) \
	-Wl,-cref -mthumb -mcpu=cortex-m4 -march=armv7e-m -mhard-float 

#LDFLAGS=$(COMMONFLAGS) -fno-exceptions -ffunction-sections -fdata-sections \
        -nostartfiles -Wl,-Map=demo.map -Wl,--gc-sections,-T$(LINKER_SCRIPT) 
#####
#####

OBJ = $(SRC:%.c=%.o) $(ASRC:%.s=%.o)

CC=$(TOOLCHAIN_PATH)/$(TOOLCHAIN_PREFIX)-gcc
LD=$(TOOLCHAIN_PATH)/$(TOOLCHAIN_PREFIX)-gcc
OBJCOPY=$(TOOLCHAIN_PATH)/$(TOOLCHAIN_PREFIX)-objcopy
AS=$(TOOLCHAIN_PATH)/$(TOOLCHAIN_PREFIX)-as
AR=$(TOOLCHAIN_PATH)/$(TOOLCHAIN_PREFIX)-ar
GDB=$(TOOLCHAIN_PATH)/$(TOOLCHAIN_PREFIX)-gdb


all: $(OBJ)
	$(CC) -o $(TARGET).elf $(LDFLAGS) $(OBJ)	$(LDLIBS)
	$(OBJCOPY) -O ihex   $(TARGET).elf $(TARGET).hex
	$(OBJCOPY) -O binary $(TARGET).elf $(TARGET).bin

.PHONY: clean

clean:
	rm -f $(OBJ)
	rm -f $(TARGET).elf
	rm -f $(TARGET).hex
	rm -f $(TARGET).bin
