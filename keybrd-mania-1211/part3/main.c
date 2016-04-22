/**********************************************************************
 *	PS2 Keyboard Library : Sample Main
 **********************************************************************
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "config.h"
#include "task.h"
#include "taskdef.h"
#include "timer.h"
#include "sound.h"
#include "ps2keyb.h"
#include "morse.h"

/**********************************************************************
 *	define
 **********************************************************************
 */
#define	ESCAPE	0x1b		// [ESC]
#define	F1		0xf1		// [F1]

void quit();				// bootloaderに戻る.

static uchar main_mode = 0;	// 0:tone 1:morse
static uchar break_flag;
void main_task_tone(void);

/**********************************************************************
 *	message function
 **********************************************************************
 */
void main_break(uchar mode)
{
	main_mode = mode;
	break_flag = 1;
}
/**********************************************************************
 *	task
 **********************************************************************
 */
void main_task(void)
{
	if(morse_stat()==0) {			// 打鍵タスクが暇.
		uchar c = kbd_getchar();	// PS2 keyboardから ASCIIコードを取得する.
		if( c ) {					// 何かキー入力があった.
			if( c == ESCAPE) {			// [ESC]が押された.
				quit();						// bootloader に戻す.
			}else if( c == F1) {		// [F1]が押された.
				main_break(0);				// 鍵盤処理に戻す.
			}else{
				morse_send(c);		// モールス符号を発信する.(c=ASCII CODE)
			}
		}
	}
	sleep_tsk(20);		//20tick 待つ.
}


void task_regist(void)
{
	init_tsk();

	if(main_mode) {
		// モールス.
		reg_tsk(ID_main ,main_task);
		reg_tsk(ID_sound,sound_task);
		reg_tsk(ID_sub  ,morse_task);
	}else{
		// 鍵盤.
		reg_tsk(ID_main ,main_task_tone);
		reg_tsk(ID_sound,sound_task);
	}
}
/**********************************************************************
 *	メイン
 **********************************************************************
 */
int	main(void)
{
	kbd_init();
	sound_init();
	morse_init();
						// 以下、20MHz時の周期:
//	timer_init(400-1);	// 20  μS(50    kHz)周期で割り込み.
	timer_init(256-1);	// 12.8μS(78.125kHz)周期で割り込み.
	sei();

	while(1) {
		sound_stop_all();
		task_regist();

		break_flag=0;
		do {
			timer_wait();
			dispatch_tsk();
		}while(break_flag==0);
  	}

	return 0;
}

/**********************************************************************
 *	
 **********************************************************************
 */
