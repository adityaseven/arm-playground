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

SYSTEM_LIB = core.a
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

VPATH	= $(SYSTEM_SRC)

SYSTEM_OBJ_C_SRC  = $(wildcard $(SYSTEM_SRC)/*.c)
SYSTEM_LIB_C_OBJS  = $(SYSTEM_OBJ_C_SRC:.c=.o)

SYSTEM_OBJ_s_SRC  = $(wildcard $(SYSTEM_SRC)/*.s)
SYSTEM_LIB_s_OBJS  = $(SYSTEM_OBJ_s_SRC:.s=.o)

SYSTEM_OBJ_S_SRC  = $(wildcard $(SYSTEM_SRC)/*.S)
SYSTEM_LIB_S_OBJS  = $(SYSTEM_OBJ_S_SRC:.S=.o)

SYSTEM_LOCAL_LIB_OBJS   = $(notdir $(SYSTEM_LIB_C_OBJS))
SYSTEM_LOCAL_LIB_OBJS  += $(notdir $(SYSTEM_LIB_s_OBJS))
SYSTEM_LOCAL_LIB_OBJS  += $(notdir $(SYSTEM_LIB_S_OBJS))

##Rules
all: main.bin

#Create the core.a file
$(SYSTEM_LIB): $(SYSTEM_OBJ_SRC)
	$(MAKE) $(SYSTEM_LOCAL_LIB_OBJS)
	$(AR) rcs $(SYSTEM_LIB) $(SYSTEM_LOCAL_LIB_OBJS)
	rm -f $(SYSTEM_LOCAL_LIB_OBJS)

#This is required to capture the assembly code
%.o : %.s
	$(AS) $(ASFLAGS) -o $@ $<

main.elf: $(OBJS) $(SYSTEM_LIB) $(LINKER)
	$(LD) $(LFLAGS) -o main.elf $(OBJS) $(SYSTEM_LIB)

main.bin: main.elf
	$(OBJCOPY) --strip-unneeded -O binary $< $@

clean:
	rm -f $(OBJS) main.elf main.bin *.d $(SYSTEM_LIB) main.o


.PHONY: all clean
