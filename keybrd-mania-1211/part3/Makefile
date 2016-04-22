# Name: Makefile
# Project: bootloadHID
# Author: Christian Starkjohann
# Creation Date: 2007-03-19
# Tabsize: 4
# Copyright: (c) 2007 by OBJECTIVE DEVELOPMENT Software GmbH
# License: GNU GPL v2 (see License.txt)
# This Revision: $Id: Makefile 684 2008-10-22 18:43:39Z cs $

###############################################################################
# Configure the following variables according to your AVR. The example below
# is for an ATMega8. Program the device with
#     make fuse    # to set the clock generator, boot section size etc.
#     make flash   # to load the boot loader into flash
#     make lock    # to protect the boot loader from overwriting

#MCU = tn2313
MCU = m88
#MCU = m168
# {default=m88}

F_CPU = 20000000
#F_CPU = 12000000

OSC = 20

ifeq ($(F_CPU),12000000)
 OSC = 12
endif

ifeq ($(F_CPU),12800000)
 OSC = RC
endif

BOOTLOADER_ADDRESS =   0

# GNU make only
ifeq ($(MCU),m168)
 TARGET = main168-$(OSC)
 DEVICE = atmega168
else
ifeq ($(MCU),tn2313)
 TARGET = main2313-$(OSC)
 DEVICE = attiny2313
else
 TARGET = main88-$(OSC)
 DEVICE = atmega88
endif
endif

FUSEL = 0xFF
FUSEH = 0xDD
FUSEX = 0xF8

# Fuse Low byte: (0xFF)
#Low: 11111111
#     ||||++++-- CKSEL[3:0] システムクロック選択
#     ||++-- SUT[1:0] 起動時間
#     |+-- CKOUT (0:PB0にシステムクロックを出力)
#     +-- CKDIV8 クロック分周初期値 (1:1/1, 0:1/8)
#
# Fuse High byte: (0xDD)
#High:11-11101 
#     |||||+++-- BODLEVEL[2:0] (111:無, 110:1.8V, 101:2.7V, 100:4.3V)
#     ||||+-- EESAVE (消去でEEPROMを 1:消去, 0:保持)
#     |||+-- WDTON (1:WDT通常動作, 0:WDT常時ON)
#     ||+-- SPIEN (1:ISP禁止, 0:ISP許可) ※Parallel時のみ
#     |+-- DWEN (On-Chipデバッグ 1:無効, 0:有効)
#     +-- RSTDISBL (RESETピン 1:有効, 0:無効(PC6))
#
# Fuse Extend byte: (0xF8)
#Ext: -----000
#          ||+-- BOOTRST ※データシート参照
#          ++-- BOOTSZ[1:0] ※データシート参照
#
###############################################################################

#PROGRAMMER = avrdude -c stk500v2 -P avrdoper -p $(DEVICE)
PROGRAMMER = hidspx -d1

#LDFLAGS += -Wl,--section-start=.text=$(BOOTLOADER_ADDRESS)
#LDFLAGS += -Wl,--section-start=.data=$(BOOTLOADER_WORKRAM),--section-start=.text=$(BOOTLOADER_ADDRESS)
#LDFLAGS += -lc -nostdlib 

COMPILE = avr-gcc -Wall -Os -Iusbdrv -I. -mmcu=$(DEVICE) -DF_CPU=$(F_CPU) -DDEBUG_LEVEL=0 # -DTEST_MODE
# NEVER compile the final product with debugging! Any debug output will
# distort timing so that the specs can't be met.

OBJS = 	main.o    \
		tonemain.o \
		reboot.o  \
		timer.o	  \
		morse.o	  \
		task.o	  \
		sound.o	  \
	   	ps2keyb.o \



# symbolic targets:
all:	$(TARGET).hex

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
#	$(PROGRAMMER) -U flash:w:$(TARGET).hex:i
	$(PROGRAMMER) $(TARGET).hex

readflash:
#	$(PROGRAMMER) -U flash:r:read.hex:i
	@$(PROGRAMMER) -rp > read.hex

fuse:
#	$(PROGRAMMER) -U hfuse:w:$(FUSEH):m -U lfuse:w:$(FUSEL):m
	@$(PROGRAMMER) -d10 -fH$(FUSEH) -fL$(FUSEL) -FX$(FUSEX)

lock:
#	$(PROGRAMMER) -U lock:w:0x2f:m
	@$(PROGRAMMER) -l0x2f

readfuse:
#	$(UISP) --rd_fuses
	$(PROGRAMMER) -d10 -rf

clean:
	rm -f $(TARGET).hex main.bin *.o 

# usbdrv/*.o main.s usbdrv/oddebug.s usbdrv/usbdrv.s

install:
	cp -p $(TARGET).hex ../bin/firmware

help:
	@echo make [MCU=m168]             ... Build HEX file
	@echo make [MCU=m168] readfuse    ... Read FUSE info.
	@echo make [MCU=m168] readflash   ... Read  flash 
	@echo make [MCU=m168] flash       ... Write flash 
	@echo make [MCU=m168] fuse        ... Write fuse
	@echo make [MCU=m168] lock        ... Write LOCKBIT
	@echo make [MCU=m168] clean       ... Clean project
	@echo make [MCU=m168] install     ... install HEX file

########
# file targets:
main.bin:	$(OBJS)
	$(COMPILE) -o main.bin $(OBJS) $(LDFLAGS)

$(TARGET).hex:	main.bin
	rm -f $(TARGET).hex $(TARGET).eep
	avr-objcopy -j .text -j .data -O ihex main.bin $(TARGET).hex
	avr-size main.bin

disasm:	main.bin
	avr-objdump -d main.bin

cpp:
	$(COMPILE) -E main.c

###
# main.o: bootloaderconfig.h usbdrv/usbdrv.c usbdrv/usbportability.h \
#	usbdrv/usbdrv.h usbdrv/oddebug.h monit.h hidcmd.h osctune.h
#
