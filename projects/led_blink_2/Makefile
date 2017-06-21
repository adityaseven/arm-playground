
CC      = arm-none-eabi-gcc
LD      = arm-none-eabi-gcc
AR      = arm-none-eabi-as
AR      = arm-none-eabi-ar
OBJCOPY = arm-none-eabi-objcopy
OBJCOPY = arm-none-eabi-objdump
SIZE    = arm-none-eabi-size

CFLAGS  = -c -fno-common -Os -g
CFLAGS += -Wall -ffunction-sections -fdata-sections -fno-builtin
CFLAGS += -Wno-unused-function -ffreestanding
CFLAGS += -mcpu=cortex-m0plus -MMD -MP -mthumb

LFLAGS = -TMKL46Z256xxx4_flash.ld

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

all: main.bin

main.elf: $(OBJS) MKL46Z256xxx4_flash.ld
	$(LD) $(LFLAGS) -o main.elf $(OBJS)

main.bin: main.elf
	$(OBJCOPY) --strip-uneeded -O binary $< $@

clean:
	rm -f $(OBJS) main.elf main.bin


.PHONY: all clean
