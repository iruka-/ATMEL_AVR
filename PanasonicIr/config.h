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
#define	PS2KBD_USE_GETCHAR		0

//	PS2 KEYBOARD ENABLE PRESSTABLE FUNCTION =======
#define	PS2KBD_USE_PRESSTABLE	0


//	PS2 KEYBOARD PORT ASSIGN ============
#define PS2KBD_PIN    PINB
#define PS2KBD_DDR    DDRB
#define PS2KBD_PORT   PORTB


//	PS2 KEYBOARD BIT ASSIGN =============
#define PS2KBD_CLOCK  0
#define PS2KBD_DATA   1


//	SOUND OUTPUT BIT ASSIGN =============
#define	SPK_OUT_DDR	  DDRB
#define	SPK_OUT_PORT  PORTB
#define	SPK_OUT_PIN   PINB
#define	SPK_OUT_BIT	  2

//	TRANSMITTER OUTPUT BIT ASSIGN =============
#define	TX_OUT_DDR	  DDRB
#define	TX_OUT_PORT  PORTB
#define	TX_OUT_BIT	  3

//	MASK BIT ============================
#define PS2KBD_CLOCK_MASK  	(1<<PS2KBD_CLOCK)
#define PS2KBD_DATA_MASK   	(1<<PS2KBD_DATA)
#define	SPK_OUT_BIT_MASK  	(1<<SPK_OUT_BIT)
#define	TX_OUT_BIT_MASK  	(1<<TX_OUT_BIT)

#endif	//_config_h_
