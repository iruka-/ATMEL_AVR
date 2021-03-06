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

void main_task_tone(void);

void task_regist(void)
{
	init_tsk();
	reg_tsk(ID_main ,main_task_tone);	// 鍵盤.
	reg_tsk(ID_sound,sound_task);
}

/**********************************************************************
 *	メイン
 **********************************************************************
 */
int	main(void)
{
	kbd_init();
	sound_init();
						// 以下、12MHz時の周期:
//	timer_init(480-1);	// 40μS(25kHz)周期で割り込み.
//	timer_init(360-1);	// 30μS(33.333kHz)周期で割り込み.
//	timer_init(240-1);	// 20μS(50kHz)周期で割り込み.
	timer_init(256-1);	// 21.3333μS(46.875kHz)周期?で割り込み.

	sei();

	sound_stop_all();
	task_regist();

	while(1) {
		timer_wait();
		dispatch_tsk();
	}
	return 0;
}

/**********************************************************************
 *	
 **********************************************************************
 */
