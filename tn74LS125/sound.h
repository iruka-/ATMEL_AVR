/**********************************************************************
 *	PCM Sound Driver
 **********************************************************************
 */
#ifndef _sound_h_
#define _sound_h_


void sound_init(void);
void sound_beep(uchar pitch);
void sound_task(void);
void sound_play(uchar ch,uint p,uchar v);
void sound_decay();
void sound_stop_all();

#endif
/**********************************************************************
 *	
 **********************************************************************
 */
