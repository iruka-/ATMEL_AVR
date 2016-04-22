/**********************************************************************
 *	16bit TIMER1 を使用した割り込みハンドラー.
 **********************************************************************
 *	timer_init()
 *	timer_wait()
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "config.h"
#include "task.h"
#include "timer.h"
#include "usart.h"

extern void	keybd_scan();

volatile unsigned int   timer_count;			//起動してからの時間.(tick)

#define	LED_PIN	0x02	// PORTA.1

/**********************************************************************
 *	初期化.
 **********************************************************************
 */
void timer_init(int cnt)
{
	timer_count = 0;

	TCCR1A = 0b00000011;	// Port出力はoff
	TCCR1B = 0b00011001;	// WGM=1111 (高速PWM) CS1=001 (CLK/1)

	OCR1AH = (cnt >> 8);
	OCR1AL =  cnt;

#ifdef	__AVR_ATtiny2313__
	TIMSK = 0b10000000;		// ATtiny2313 : TOIE1=1 (Overflow Interrupt)
#else
	TIMSK1 = 0b00000001;	// ATmega88   : TOIE1=1 (Overflow Interrupt)
#endif
}

void led_on()
{
	PORTA &= ~LED_PIN;
}

void led_off()
{
	PORTA |= LED_PIN;
}

void led_blink()
{
	timer_count++;
	if(timer_count & 0x8) {
		led_on();
	}else{
		led_off();
	}
}

/**********************************************************************
 *	timer : 割り込みハンドラー.
 **********************************************************************
 */
ISR(TIMER1_OVF_vect)
{
//	led_blink();
	keybd_scan();
}

/**********************************************************************
 *	
 **********************************************************************
 */
