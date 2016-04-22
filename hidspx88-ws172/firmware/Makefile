# Name: Makefile
# Project: HIDaspx
# Author: iruka & senshu
# Creation Date: 2008-09-02
# Tabsize: 4
# Copyright: (c) 2006 by OBJECTIVE DEVELOPMENT Software GmbH
# License: GNU GPL v2 (see License.txt) or proprietary (CommercialLicense.txt)
# This Revision: $Id: Makefile 566 2008-04-26 14:21:47Z cs $

DEVICE = atmega88
#DEVICE = attiny2313
#F_CPU = 12000000
F_CPU = 16000000
FUSE_L  = 0xBF
FUSE_H  = 0xDB

#PORT = avrdoper
#PROGRAMMER = stk500v2
#AVRDUDE = avrdude -c $(PROGRAMMER) -P $(PORT) -p $(DEVICE)
PROGRAMMER = avrspx

CFLAGS = $(DEFINES) -Iusbdrv -I. -DDEBUG_LEVEL=0 -DF_CPU=$(F_CPU)

MAIN = main

 ifeq ($(DEVICE),atmega88)
OBJECTS = $(MAIN).o m88.o delay.o usbdrv/usbdrv.o usbdrv/usbdrvasm.o 
 else
OBJECTS = $(MAIN).o crt.o delay.o usbdrv/usbdrv.o usbdrv/usbdrvasm.o 
 endif

COMPILE = avr-gcc -Wall -Os $(CFLAGS) -mmcu=$(DEVICE)

############################## ATtiny2313 ##############################
#Low: 10111111 (0xBF)
#     ||||++++-- CKSEL[3:0] システムクロック選択
#     ||++-- SUT[1:0] 起動時間
#     |+-- CKOUT (0:PD2にシステムクロックを出力)
#     +-- CKDIV8 クロック分周初期値 (1:1/1, 0:1/8)
#
#High:11-11011 (0xDB)
#     |||||||+-- RSTDISBL (RESETピン 1:有効, 0:無効(PA2))
#     ||||+++-- BODLEVEL[2:0] (111:Off, 110:1.8, 101:2.7, 100:4.3)
#     |||+-- WDTON (WDT 0:常時ON, 1:通常)
#     ||+-- SPIEN (1:ISP禁止, 0:ISP許可) ※Parallel時のみ
#     |+-- EESAVE (消去でEEPROMを 1:消去, 0:保持)
#     +-- DWEN (On-Chipデバッグ 1:無効, 0:有効)
#
#Ext: -------1 (0xFF)
#            +-- SPMEN (SPM命令 1:無効, 0:有効)
#
default: $(MAIN).hex

# symbolic targets:
help:
	@echo "This Makefile has no default rule. Use one of the following:"
	@echo "make hex ......... to build main.hex for HIDaspx hardware"
	@echo "make program ..... to flash fuses and firmware"
	@echo "make fuse ........ to flash the fuses"
	@echo "make flash ....... to flash the firmware"
	@echo "make clean ....... to delete objects and hex file"

hex: $(MAIN).hex
program: flash fuse

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

flash:
#	$(AVRDUDE) -U flash:w:$(MAIN).hex:i
	$(PROGRAMMER) -d1 $(MAIN).hex

usbaspload:
	$(MAKE) flash PROGRAMMER=usbasp DEVICE=atmega168

fuse:
#	@[ "$(FUSE_H)" != "" -a "$(FUSE_L)" != "" ] || \
#		{ echo "*** Edit Makefile and choose values for FUSE_L and FUSE_H!"; exit 1; }
#	$(AVRDUDE) -U hfuse:w:$(FUSE_H):m -U lfuse:w:$(FUSE_L):m
	$(PROGRAMMER) -d10 -FL$(FUSE_L) -FH$(FUSE_H)

clean:
	rm -f $(MAIN).dis $(MAIN).hex $(MAIN).lst $(MAIN).obj $(MAIN).cof $(MAIN).list $(MAIN).map $(MAIN).eep.hex $(MAIN).elf *.o usbdrv/*.o $(MAIN).s usbdrv/oddebug.s usbdrv/usbdrv.s time*.dat

install:
	cp -p $(MAIN).hex ../bin/firmware

# file targets:
$(MAIN).elf:	$(OBJECTS)
	$(COMPILE) -o $(MAIN).elf $(OBJECTS) -nostdlib
#	$(COMPILE) -o $(MAIN).elf $(OBJECTS)

$(MAIN).hex:	$(MAIN).elf
	rm -f $(MAIN).hex $(MAIN).eep.hex
	avr-objcopy -j .text -j .data -O ihex $(MAIN).elf $(MAIN).hex
	avr-size --mcu=$(DEVICE) $(MAIN).elf
#	sh checksize $(MAIN).elf 2048 128
#	checksize $(MAIN).elf 8192 1024
# do the checksize script as our last action to allow successful compilation
# on Windows with WinAVR where the Unix commands will fail.

disasm:	$(MAIN).elf
	avr-objdump -d $(MAIN).elf

cpp:
	$(COMPILE) -E $(MAIN).c

main.o : usi_mega88.h usi_tn2313.h

### EOF ###
