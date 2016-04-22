/*
 * Project: AVR-Doper
 * Author: Christian Starkjohann <cs@obdev.at>
 * Creation Date: 2006-06-21
 * Tabsize: 4
 * Copyright: (c) 2006 by Christian Starkjohann, all rights reserved.
 * License: GNU GPL v2 (see License.txt) or proprietary (CommercialLicense.txt)
 * Revision: $Id: main.c 571 2008-04-27 14:39:33Z cs $
 */

#define	SEPARATE_STRING		(0)

/* configuration options: */
//#define USE_DCD_REPORTING       0
/* If this option is defined to 1, the driver will report carrier detect when
 * the serial device is opened. This is useful on some Unix platforms to allow
 * using the /dev/tty* instead of /dev/cu* devices.
 * Setting this option to 0 saves a couple of bytes in flash and RAM memory.
 */
#define HW_CDC_PACKET_SIZE      8
/* Size of bulk transfer packets. The standard demands 8 bytes, but we may
 * be better off with less. Try smaller values if the communication hangs.
 */

static PROGMEM char	deviceDescrCDC[] = {	/* USB device descriptor */
	18,			/* sizeof(usbDescriptorDevice):	length of descriptor in	bytes */
	USBDESCR_DEVICE,		/* descriptor type */
	0x10, 0x01,				/* USB version supported */
	0x02,					/* device class: CDC */
	0,						/* subclass	*/
	0,						/* protocol	*/
	8,						/* max packet size */
	USB_CFG_VENDOR_ID,		/* 2 bytes */
	0xe1, 0x05,				/* 2 bytes:	shared PID for CDC-ACM devices */
	USB_CFG_DEVICE_VERSION,	/* 2 bytes */
	1,						/* manufacturer	string index */
	2,						/* product string index	*/
	3,						/* serial number string	index */
	1,						/* number of configurations	*/
};

static PROGMEM char	configDescrCDC[] = {   /* USB configuration	descriptor */
	9,			/* sizeof(usbDescriptorConfiguration): length of descriptor	in bytes */
	USBDESCR_CONFIG,	/* descriptor type */
	67,	0,		/* total length	of data	returned (including	inlined	descriptors) */
	2,			/* number of interfaces	in this	configuration */
	1,			/* index of	this configuration */
	0,			/* configuration name string index */
#if	USB_CFG_IS_SELF_POWERED
	USBATTR_SELFPOWER,	/* attributes */
#else
#ifdef USBATTR_BUSPOWER
	USBATTR_BUSPOWER,	/* attributes */
#else
	0,	/* attributes */
#endif
#endif
	USB_CFG_MAX_BUS_POWER/2,			/* max USB current in 2mA units	*/

	/* interface descriptors follow	inline:	*/
	/* Interface Descriptor	for	CDC-ACM	Control	 */
	9,			/* sizeof(usbDescrInterface): length of	descriptor in bytes	*/
	USBDESCR_INTERFACE,	/* descriptor type */
	0,			/* index of	this interface */
	0,			/* alternate setting for this interface	*/
	1,			/* endpoints excl 0: number	of endpoint	descriptors	to follow */
	USB_CFG_INTERFACE_CLASS,	/* see usbconfig.h */
	USB_CFG_INTERFACE_SUBCLASS,
	USB_CFG_INTERFACE_PROTOCOL,
	0,			/* string index	for	interface */

	/* CDC Class-Specific descriptors */
	5,			/* sizeof(usbDescrCDC_HeaderFn): length	of descriptor in bytes */
	0x24,		/* descriptor type */
	0,			/* Subtype:	header functional descriptor */
	0x10, 0x01,	/* CDC spec	release	number in BCD */

	4,			/* sizeof(usbDescrCDC_AcmFn): length of	descriptor in bytes	*/
	0x24,		/* descriptor type */
	2,			/* Subtype:	abstract control management	functional descriptor */
	0x02,		/* capabilities: SET_LINE_CODING, GET_LINE_CODING, SET_CONTROL_LINE_STATE */

	5,			/* sizeof(usbDescrCDC_UnionFn):	length of descriptor in	bytes */
	0x24,		/* descriptor type */
	6,			/* Subtype:	union functional descriptor	*/
	0,			/* CDC_COMM_INTF_ID: master	interface (control)	*/
	1,			/* CDC_DATA_INTF_ID: slave interface (data)	*/

	5,			/* sizeof(usbDescrCDC_CallMgtFn): length of	descriptor in bytes	*/
	0x24,		/* descriptor type */
	1,			/* Subtype:	call management	functional descriptor */
	0x03,		/* capabilities: allows	management on data interface, handles call management by itself	*/
	1,			/* CDC_DATA_INTF_ID: interface used	for	call management	*/

	/* Endpoint	Descriptor */
	7,			/* sizeof(usbDescrEndpoint)	*/
	USBDESCR_ENDPOINT,	/* descriptor type = endpoint */
	0x83,		/* IN endpoint number 3	*/
	0x03,		/* attrib: Interrupt endpoint */
	8, 0,		/* maximum packet size */
	100,	// @@kuga 2,	//100,	/* in ms */

	/* Interface Descriptor	for	CDC-ACM	Data  */
	9,			/* sizeof(usbDescrInterface): length of	descriptor in bytes	*/
	USBDESCR_INTERFACE,	/* descriptor type */
	1,			/* index of	this interface */
	0,			/* alternate setting for this interface	*/
	2,			/* endpoints excl 0: number	of endpoint	descriptors	to follow */
	0x0a,		/* Data	Interface Class	Codes */
	0,			/* interface subclass */
	0,			/* Data	Interface Class	Protocol Codes */
	0,			/* string index	for	interface */

	/* Endpoint	Descriptor */
	7,			/* sizeof(usbDescrEndpoint)	*/
	USBDESCR_ENDPOINT,	/* descriptor type = endpoint */
	0x01,		/* OUT endpoint	number 1 */
	0x02,		/* attrib: Bulk	endpoint */
	HW_CDC_PACKET_SIZE,	0,	/* maximum packet size */
	0,			/* in ms */

	/* Endpoint	Descriptor */
	7,			/* sizeof(usbDescrEndpoint)	*/
	USBDESCR_ENDPOINT,	/* descriptor type = endpoint */
	0x81,		/* IN endpoint number 1	*/
	0x02,		/* attrib: Bulk	endpoint */
	HW_CDC_PACKET_SIZE,	0,	/* maximum packet size */
	0,			/* in ms */
};


