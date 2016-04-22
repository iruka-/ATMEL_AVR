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

volatile unsigned int   timer_count;			//起動してからの時間.(tick)
		 unsigned int   adj_count;			//調整値

char	trigger_flag;				//アラーム時間が来たら1.
char	cmd_tick = 0;				//毎秒時刻printする.

#define	TEST			0			//1にすると毎分０秒でPOWER_ONする.
#define	ONLY_WEEKDAY	1			//土日はPOWER_ONしない.

//volatile uchar timer_tick;			//タイマー割り込み発生フラグ.


Time cur;
Time alarm;
uchar sec10;	// 0..9

/**********************************************************************
 *	初期化.
 **********************************************************************
 */
void timer_init(int cnt)
{
	cur.week=1;
	cur.hour=9;
	cur.minute=0;
	cur.sec=0;

	alarm.hour=0;
	alarm.minute=45;
	alarm.sec=61;			// 設定するまで無効にする.

	timer_count = 0;
	adj_count = 10000;		// 割合 1/10000 だけ遅れるようにする.
//	adj_count = 0xffff;		// 1/65535 だけ遅らせる.

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

#if	0
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
int timer_gettime(void)
{
	return timer_count;
}
#endif
/**********************************************************************
 *	timer : 現在時刻を更新する.
 **********************************************************************
 */
void add_cur_time()
{
	// LEDを短く点灯させる.
#if	1
	if((cur.msec == 0)&&(sec10==0)) {
			PORTB &= ~0x08;
	}else{
		PORTB |=  0x08;
	}
#endif

	cur.msec++;		// 0..9
	if(	cur.msec < 100) return;
	cur.msec = 0;



	if(	cmd_tick ) {
		USART_Tick();
	}

#if	1
	sec10++;
	if(	sec10 >= 10) {sec10=0;}
#endif

	cur.sec++;
	if(	cur.sec < 60) return;

	cur.sec = 0;
	sec10 = 0;
	cur.minute++;
	if(	cur.minute < 60) return;

	cur.minute = 0;
	cur.hour++;
	if(	cur.hour < 24) return;

	cur.hour = 0;
	cur.week++;
	if(	cur.week < 7) return;

	cur.week = 0;
}

/**********************************************************************
 *	timer : アラーム時刻と合致した場合の処理.
 **********************************************************************
 */
void trigger_alarm()
{
	PORTB |= 0x04;
	trigger_flag=1;
}

/**********************************************************************
 *	timer : アラーム時刻と合致しない場合の処理.
 **********************************************************************
 */
void trigger_alarm_stop()
{
	if(	trigger_flag ) {
		trigger_flag=0;
		PORTB &= ~0x04;
	}
}
/**********************************************************************
 *	timer : アラーム時刻と比較する.
 **********************************************************************
 */
int compare_alarm()
{
#if	TEST
	if(cur.sec    != alarm.sec)    return 0;
	return 1;
#else
	if(cur.hour   != alarm.hour)   return 0;
	if(cur.minute != alarm.minute) return 0;
	if(cur.sec    != alarm.sec)    return 0;
//	if(cur.msec   != alarm.msec)   return 0;

#if	ONLY_WEEKDAY				//土日はPOWER_ONしない.
	if(cur.week == 0) return 0;	// Sun
	if(cur.week == 6) return 0;	// Sat
#endif

	return 1;
#endif
}

#if	0
/**********************************************************************
 *	timer : 時刻の進み方を補正する.( 1 / adj_count だけ遅らせる.)
 **********************************************************************
 */
char add_adj_count()
{
	timer_count++;
	if(	timer_count >= adj_count) {
		timer_count=0;
		return 1;
	}
	return 0;
}
#endif
/**********************************************************************
 *	timer : 割り込みハンドラー.
 **********************************************************************
 */
ISR(TIMER1_OVF_vect)
{
	char adj=0,hit;

//	adj = add_adj_count();			//時間補正処理.
	timer_count++;
	if(	timer_count >= adj_count) {
		timer_count=0;
		adj=1;
	}

	if(adj==0) {
		add_cur_time();				// 時計に 1/100 秒を加算.
		hit = compare_alarm();		// アラーム時刻との比較.
		if(hit) {
			trigger_alarm();		//  アラーム鳴らす.
		}else{
			trigger_alarm_stop();	//  アラーム止める.
		}
	}

	USART_cmdproc();
}


#if	0
ISR(TIMER1_OVF_vect)
{
	timer_count ++;
	if(	timer_count==0) {
		timer_count_high++;
	}
	timer_tick ++;

	led_tick++;
	if(	led_tick>=100) {
		led_tick=0;
	}
}
#endif
/**********************************************************************
 *	
 **********************************************************************
 */
