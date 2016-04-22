/**********************************************************************
 *	モールス打鍵アルゴリズム.
 **********************************************************************
 *	uchar morse_stat()
 *	void morse_send(uchar c)
 *	void morse_task()
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "config.h"
#include "task.h"
#include "timer.h"
#include "morse.h"
#include "sound.h"


#include "morsetab.h"	//符号表.


//	長音、短音の長さ定数.
#define	MORSE_SPEED1	32
#define	MORSE_SPEED3	(MORSE_SPEED1*3+4)	//体感上、長音を少し長くする.


//	状態変数:
uchar morse_char;	// ASCII CODE
uchar morse_pat;	// pattern (MSBから morse_cnt ビット分が有効)
					// bitが0なら長音,1なら短音.

uchar morse_cnt;	// counter (1..5まで、0なら打鍵終了)
uchar morse_sub;	// sub count 0:mark 1:space 2:end of job

void  keyon(uchar c)
{
	if(c) {
		sound_play(0,0x280,0xff);
	}else{
		sound_play(0,0,0);
	}
}

/**********************************************************************
 *	
 **********************************************************************
 */
void morse_init(void)
{
	TX_OUT_PORT |= TX_OUT_BIT_MASK;	// PD7をH.
//	TX_OUT_DDR  |= TX_OUT_BIT_MASK;	// PD7をout.
}

/**********************************************************************
 *	鍵を押す.(CW 発振)
 **********************************************************************
 */
static	void morse_out_mark(void)
{
	TX_OUT_PORT &= ~TX_OUT_BIT_MASK;	// PD7をL.(電波on)
	TX_OUT_DDR  |= TX_OUT_BIT_MASK;		// PD7をout.
	keyon(1);							//サウンドルーチンをOn.
}
/**********************************************************************
 *	鍵を離す.(CW 停止)
 **********************************************************************
 */
static	void morse_out_space(void)
{
	TX_OUT_DDR  &= ~TX_OUT_BIT_MASK;	// PD7をin.
	TX_OUT_PORT |= TX_OUT_BIT_MASK;		// PD7をH.(pull up)
	keyon(0);							//サウンドルーチンをOff.
}

/**********************************************************************
 *	コマンド受領の可否を返す.
 **********************************************************************
 *	戻り値 :
 *		0  : コマンド受付可能.
 *	   !0  : 現在打鍵中のASCII 文字(1字)
 */
uchar morse_stat(void)
{
	return morse_char;
}

/**********************************************************************
 *	コマンド受領処理 sub.
 **********************************************************************
 *	c = 打鍵する ASCIIコード .
 *	i = 打鍵するコードのテーブル番号.(Alphabet 26個 + Numeric 10個 のどれか)
 */
static void morse_send_idx(uchar c,uchar i)
{
	uchar d = __LPM(&morse_tab[i]);
	morse_char = c;
	morse_pat = d;
	morse_cnt = d & 7;
	morse_sub = 0;
}
/**********************************************************************
 *	コマンド受領処理.
 **********************************************************************
 *	c = 打鍵する ASCIIコード .
 */
void morse_send(uchar c)
{
	if((c>='A')&&(c<='Z')) {morse_send_idx(c,c-'A'   );return;}
	if((c>='a')&&(c<='z')) {morse_send_idx(c,c-'a'   );return;}
	if((c>='0')&&(c<='9')) {morse_send_idx(c,c-'0'+26);return;}
	if((c==' ')) {morse_send_idx(c,'e'-'a');return;}
}

/**********************************************************************
 *	次の起床時間をセットする.
 **********************************************************************
 *	len = 0   長音待ち.
 *	len !=0   短音待ち.
 */
void sleep_key(uchar len)
{
	uchar    c;
	if(!len){c = MORSE_SPEED3;}
	else	{c = MORSE_SPEED1;}

	sleep_tsk(c << 5);
}
/**********************************************************************
 *	モールス打鍵タスク.
 **********************************************************************
 */
void morse_task(void)
{
	uchar keylen;
	if(morse_char==0) return;	//打鍵文字無し:

	switch(morse_sub) {	//サブシーケンスにより分岐処理:
	 case 0:	// KEY_ON ==========
		keylen= morse_pat & 0x80; 	//打鍵パターン.長音なら0、短音なら1.
				morse_pat<<=1; 		//打鍵パターンを１つシフト.
				morse_cnt--;		//打鍵数を１つ減少.

		if(morse_char != ' ') {		//空白以外.
			morse_out_mark();		//キー押下.
		}

		morse_sub = 1;				//次シーケンスに移行.
		sleep_key(keylen);			//起床までの待ち時間セット.
	 	break;
	 case 1:	// KEY_OFF ========
		morse_out_space();			//キー離す.

		morse_sub = 2;				//次シーケンスに移行.
		sleep_key(1);
	 	break;
	 case 2:	// STOP BIT =======
		if(	morse_cnt == 0 ) {		//全部打鍵した?
			morse_sub = 3;			//次シーケンスに移行.
			sleep_key(0);			//起床までの待ち時間セット.
		}else{
			morse_sub=0;			//シーケンス・リセット.
		}
	 	break;
	 default:
	 case 3:	// JOB END ========	打鍵終了.
		morse_sub = 0;
		morse_char = 0;				//次の打鍵受付可能にする.
	 	break;
	}
}
/**********************************************************************
 *	
 **********************************************************************
 */
