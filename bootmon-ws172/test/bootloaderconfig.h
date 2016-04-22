/* Name: bootloaderconfig.h
 * Project: AVR bootloader HID
 * Author: Christian Starkjohann
 * Creation Date: 2007-03-19
 * Tabsize: 4
 * Copyright: (c) 2007 by OBJECTIVE DEVELOPMENT Software GmbH
 * License: GNU GPL v2 (see License.txt)
 * This Revision: $Id: bootloaderconfig.h 684 2008-10-22 18:43:39Z cs $
 */

#ifndef __bootloaderconfig_h_included__
#define __bootloaderconfig_h_included__

/*
General Description:
This file (together with some settings in Makefile) configures the boot loader
according to the hardware.

This file contains (besides the hardware configuration normally found in
usbconfig.h) two functions or macros: bootLoaderInit() and
bootLoaderCondition(). Whether you implement them as macros or as static
inline functions is up to you, decide based on code size and convenience.

bootLoaderInit() is called as one of the first actions after reset. It should
be a minimum initialization of the hardware so that the boot loader condition
can be read. This will usually consist of activating a pull-up resistor for an
external jumper which selects boot loader mode. You may call leaveBootloader()
from this function if you know that the main code should run.

bootLoaderCondition() is called immediately after initialization and in each
main loop iteration. If it returns TRUE, the boot loader will be active. If it
returns FALSE, the boot loader jumps to address 0 (the loaded application)
immediately.

For compatibility with Thomas Fischl's avrusbboot, we also support the macro
names BOOTLOADER_INIT and BOOTLOADER_CONDITION for this functionality. If
these macros are defined, the boot loader usees them.
*/

/* ---------------------------- Hardware Config ---------------------------- */
/*
                          ATmega88/168
                         ___    ___
    (PCINT14/_RESET)PC6 [1  |__| 28] PC5(ADC5/SCL/PCINT13)
       (PCINT16/RxD)PD0 [2       27] PC4(ADC4/SDA/PCINT12)
       (PCINT17/TxD)PD1 [3       26] PC3(ADC3/PCINT11)
      (PCINT18/INT0)PD2 [4       25] PC2(ADC2/PCINT10)
 (PCINT19/OC2B/INT1)PD3 [5       24] PC1(ADC1/PCINT9)
    (PCINT20/XCK/T0)PD4 [6       23] PC0(ADC0/PCINT8)
                    Vcc [7       22] GND
                    GND [8       21] AREF
(PCINT6/XTAL1/TOSC1)PB6 [9       20] AVcc
(PCINT7/XTAL2/TOSC2)PB7 [10      19] PB5(SCK/PCINT5)
   (PCINT21/OC0B/T1)PD5 [11      18] PB4(MISO/PCINT3)
 (PCINT22/OC0A/AIN0)PD6 [12      17] PB3(MOSI/OC2A/PCINT3)
      (PCINT23/AIN1)PD7 [13      16] PB2(_SS/OC1B/PCINT2)
  (PCINT0/CLKO/ICP1)PB0 [14      15] PB1(OC1A/PCINT1)
                         ~~~~~~~~~~
*/


// Select One.
#define	USB_CONFIG_IS_PORTD_3_2		0	// 
#define	USB_CONFIG_IS_PORTB_0_1		1	// wsn#172
//


#if	USB_CONFIG_IS_PORTD_3_2
/*
   ---------------------------------------
   USB:     PD3   ===> USB D-
            PD2   ===> USB D+
   XTAL:    XTAL1,2 => Crystal 12, 16, 20MHz
   --------------------------------------- 
 */
#define USB_CFG_IOPORTNAME      D
#define USB_CFG_DMINUS_BIT      3
#define USB_CFG_DPLUS_BIT       2
#endif


#if	USB_CONFIG_IS_PORTB_0_1
/*
   ---------------------------------------
   USB:     PB0     ===> USB D-
            PB1,PD2 ===> USB D+
   XTAL:    XTAL1,2 => Crystal 12, 16, 20MHz
   ---------------------------------------
 */
#define USB_CFG_IOPORTNAME      B
#define USB_CFG_DMINUS_BIT      0
#define USB_CFG_DPLUS_BIT       1
#endif


#define USB_CFG_CLOCK_KHZ       (F_CPU/1000)
/* Clock rate of the AVR in MHz. Legal values are 12000, 12800, 15000, 16000,
 * 16500 and 20000. The 12.8 MHz and 16.5 MHz versions of the code require no
 * crystal, they tolerate +/- 1% deviation from the nominal frequency. All
 * other rates require a precision of 2000 ppm and thus a crystal!
 * Default if not specified: 12 MHz
 */

/* ----------------------- Optional Hardware Config ------------------------ */

/* #define USB_CFG_PULLUP_IOPORTNAME   D */
/* If you connect the 1.5k pullup resistor from D- to a port pin instead of
 * V+, you can connect and disconnect the device from firmware by calling
 * the macros usbDeviceConnect() and usbDeviceDisconnect() (see usbdrv.h).
 * This constant defines the port on which the pullup resistor is connected.
 */
/* #define USB_CFG_PULLUP_BIT          4 */
/* This constant defines the bit number in USB_CFG_PULLUP_IOPORT (defined
 * above) where the 1.5k pullup resistor is connected. See description
 * above for details.
 */

/* --------------------------- Functional Range ---------------------------- */

#define BOOTLOADER_CAN_EXIT     1
/* If this macro is defined to 1, the boot loader command line utility can
 * initiate a reboot after uploading the FLASH when the "-r" command line
 * option is given. If you define it to 0 or leave it undefined, the "-r"
 * option won't work and you save a couple of bytes in the boot loader. This
 * may be of advantage if you compile with gcc 4 instead of gcc 3 because it
 * generates slightly larger code.
 * If you need to save even more memory, consider using your own vector table.
 * Since only the reset vector and INT0 (the first two vectors) are used,
 * this saves quite a bit of flash. See Alexander Neumann's boot loader for
 * an example: http://git.lochraster.org:2080/?p=fd0/usbload;a=tree
 */

/* ------------------------------------------------------------------------- */

/* Example configuration: Port D bit 3 is connected to a jumper which ties
 * this pin to GND if the boot loader is requested. Initialization allows
 * several clock cycles for the input voltage to stabilize before
 * bootLoaderCondition() samples the value.
 * We use a function for bootLoaderInit() for convenience and a macro for
 * bootLoaderCondition() for efficiency.
 */

#ifndef __ASSEMBLER__   /* assembler cannot parse function definitions */
#include <util/delay.h>

//#define	CONDITION_JUMPER	3
#define	CONDITION_JUMPER	5

static inline void  bootLoaderInit(void)
{
    PORTD = 1 << CONDITION_JUMPER; /* activate pull-up for key */
    _delay_us(10);  /* wait for levels to stabilize */
}

#if	0
#define bootLoaderCondition()   ((PIND & (1 << CONDITION_JUMPER)) == 0)   /* True if jumper is set */
#else
#define bootLoaderCondition()   ((PIND & (1 << CONDITION_JUMPER)))   /* True if jumper is not set */
#endif

#if F_CPU==12800000
#define	INCLUDE_MONITOR_CMD		0
#define INCLUDE_POLL_CMD		0
#define INCLUDE_EEPROM_RW		0
#else
#define	INCLUDE_MONITOR_CMD		1
#define INCLUDE_POLL_CMD		1
#define INCLUDE_EEPROM_RW		1
#endif

#endif

/* ------------------------------------------------------------------------- */

#endif /* __bootloader_h_included__ */
