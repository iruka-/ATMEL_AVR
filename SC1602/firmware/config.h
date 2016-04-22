/**********************************************************
 *	PIN ASSIGNMENT
 **********************************************************
 */
#ifndef	_config_h_
#define	_config_h_


//	PS2 KEYBOARD HANDLER SELECT =========
#define	PS2KBD_USE_INTERRUPT	0
//	1: 	hardware interrupt ( PCINT8 )
//	0:	software emulate   ( called from timer-interrupt )

//	PS2 KEYBOARD ENABLE GETCHAR FUNCTION =======
#define	PS2KBD_USE_GETCHAR		1

//	PS2 KEYBOARD ENABLE PRESSTABLE FUNCTION =======
#define	PS2KBD_USE_PRESSTABLE	1


//#define	SKEY_AT101	  			// English (International) 101/104 AT Layout keyboard


//	PS2 KEYBOARD PORT ASSIGN ============
#define PS2KBD_PIN    PINC
#define PS2KBD_DDR    DDRC
#define PS2KBD_PORT   PORTC


//	PS2 KEYBOARD BIT ASSIGN =============
#define PS2KBD_CLOCK  0
#define PS2KBD_DATA   1


//	SOUND OUTPUT BIT ASSIGN =============
#define	SPK_OUT_DDR	  DDRD
#define	SPK_OUT_PORT  PORTD
#define	SPK_OUT_PIN   PIND
#define	SPK_OUT_BIT	  6

//	TRANSMITTER OUTPUT BIT ASSIGN =============
#define	TX_OUT_DDR	  DDRD
#define	TX_OUT_PORT  PORTD
#define	TX_OUT_BIT	  7

//	MASK BIT ============================
#define PS2KBD_CLOCK_MASK  	(1<<PS2KBD_CLOCK)
#define PS2KBD_DATA_MASK   	(1<<PS2KBD_DATA)
#define	SPK_OUT_BIT_MASK  	(1<<SPK_OUT_BIT)
#define	TX_OUT_BIT_MASK  	(1<<TX_OUT_BIT)

#endif	//_config_h_
