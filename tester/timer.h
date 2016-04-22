/**********************************************************************
 *	16bit TIMER1 を使用した割り込みハンドラー.
 **********************************************************************
 *	timer_init()
 *	timer_wait()
 */
#ifndef _timer_h_
#define _timer_h_

volatile char  time_hh;				//時刻
volatile char  time_mm;				//時刻
volatile char  time_ss;				//時刻
volatile int   time_tick;			//時刻


void  timer_init(int cnt);
uchar timer_wait(void);
uchar timer_gettick(void);
int   timer_gettime(void);

#endif  //_timer_h_
/**********************************************************************
 *	
 **********************************************************************
 */
