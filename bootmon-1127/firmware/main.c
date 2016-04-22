/* Name: main.c
 * Project:	AVR	bootloader HID
 * Author: Christian Starkjohann
 * Creation	Date: 2007-03-19
 * Tabsize:	4
 * Copyright: (c) 2007 by OBJECTIVE	DEVELOPMENT	Software GmbH
 * License:	GNU	GPL	v2 (see	License.txt)
 * This	Revision: $Id: main.c 684 2008-10-22 18:43:39Z cs $

 *	TABSIZE=4
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <avr/boot.h>
#include <string.h>
#include <util/delay.h>

static void	leaveBootloader() __attribute__((__noreturn__));
int  main(void) __attribute__((__noreturn__));

#if	F_CPU == 12800000	/* RC OSC mode */
char lastTimer0Value;
#endif

#include "bootloaderconfig.h"
#include "usbdrv/usbdrv.c"

#define OPTIMIZE_SIZE		1	// r4,r5,r6,r7,r8を global変数として使用する.
								// usbdrv側で同レジスタが使用されていないことが条件.

#if	1	//INCLUDE_WATCHDOG
#define	WDT_reset()	wdt_reset()
#else
#define	WDT_reset()	/* nothing */
#endif

/* ------------------------------------------------------------------------	*/

#ifndef	ulong
#	define ulong	unsigned long
#endif
#ifndef	uint
#	define uint		unsigned int
#endif

#if	(FLASHEND) > 0xffff	/* we need long	addressing */
#	define addr_t			ulong
#else
#	define addr_t			uint
#endif

#if OPTIMIZE_SIZE
register addr_t			currentAddress	asm("r2");	/* 2byte */
register uchar			factory_osccal	asm("r4");
register uchar			save_osccal		asm("r5");
register uchar 			*page_addr	 	asm("r6");	/* 2bytes */
register uchar			offset			asm("r8");	/* data	already	processed in current transfer */
#if	BOOTLOADER_CAN_EXIT
register uchar			exitMainloop	asm("r9");
#endif
#else
static addr_t			currentAddress;	/* in bytes	*/
static uchar			factory_osccal;
static uchar			save_osccal;
static uchar			offset;			/* data	already	processed in current transfer */
static uchar 			*page_addr;
#if	BOOTLOADER_CAN_EXIT
static uchar			exitMainloop;
#endif
#endif


static uchar			usbData[10];	// reportID + data[9]


//	REPORT_ID.
#define ID1    1
#define ID2    2
#define ID3    3
#define ID4    4

//	REPORT_COUNT+2の値.
#define	LENGTH1  8
#define	LENGTH2 132
#define	LENGTH3 40
#define	LENGTH4  6

/*
 「重要」
 usbHidReportDescriptor[] の内容を修正した時には、usbconfig.h の
 以下の行に、usbHidReportDescriptor[] の占めるサイズを正確に設定すること

 #define USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH    42
 #define USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH    51

*/
PROGMEM	char usbHidReportDescriptor[] = {
	0x06, 0x00,	0xff,			   // USAGE_PAGE (Generic Desktop)
	0x09, 0x01,					   // USAGE	(Vendor	Usage 1)
	0xa1, 0x01,					   // COLLECTION (Application)
	0x15, 0x00,					   //	LOGICAL_MINIMUM	(0)
	0x26, 0xff,	0x00,			   //	LOGICAL_MAXIMUM	(255)
	0x75, 0x08,					   //	REPORT_SIZE	(8)

	0x85, 0x01,					   //	REPORT_ID (1)
	0x95, 0x06,					   //	REPORT_COUNT (6)
	0x09, 0x00,					   //	USAGE (Undefined)
	0xb2, 0x02,	0x01,			   //	FEATURE	(Data,Var,Abs,Buf)

	0x85, 0x02,					   //	REPORT_ID (2)
	0x95, 0x83,					   //	REPORT_COUNT (131)
	0x09, 0x00,					   //	USAGE (Undefined)
	0xb2, 0x02,	0x01,			   //	FEATURE	(Data,Var,Abs,Buf)

    0x85, 0x03,                    //   REPORT_ID (3)
    0x95, 0x26,                    //   REPORT_COUNT (38)
    0x09, 0x00,                    //   USAGE (Undefined)
    0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)

#if INCLUDE_POLL_CMD
    0x85, 0x04,                    //   REPORT_ID (4)
    0x95, 0x04,                    //   REPORT_COUNT (4)
    0x09, 0x00,                    //   USAGE (Undefined)
    0xb2, 0x02, 0x01,              //   FEATURE (Data,Var,Abs,Buf)
#endif

	0xc0						   // END_COLLECTION
};


#include "monit.h"


/* allow compatibility with	avrusbboot's bootloaderconfig.h: */
#ifdef BOOTLOADER_INIT
#	define bootLoaderInit()			BOOTLOADER_INIT
#endif
#ifdef BOOTLOADER_CONDITION
#	define bootLoaderCondition()	BOOTLOADER_CONDITION
#endif

/* compatibility with ATMega88 and other new devices: */
#ifndef	TCCR0
#define	TCCR0	TCCR0B
#endif
#ifndef	GICR
#define	GICR	MCUCR
#endif


#if	F_CPU == 12800000
#define cli_save_cal() cli()
static void cli_save_cal_()	/* 要検討 */
{
	save_osccal = OSCCAL;
//	OSCCAL = factory_osccal;
	cli();
}

