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

volatile int   timer_count;			//起動してからの時間.(tick)
volatile int   timer_count_high;	//起動してからの時間.(tick/65536)

volatile char  time_hh;				//時刻
volatile char  time_mm;				//時刻
volatile char  time_ss;				//時刻
volatile int   time_tick;			//時刻

volatile uchar timer_tick;			//タイマー割り込み発生フラグ.

#define	TIME_INTERVAL	2000		//2KHz

#define	TIMER_DA_OUT	0			//タイマー割り込みのタイミングでD/A出力を有効化.

#if		TIMER_DA_OUT				//タイマー割り込みのタイミングでD/A出力を有効化.
static	 uchar timer_daout;			//タイマー割り込みのタイミングでD/A出力.
#endif


void kbd_int_handler(void);

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

/**********************************************************************
 *	次に割り込むまで待つ.
 **********************************************************************
 */
uchar timer_wait(void)
{
	while(timer_tick == 0) { /* nothing */ };
	timer_tick --;
	return timer_tick;
}
/**********************************************************************
 *	
 **********************************************************************
 */
uchar timer_gettick(void)
{
	return timer_tick;
}
/**********************************************************************
 *
 **********************************************************************
 */
int timer_gettime(void)
{
	return timer_count;
}
/**********************************************************************
 *	timer : 割り込みハンドラー.
 **********************************************************************
 */
ISR(TIMER1_OVF_vect)
{
#if		TIMER_DA_OUT				//タイマー割り込みのタイミングでD/A出力を有効化.
	OCR0A = timer_daout;
#endif
	timer_count ++;
	if(	timer_count==0) {
		timer_count_high++;
	}
	timer_tick ++;

//	-------------
	time_tick++;
	if( time_tick >= TIME_INTERVAL ) {
		time_tick = 0;
		time_ss++;
		if( time_ss >= 60 ) {
			time_ss =0;
			time_mm++;
			if( time_mm >= 60 ) {
				time_mm =0;
				time_hh++;
				if( time_hh >= 24 ) {
					time_hh =0;
				}
			}
		}
	}

}
/**********************************************************************
 *	
 **********************************************************************
 */
void wait_usec(uchar n)
{
	do {
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");

		asm("nop");
		asm("nop");
		asm("nop");
	}while(--n);
}
/**********************************************************************
 *
 **********************************************************************
 */

#if		TIMER_DA_OUT				//タイマー割り込みのタイミングでD/A出力を有効化.
void sound_out(uchar x)
{
	timer_daout = x;
}
#endif
/**********************************************************************
 *	
 **********************************************************************
 */