/* ------------------------------------------------------------------------- */
PROGMEM char usbDescriptorString0[] = { /* language descriptor */
    4,          /* sizeof(usbDescriptorString0): length of descriptor in bytes */
    3,          /* descriptor type */
    0x09, 0x04, /* language index (0x0409 = US-English) */
};

/* ------------------------------------------------------------------------- */
//#define USB_STRING_DESCRIPTOR_HEADER(stringLength) ((2*(stringLength)+2) | (3<<8))
#define	VENDOR_NAME		'R','S','2','3','2'
#define	VENDOR_NAME_LEN		5

#if	SEPARATE_STRING
#define	DEVICE_NAME		'R','S','2','3','2'
#define	DEVICE_NAME_LEN		5


#define	SERIAL_NUMBER	'R','S','2','3','2'
#define	SERIAL_NUMBER_LEN	5
#endif
/* ------------------------------------------------------------------------- */
PROGMEM int  usbDescriptorStringVendor[] = {
    USB_STRING_DESCRIPTOR_HEADER(VENDOR_NAME_LEN),
    VENDOR_NAME
};
#if	SEPARATE_STRING
PROGMEM int  usbDescriptorStringDevice[] = {
    USB_STRING_DESCRIPTOR_HEADER(DEVICE_NAME_LEN),
    DEVICE_NAME
};

PROGMEM int usbDescriptorStringSerialNumber[] = {
    USB_STRING_DESCRIPTOR_HEADER(SERIAL_NUMBER_LEN),
    SERIAL_NUMBER
};
#endif
/* ------------------------------------------------------------------------- */

uchar usbFunctionDescriptor(usbRequest_t *rq)
{
	uchar *p = NULL, len = 0;
	if(rq->wValue.bytes[1] == USBDESCR_DEVICE){
		p =	(uchar *)deviceDescrCDC;
		len	= sizeof(deviceDescrCDC);
	}else if(rq->wValue.bytes[1] == USBDESCR_CONFIG){
		p =	(uchar *)configDescrCDC;
		len	= sizeof(configDescrCDC);
	}else{	/* must	be string descriptor */
        switch(rq->wValue.bytes[0]) {
        case (0)://GET_DESCRIPTOR(USB_CFG_DESCR_PROPS_STRING_0, usbDescriptorString0)
			p =	(uchar *)usbDescriptorString0;
			len	= sizeof(usbDescriptorString0);	break;
		default:
        case (1)://GET_DESCRIPTOR(USB_CFG_DESCR_PROPS_STRING_VENDOR, usbDescriptorStringVendor)
			p =	(uchar *)usbDescriptorStringVendor;
			len	= sizeof(usbDescriptorStringVendor);	break;
#if	SEPARATE_STRING
        case (2)://GET_DESCRIPTOR(USB_CFG_DESCR_PROPS_STRING_PRODUCT, usbDescriptorStringDevice)
			p =	(uchar *)usbDescriptorStringDevice;
			len	= sizeof(usbDescriptorStringDevice);	break;
        case (3)://GET_DESCRIPTOR(USB_CFG_DESCR_PROPS_STRING_SERIAL_NUMBER,DescriptorStringSerialNumb
			p =	(uchar *)usbDescriptorStringSerialNumber;
			len	= sizeof(usbDescriptorStringSerialNumber);	break;
#endif
		}
	}
	usbMsgPtr =	p;
	return len;
}

/* ------------------------------------------------------------------------- */
