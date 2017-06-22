
CC      = arm-none-eabi-gcc
LD      = arm-none-eabi-gcc
AS      = arm-none-eabi-gcc -x assembler-with-cpp
AR      = arm-none-eabi-ar
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
SIZE    = arm-none-eabi-size

CFLAGS  = -c
CFLAGS += -g
cflags += -ggdb

CFLAGS += -Wall

#Creates a function name .text section in the elf, it means that there is a
#corresponding section for each function.
# http://www.keil.com/support/man/docs/armclang_ref/armclang_ref_sam1445342406709.htm
# increases the size of the code
# https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html
# Not compatible with g?
CFLAGS += -ffunction-sections
CFLAGS += -fdata-sections

#https://gcc.gnu.org/onlinedocs/gcc-4.2.2/gcc/C-Dialect-Options.html
CFLAGS += -fno-builtin

#Remove Warn if a defined function is not used
#https://stackoverflow.com/questions/386220/how-can-i-hide-defined-but-not-used-warnings-in-gcc?rq=1
CFLAGS += -Wno-unused-function

#Tells the compiler not assume things, i.e that program starts at main
#https://stackoverflow.com/questions/17692428/what-is-ffreestanding-option-in-gcc
CFLAGS += -ffreestanding

#Print out the dependencies
CFLAGS += -MMD -MP

#Tell the compiler which cpur and instruction set to use
CFLAGS += -mcpu=cortex-m0plus -mthumb

ASFLAGS = $(CFLAGS)

#what are these lgcc lc and lnosys and why do they need to be in this order?
#what is Wl and gc-sections

LINKER = ../../cmsis/MKL46Z4/linker/MKL46Z256xxx4_flash.ld

#Attaches a stub version of system calls to the Program
LFLAGS  = -lgcc
LFLAGS += -lc
LFLAGS += -lnosys
LFLAGS += --specs=nosys.specs

#Add the linker
LFLAGS += -T$(LINKER)
LFLAGS += -Wl,--gc-sections
LFLAGS += -mcpu=cortex-m0plus -mthumb

#Library headers
CFLAGS += -I./
CFLAGS += -I../../cmsis/arm/
CFLAGS += -I../../cmsis/MKL46Z4/

#our code
OBJS = main.o

#startup files
OBJS += ../../cmsis/MKL46Z4/startup_MKL46Z4.o

#Library objects
OBJS += ../../cmsis/MKL46Z4/system_MKL46Z4.o

##Rules

#This is required to capture the assembly code
%.o : %.s
	$(AS) $(ASFLAGS) -o $@ $<

all: main.bin

main.elf: $(OBJS) $(LINKER)
	$(LD) $(LFLAGS) -o main.elf $(OBJS)

main.bin: main.elf
	$(OBJCOPY) --strip-unneeded -O binary $< $@

clean:
	rm -f $(OBJS) main.elf main.bin *.d


.PHONY: all clean
