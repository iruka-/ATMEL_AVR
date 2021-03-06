/**********************************************************************
 *	Tiny Task Controler
 **********************************************************************
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "task.h"

/**********************************************************************
 *	
 **********************************************************************
 */
TCB   task_tcb[MAX_TASKS];	//実体.
uchar cur_tsk;				//現在のtask id.

/**********************************************************************
 *	
 **********************************************************************
 */
//	TCBの初期化.
void init_tsk(void)
{
	uchar i;
	for(i=0;i<MAX_TASKS;i++) {
		task_tcb[i].stat = 0;
	}
}

/**********************************************************************
 *	
 **********************************************************************
 */
//	ディスパッチ実行.
void dispatch_tsk(void)
{
	TCB *t = task_tcb;
	for(cur_tsk=0;cur_tsk<MAX_TASKS;cur_tsk++,t++) {
		if( t->stat ) {
			if( t->stat & TSTAT_SLEEP) {
				if( t->sleep_cnt ) {
					t->sleep_cnt--;
				}
				if( t->sleep_cnt != 0 ) {
					continue;
				}else{
					t->stat = TSTAT_RUNNABLE;
				}
			}
			(*t->func)();
		}
	}
}

/**********************************************************************
 *	
 **********************************************************************
 */
