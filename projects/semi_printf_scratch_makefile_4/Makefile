SUFFIX = arm-none-eabi-
CC      = $(SUFFIX)gcc
CXX     = $(SUFFIX)g++
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
CFLAGS += -ggdb
CFLAGS += -Wall
CFLAGS += -ffunction-sections
CFLAGS += -fdata-sections
CFLAGS += -fno-builtin
CFLAGS += -Wno-unused-function
CFLAGS += -ffreestanding
CFLAGS += -MMD -MP
CFLAGS += -mcpu=$(CPU) -m$(INSTRUCTION)

ASFLAGS = $(CFLAGS)

# To disable, run "make ENABLE_SEMIHOSTING=0" or comment next line out
ENABLE_SEMIHOSTING ?= 1

ifeq ($(ENABLE_SEMIHOSTING),1)
#Added --specs=rdimon.specs for semihosting support
#http://bgamari.github.io/posts/2014-10-31-semihosting.html
LFLAGS       = --specs=rdimon.specs
LFLAGS       += -lc -lrdimon
CFLAGS       += -DENABLE_SEMIHOSTING=1
else
LFLAGS        = --specs=nosys.specs
endif

#Added nano.specs to link with newlib-nano decreases the size dramatically and
#allows for semihosting
#https://mcuoneclipse.com/2014/07/11/switching-arm-gnu-tool-chain-and-libraries-in-kinetis-design-studio/
LFLAGS += --specs=nano.specs
LFLAGS += -T$(LINKER)
#Add main.map shows how the memory is loaded
LFLAGS += -Wl,-Map=main.map,--gc-sections
LFLAGS += -mcpu=$(CPU) -m$(INSTRUCTION)

## main libraries
SYSTEM_SRC  = ../../cmsis/$(CHIP_FAMILY)

#Library headers
INC     = -I./
INC    += -I../../cmsis/arm/
INC    += -I../../cmsis/$(CHIP_FAMILY)/
CFLAGS += $(INC)

VPATH	= $(SYSTEM_SRC)

#All source/object files

#patsubst
#https://www.gnu.org/software/make/manual/html_node/Text-Functions.html
C_SRCS  = $(foreach dir,$(SYSTEM_SRC),$(wildcard $(dir)/*.c))
C_OBJS  = $(patsubst %.c, %.o, $(notdir $(C_SRCS)))
OBJS   += $(C_OBJS)

CPP_SRCS  = $(foreach dir,$(SYSTEM_SRC),$(wildcard $(dir)/*.cpp))
CPP_OBJS  = $(patsubst %.cpp, %.o, $(notdir $(CPP_SRCS)))
OBJS   += $(CPP_OBJS)

s_SRCS  = $(foreach dir,$(SYSTEM_SRC),$(wildcard $(dir)/*.s))
s_OBJS  = $(patsubst %.s, %.o, $(notdir $(s_SRCS)))
OBJS   += $(s_OBJS)

S_SRCS  = $(foreach dir,$(SYSTEM_SRC),$(wildcard $(dir)/*.S))
S_OBJS  = $(patsubst %.S, %.o, $(notdir $(S_SRCS)))
OBJS   += $(S_OBJS)

##Rules

#Capture the following files and apply the rules below to convert them to objects

%.o : %.s
	$(AS) $(ASFLAGS) -o $@ $<

%.o : %.S
	$(AS) $(ASFLAGS) -o $@ $<

%.o : %.c
	$(CC) $(CFLAGS) -o $@ $<

%.o : %.cpp
	$(CC) $(CXXFLAGS) -o $@ $<

all: exec bin info
	$(LD) $(LFLAGS) -o main.elf $(OBJS)

exec: $(OBJS)
	$(LD) $(LFLAGS) -o main.elf $(OBJS)

bin:
	$(OBJCOPY) --strip-unneeded -O binary main.elf main.bin

#Get the information out, helps if the file is too big to load into memory
#and if openocd . gdb work here.
#https://mcuoneclipse.com/2013/04/14/text-data-and-bss-code-and-data-size-explained/
info:
	$(SIZE) -A main.elf
	$(SIZE) -B main.elf

clean:
	rm -f $(OBJS) main.elf main.bin main.map *.d $(SYSTEM_LIB) main.o


.PHONY: all clean bin exec
