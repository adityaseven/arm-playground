
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
CFLAGS += -ffunction-sections
CFLAGS += -fdata-sections
CFLAGS += -fno-builtin
CFLAGS += -Wno-unused-function
CFLAGS += -ffreestanding
CFLAGS += -MMD -MP
CFLAGS += -mcpu=cortex-m0plus -mthumb

ASFLAGS = $(CFLAGS)

#what are these lgcc lc and lnosys and why do they need to be in this order?
#what is Wl and gc-sections

LINKER = ../../cmsis/MKL46Z4/linker/MKL46Z256xxx4_flash.ld

LFLAGS  = -lgcc
LFLAGS += -lc
LFLAGS += -lnosys
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
