SUFFIX = arm-none-eabi-
CC      = $(SUFFIX)gcc
LD      = $(SUFFIX)gcc
AS      = $(SUFFIX)gcc -x assembler-with-cpp
AR      = $(SUFFIX)ar
OBJCOPY = $(SUFFIX)objcopy
OBJDUMP = $(SUFFIX)objdump
SIZE    = $(SUFFIX)size

LINKER = ../../cmsis/MKL46Z4/linker/MKL46Z256xxx4_flash.ld
CPU    = cortex-m0plus
INSTRUCTION = thumb
CHIP_FAMILY = MKL46Z4

OBJS = main.o

CFLAGS  = -c
CFLAGS += -g
cflags += -ggdb
CFLAGS += -Wall
CFLAGS += -ffunction-sections
CFLAGS += -fdata-sections
CFLAGS += -fno-builtin
CFLAGS += -Wno-unused-function
CFLAGS += -ffreestanding
CFLAGS += -MMD -MP
CFLAGS += -mcpu=$(CPU) -m$(INSTRUCTION)

ASFLAGS = $(CFLAGS)

LFLAGS = --specs=nosys.specs
LFLAGS += -T$(LINKER)
LFLAGS += -Wl,--gc-sections
LFLAGS += -mcpu=$(CPU) -m$(INSTRUCTION)

## main libraries
SYSTEM_SRC  = ../../cmsis/$(CHIP_FAMILY)

#Library headers
INC     = -I./
INC    += -I../../cmsis/arm/
INC    += -I../../cmsis/$(CHIP_FAMILY)/
CFLAGS += $(INC)


#https://www.gnu.org/software/make/manual/html_node/General-Search.html
VPATH	= $(SYSTEM_SRC)

#Capture all the c files
SYSTEM_OBJ_C_SRC  = $(wildcard $(SYSTEM_SRC)/*.c)
#Capture all the object file for the c files
SYSTEM_LIB_C_OBJS  = $(SYSTEM_OBJ_C_SRC:.c=.o)

#Capture all the s assembly files
SYSTEM_OBJ_s_SRC  = $(wildcard $(SYSTEM_SRC)/*.s)
#Capture all the object file for the s assembly files
SYSTEM_LIB_s_OBJS  = $(SYSTEM_OBJ_s_SRC:.s=.o)

#Capture all the s assembly files
SYSTEM_OBJ_S_SRC  = $(wildcard $(SYSTEM_SRC)/*.S)
#Capture all the object file for the s assembly files
SYSTEM_LIB_S_OBJS  = $(SYSTEM_OBJ_S_SRC:.S=.o)

#Remove the folder information for the c, s, S assembly files
SYSTEM_LOCAL_LIB_OBJS   = $(notdir $(SYSTEM_LIB_C_OBJS))
SYSTEM_LOCAL_LIB_OBJS  += $(notdir $(SYSTEM_LIB_s_OBJS))
SYSTEM_LOCAL_LIB_OBJS  += $(notdir $(SYSTEM_LIB_S_OBJS))

##Rules
all: main.bin

#Create the core.a file
core.a: $(SYSTEM_OBJ_SRC)
	$(MAKE) $(SYSTEM_LOCAL_LIB_OBJS)
	$(AR) rcs core.a $(SYSTEM_LOCAL_LIB_OBJS)
	rm -f $(SYSTEM_LOCAL_LIB_OBJS)

#This is required to capture the assembly code
%.o : %.s
	$(AS) $(ASFLAGS) -o $@ $<


main.elf: $(OBJS) core.a $(LINKER)
	$(LD) $(LFLAGS) -o main.elf $(OBJS) core.a

main.bin: main.elf
	$(OBJCOPY) --strip-unneeded -O binary $< $@

clean:
	rm -f $(OBJS) main.elf main.bin *.d core.a main.o


.PHONY: all clean
