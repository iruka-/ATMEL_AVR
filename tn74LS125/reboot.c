/**********************************************************************
 *	bootloadHID を使用する場合の再起動ルーチン
 **********************************************************************
 *	void quit()  : アプリケーションを終了して、ブートローダーに戻る.
 *		ATmega88 に限定.
 *		bootloadHIDから起動した場合に限定.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <string.h>


#ifndef	__AVR_ATtiny2313__

static	void (*usrfunc)();

void jump()
{
	usrfunc();
}

#endif

/**********************************************************************
 *	
 **********************************************************************
 */
void quit(void)
{
#ifndef	__AVR_ATtiny2313__

	PCICR  = 0;	// PCINT All Disable.
	PCMSK0 = 0;	// PCINT0  Mask off.
	PCMSK1 = 0;	// PCINT8  Mask off.
	PCMSK2 = 0;	// PCINT16 Mask off.

	TIMSK1 = 0;	// TOIE1=0 (Overflow Interrupt)

	cli();

	// ATmega88以外は bootloaderのアドレスを調整する必要があります.

	usrfunc	= (void*) 0xc00;	// 0x1800 / 2
	jump();
#endif
}

/**********************************************************************
 *	
 **********************************************************************
 */