static inline void restore_cal()
{
	//OSCCAL = save_osccal;
}
#else
#define cli_save_cal() cli()
#define restore_cal()
#endif

static void	leaveBootloader()
{
	cli();
	boot_rww_enable();
	USB_INTR_ENABLE	= 0;
	USB_INTR_CFG = 0;		/* also	reset config bits */
#if	F_CPU == 12800000
	TCCR0 =	0;				/* default value */
#endif
	GICR = (1 << IVCE);		/* enable change of	interrupt vectors */
	GICR = (0 << IVSEL);	/* move	interrupts to application flash	section	*/
/* We must go through a	global function	pointer	variable instead of	writing
 *	((void (*)(void))0)();
 * because the compiler	optimizes a	constant 0 to "rcall 0"	which is not
 * handled correctly by	the	assembler.
 */

//static void	(*nullVector)(void)	__attribute__((__noreturn__));
//	nullVector();

	asm("clr r30");
	asm("clr r31");
	asm("icall");
}

uchar	usbFunctionSetup(uchar data[8])
{
	usbRequest_t	 *rq = (void *)data;
	uchar report_id = rq->wValue.bytes[0];
	if(rq->bRequest	== USBRQ_HID_SET_REPORT){
		offset = 0;
		return USB_NO_MSG;
	}else if(rq->bRequest	== USBRQ_HID_GET_REPORT){
		usbData[0] = report_id;    /* store report ID */
		usbMsgPtr =	usbData;
#if	INCLUDE_POLL_CMD 	// ReportID:4  POLLING PORTを実装する.
		if(	report_id == ID4) {
			usbData[1] = *page_addr;
		}
#endif
		return rq->wLength.word;		//return 7;
	}
	return 0;
}


uchar usbFunctionWrite(uchar *data,	uchar len)
{
	uchar	isLast;

//	address.l =	currentAddress;
	if(offset == 0){
#if	INCLUDE_MONITOR_CMD
		if(	data[0] == 1) {
			monit_main(data);
			return 1;		// (7byte) this is Last packet.
		}
#endif

		currentAddress = data[1] |(data[2]<<8);

#if	(FLASHEND) > 0xffff	/* we need long	addressing */

#error  address range over

		address.c[0] = data[1];
		address.c[1] = data[2];
		address.c[2] = data[3];
		address.c[3] = 0;
#endif

		data +=	4;
		len	-= 4;
	}
	offset += len;
	isLast = offset	& 0x80;	/* != 0	if last	block received */
	do{
		addr_t prevAddr = currentAddress;	//address.l;
		if((prevAddr & (SPM_PAGESIZE - 1)) == 0){	/* if page start: erase	*/
#ifndef	TEST_MODE
			cli_save_cal();
			boot_page_erase(prevAddr);		/* erase page */
			sei();
			boot_spm_busy_wait();			/* wait	until page is erased */
			restore_cal();
#endif
		}
		cli_save_cal();
		boot_page_fill(prevAddr, *(short *)data);
		sei();
		restore_cal();

		currentAddress +=2 ;	//address.l += 2;
		data +=	2;
		/* write page when we cross	page boundary */
#if 0
		if((address.s[0] & (SPM_PAGESIZE - 1)) == 0)
#else
		if((currentAddress & (SPM_PAGESIZE - 1)) == 0)
#endif
		{
#ifndef	TEST_MODE
			cli_save_cal();
			boot_page_write(prevAddr);
			sei();
			boot_spm_busy_wait();
			restore_cal();
#endif
		}
		len	-= 2;
	}while(len);
	return isLast;
}

static void	initForUsbConnectivity(void)
{
uchar	i =	0;

#if	F_CPU == 12800000
	TCCR0 =	3;			/* 1/64	prescaler */
#endif
	usbInit();
	/* enforce USB re-enumerate: */
	usbDeviceDisconnect();	/* do this while interrupts	are	disabled */
	do{				/* fake	USB	disconnect for > 250 ms	*/
		WDT_reset();
		_delay_ms(1);
	}while(--i);
	usbDeviceConnect();
	sei();
}

int main(void)
{
#if	F_CPU == 12800000	/* RC OSC mode */
	factory_osccal = OSCCAL;
//	OSCCAL = 240;		/* 自動補正されるが、収束を早める為、近似値をセット */
#endif
	/* initialize hardware */
	bootLoaderInit();
	cmd_caps();
	odDebugInit();
	exitMainloop=0;
	/* jump	to application if jumper is	set	*/
	if(bootLoaderCondition()){
#ifndef	TEST_MODE
		GICR = (1 << IVCE);	 /*	enable change of interrupt vectors */
		GICR = (1 << IVSEL); /*	move interrupts	to boot	flash section */
#endif
		initForUsbConnectivity();
		do{	/* main	event loop */
			WDT_reset();
			usbPoll();
#if	BOOTLOADER_CAN_EXIT
			if(exitMainloop){
#if	F_CPU != 12800000	/* memory is tight at 12.8 MHz,	save luxury	stuff */
				static uint	i;
				if(--i == 0)	/* delay 65k iterations	to allow for USB reply to exit command */
#endif
					break;
			}
#endif
		}while(bootLoaderCondition());
	}
	leaveBootloader();			// := __attribute__((__noreturn__));
	while(1) {
#if	0
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
#else
	 /* nothing */
#endif
	 }	// omit return process
}

