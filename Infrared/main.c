/**********************************************************************
 *	InfraLED Remocon
 **********************************************************************
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "config.h"
#include "timer.h"
#include "usart.h"

/**********************************************************************
 *	define
 **********************************************************************
 */
void USART_Init(int baud);
void keybd_init();

#if	0
void task_regist(void)
{
	init_tsk();
//	reg_tsk(ID_main ,main_task_tone);	// 鍵盤.
//	reg_tsk(ID_sound,sound_task);
}
#endif
#define	CPUCLK	4000000		//3579545
/**********************************************************************
 *	メイン
 **********************************************************************
 */
int	main(void)
{
	//	PortA=LED
	PORTA=0b00000010;
	DDRA =0b00000011;	// PA0は赤外LED正論理出力 / PA1 はチェック用LED負論理出力.
	//	PortB=weak pullup(in)
	PORTB=0b11111111;	// 全部Pull-UP
	DDRB =0b00000000;
	//	PortD=pull-down(sel only)
	PORTD=0b10000011;	// PD0,PD1,PD7はPull-UP
	DDRD =0b00000000;

	//電力節約.
	ACSR=0x80;	//	アナログ比較器の電力をOff
	

	keybd_init();
	timer_init(CPUCLK/8/20 - 1); // 1/20秒 (20Hz) 周期で割り込み.
//	USART_Init(9600);

	sei();
//	task_regist();

	while(1) {
//		MCUCR = 0b00000000;	// Idle!
		MCUCR = 0b00100000;	// standby!
		asm("sleep");

//		timer_wait();
//		dispatch_tsk();

	}
	return 0;
}

/**********************************************************************
 *	
 **********************************************************************
 */
