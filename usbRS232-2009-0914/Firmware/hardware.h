/*
 * Name: hardware.h
 * Project: AVR-Doper
 * Author: Christian Starkjohann <cs@obdev.at>
 * Creation Date: 2006-07-05
 * Tabsize: 4
 * Copyright: (c) 2006 by Christian Starkjohann, all rights reserved.
 * License: GNU GPL v2 (see License.txt) or proprietary (CommercialLicense.txt)
 * Revision: $Id: hardware.h 566 2008-04-26 14:21:47Z cs $
 */

/*
General Description:
This module defines hardware properties and configuration choices.
*/

#ifndef __hardware_h_included__
#define __hardware_h_included__

/* ---------------------------- Hardware Config ---------------------------- */
//	Select one.
#define HWPIN_USB_IS_D_4_3		1	// USB_D- = PORTD.4  , USB_D+ = PORTD.3 (HIDaspx)
#define HWPIN_USB_IS_D_3_2		0	// USB_D- = PORTD.3  , USB_D+ = PORTD.2

//	Busy LED.
#define	HW_USE_LED_CONTROL		1	// BUSY LED CONTROL
/* This is the bit number in PORTB where BUSY LED ASSIGNED.
 */
#define	HW_LED_PIN				3	// PORTB.3
/* ---------------------------- Hardware Config ---------------------------- */






#if HWPIN_USB_IS_D_3_2
/* ---------------------------- Hardware Config ---------------------------- */
#define USB_CFG_IOPORTNAME      D	//PORT_NAME(HWPIN_USB_DMINUS)
/* This is the port where the USB bus is connected. When you configure it to
 * "B", the registers PORTB, PINB and DDRB will be used.
 */
#define USB_CFG_DMINUS_BIT      3	//PORT_BIT(HWPIN_USB_DMINUS)
/* This is the bit number in USB_CFG_IOPORT where the USB D- line is connected.
 * This may be any bit in the port.
 */
#define USB_CFG_DPLUS_BIT       2	//PORT_BIT(HWPIN_USB_DPLUS)
/* This is the bit number in USB_CFG_IOPORT where the USB D+ line is connected.
 * This may be any bit in the port. Please note that D+ must also be connected
 * to interrupt pin INT0!
 */
#define	USE_INTEERUPT_AS_INT1	0	// INT0 --> INT1
/* ---------------------------- Hardware Config ---------------------------- */
#endif



#if HWPIN_USB_IS_D_4_3
/* ---------------------------- Hardware Config ---------------------------- */
#define USB_CFG_IOPORTNAME      D	//PORT_NAME(HWPIN_USB_DMINUS)
/* This is the port where the USB bus is connected. When you configure it to
 * "B", the registers PORTB, PINB and DDRB will be used.
 */
#define USB_CFG_DMINUS_BIT      4	//PORT_BIT(HWPIN_USB_DMINUS)
/* This is the bit number in USB_CFG_IOPORT where the USB D- line is connected.
 * This may be any bit in the port.
 */
#define USB_CFG_DPLUS_BIT       3	//PORT_BIT(HWPIN_USB_DPLUS)
/* This is the bit number in USB_CFG_IOPORT where the USB D+ line is connected.
 * This may be any bit in the port. Please note that D+ must also be connected
 * to interrupt pin INT0!
 */
#define	USE_INTEERUPT_AS_INT1	1	// INT0 --> INT1
/* ---------------------------- Hardware Config ---------------------------- */
#endif


#ifndef F_CPU
#error "F_CPU must be defined in compiler command line!"
/* #define F_CPU                   12000000 */
/* Our CPU frequency.
 */
#endif

#endif /* __hardware_h_included__ */
