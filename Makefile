# Name: Makefile
# Author: Michael Martens
# Copyright: 
# License: 

# DEVICE ....... The AVR device you compile for
# CLOCK ........ Target AVR clock rate in Hertz
# OBJECTS ...... The object files created from your source files. This list is
#                usually the same as the list of source files with suffix ".o".
# PROGRAMMER ... Options to avrdude which define the hardware you use for
#                uploading to the AVR and the interface where this hardware
#                is connected.
# FUSES ........ Parameters for avrdude to flash the fuses appropriately.

DEVICE     = atmega324pa
CLOCK      = 8000000
PROGRAMMER = -c stk500v2 -P /dev/ttyUSB0 
OBJECTS    = main.o
LIBOBJECTS = at324hw.o scheduler.o pwm.o adc.o usart.o pcint2.o
FUSES      = -U lfuse:w:0x62:m -U hfuse:w:0xd9:m -U efuse:w:0xff:m
ARFLAGS    = -r
AR         = avr-ar
RANLIB     = avr-ranlib

######################################################################
######################################################################

# Tune the lines below only if you know what you are doing:

AVRDUDE = avrdude $(PROGRAMMER) -p m324pa
COMPILE = avr-gcc -g -Wall -Os -DF_CPU=$(CLOCK) -mmcu=$(DEVICE)

# symbolic targets:
all:	main.hex libmsr.a

.c.o:
	$(COMPILE) -c $< -o $@

.S.o:
	$(COMPILE) -x assembler-with-cpp -c $< -o $@
# "-x assembler-with-cpp" should not be necessary since this is the default
# file type for the .S (with capital S) extension. However, upper case
# characters are not always preserved on Windows. To ensure WinAVR
# compatibility define the file type manually.

.c.s:
	$(COMPILE) -S $< -o $@

flash:	all
	$(AVRDUDE) -U flash:w:main.hex:i

fuse:
	$(AVRDUDE) $(FUSES)

install: flash fuse

# if you use a bootloader, change the command below appropriately:
load: all
	bootloadHID main.hex

clean:
	rm -f main.hex main.elf $(OBJECTS) $(LIBOBJECTS)

# file targets:
main.elf: $(OBJECTS) $(LIBOBJECTS)
	$(COMPILE) -o main.elf $(OBJECTS) $(LIBOBJECTS)

main.hex: main.elf
	rm -f main.hex
	avr-objcopy -j .text -j .data -O ihex main.elf main.hex
# If you have an EEPROM section, you must also create a hex file for the
# EEPROM and add it to the "flash" target.

libmsr.a: $(LIBOBJECTS)
	-rm -f $@
	$(AR) $(ARFLAGS) $@ $(LIBOBJECTS)
	$(RANLIB) $@

# Targets for code debugging and analysis:
disasm:	main.elf
	avr-objdump -d main.elf

cpp:
	$(COMPILE) -E main.c

