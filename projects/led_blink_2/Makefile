
CC      = arm-none-eabi-gcc
LD      = arm-none-eabi-gcc
AS      = arm-none-eabi-gcc -x assembler-with-cpp
AR      = arm-none-eabi-ar
OBJCOPY = arm-none-eabi-objcopy
OBJDUMP = arm-none-eabi-objdump
SIZE    = arm-none-eabi-size

CFLAGS  = -c -g -ggdb
CFLAGS += -Wall -ffunction-sections -fdata-sections -fno-builtin
CFLAGS += -Wno-unused-function -ffreestanding
CFLAGS += -mcpu=cortex-m0plus -MMD -MP -mthumb

#what are these lgcc lc and lnosys and why do they need to be in this order?
#what is Wl and gc-sections
LFLAGS  = -lgcc -lc -lnosys  -TMKL46Z256xxx4_flash.ld  -Wl,--gc-sections -mcpu=cortex-m0plus -mthumb

#Library headers
CFLAGS += -I./
CFLAGS += -I../../cmsis/arm/
CFLAGS += -I../../cmsis/MKL46Z4/

#our code
OBJS = main.o

#startup files
OBJS += startup_MKL46Z4.o

#Library objects
OBJS += ../../cmsis/MKL46Z4/system_MKL46Z4.o

#http://mcufreaks.blogspot.com/2013/03/customizable-makefile-for-gcc-arm.html
#embed with elliot
##Rules

#This is required to capture the assembly code
%.o : %.s
	$(AS) $(CFLAGS) -o $@ $<


all: main.bin

main.elf: $(OBJS) MKL46Z256xxx4_flash.ld
	$(LD) $(LFLAGS) -o main.elf $(OBJS)

main.bin: main.elf
	$(OBJCOPY) --strip-unneeded -O binary $< $@

clean:
	rm -f $(OBJS) main.elf main.bin


.PHONY: all clean
