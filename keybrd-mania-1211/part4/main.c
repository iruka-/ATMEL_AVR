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
	reg_tsk(ID_main ,main_task_tone);	// Œ®”Õ.
	reg_tsk(ID_sound,sound_task);
}

/**********************************************************************
 *	ƒƒCƒ“
 **********************************************************************
 */
int	main(void)
{
	kbd_init();
	sound_init();
						// ˆÈ‰ºA12MHz‚ÌüŠú:
//	timer_init(480-1);	// 40ƒÊS(25kHz)üŠú‚ÅŠ„‚è‚İ.
//	timer_init(360-1);	// 30ƒÊS(33.333kHz)üŠú‚ÅŠ„‚è‚İ.
//	timer_init(240-1);	// 20ƒÊS(50kHz)üŠú‚ÅŠ„‚è‚İ.
	timer_init(256-1);	// 21.3333ƒÊS(46.875kHz)üŠú?‚ÅŠ„‚è‚İ.

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
