/**********************************************************************
 *	赤外線リモコン・キースキャン
 **********************************************************************
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "config.h"
#include "task.h"
#include "timer.h"
#include "usart.h"

void	timer_init(int cnt);
void	led_on();
void	led_off();

void	delay10();
void	delay8(char n);
uchar	set_code(uchar *buf,uchar maker_id,uchar code_id);

#define	MAX_SCAN_COLUM	7

#define	LED_ON		PORTA
#define	LED_PORT	0x01		// 赤LED
#define	ILD_PORT	0x02		// 赤外LED

#define	PBMASK 0x0f				// PB0:3 のみ有効.

uchar scan_cnt=0;				// 0～4
uchar trig_key=0;				// 押されたキーに対応するindex番号.(0～55)
uchar trig_keycnt=0;			// 押されたキーの個数(押されてなければ0.押されたら1)
								// 複数押されたときは2以上になるので、その場合はキーを捨てる.
uchar old_key=0;				// 前回の key index番号.(0～55)

uchar maker_id=0;
uchar maker_cnt=0;

uchar kb_stat[MAX_SCAN_COLUM];	// 7 x 8 = 56bit(key) 分の押された情報.
uchar ild_data[6];				// 赤外LEDから送出するデータ列.


//----------------------------------------
#define	ILD_PIN		0x01				// 赤外LEDのポート  PORTA.0
#define	ild_on()	PORTA |=  ILD_PIN;	// 赤外LEDを点灯させる. (正論理)
#define	ild_off()	PORTA &= ~ILD_PIN;	// 赤外LEDを消灯させる.
//----------------------------------------

#define	PN	16	//14	//18					// 赤外LED (38kHz) パルスの数. (1Tあたり)

/*
static uchar pulldn_tbl[8]={
	0x01,0x02,0x04,0x08,
	0x10,0x20,0x40,0x80,
};
*/

/**********************************************************************
 *	↑のテーブルと同値.
 *	index i(i=0～7)に対し、DDRD に書き込む値を計算する. 
 **********************************************************************
 */
uchar pulldn_tbl(uchar i)
{
	uchar m=0x01;
	while(i) {
		m<<=1;
		i--;
	}
	return m;
}


/**********************************************************************
 *	n x 2ms待つ.
 **********************************************************************
 */
void delay2ms(uchar n)
{
	uchar i;
	for(i=0;i<n;i++) {
		delay8(250);	//2mS
	}
}

/**********************************************************************
 *	LED(可視光) 点滅
 **********************************************************************
 */
void LED_pulse(uchar m)
{
	uchar i;
	for(i=0;i<m;i++) {
		led_on();
		delay2ms(25);	// 0.5秒.
		led_off();
		delay2ms(25);	// 0.5秒.
	}
}

void print_maker()
{
	if(maker_id) {
		LED_pulse(2);
	}else{
		LED_pulse(10);
	}
}

/**********************************************************************
 *	赤外線: 1T 分の38kHz信号を送出
 **********************************************************************
 *	38KHz = 13.1uS on + 13.1uS off
 * CPU Clk= 500kHz = 2uS
 *		3 Cycle On + 10 Cycle Off : Total 13 Cycle = 26nS (38kHz)
000001ac <ILD_pulse>:
 1ac:	d8 9a       	sbi	0x1b, 0	; 27
 1ae:	00 00       	nop
 1b0:	00 00       	nop
 1b2:	d8 98       	cbi	0x1b, 0	; 27
 1b4:	00 00       	nop
 1b6:	00 00       	nop
 1b8:	00 00       	nop
 1bc:	00 00       	nop
 1be:	00 00       	nop
	...
 1c0:	81 50       	subi	r24, 0x01	; 1
 1c2:	a1 f7       	brne	.-24     	; 0x1ac <ILD_pulse>
 1c4:	08 95       	ret


 */
//	500kHz Version
void ILD_pulse(uchar m)
{
	do {
		ild_on();		// 2:
		asm("nop");		// 1:
		asm("nop");		// 1:

		ild_off();		// 2:
		asm("nop");		// 1:
		asm("nop");		// 1:
		asm("nop");		// 1:
		asm("nop");		// 1:
	}while(--m);		// subi + bne = 3:
}

#if	0	// 1MHz Version
void ILD_pulse(uchar m)
{
	uchar i;
	for(i=0;i<m;i++) {
		ild_on();
		delay10();
		ild_off();
		delay10();
	}
}
#endif
/**********************************************************************
 *	赤外線: 1T 分の時間待ち.
 **********************************************************************
 */
void ILD_pause(uchar m)
{
	do {
		ild_off();		// 1:
		asm("nop");		// 1:
		asm("nop");		// 1:

		ild_off();		// 1:
		asm("nop");		// 1:
		asm("nop");		// 1:
		asm("nop");		// 1:
		asm("nop");		// 1:
		asm("nop");		// 1:
		asm("nop");		// 1:
	}while(--m);		// subi + bne = 3:
}
/**********************************************************************
 *	赤外線: 1bit 分のデータを送出
 **********************************************************************
 */
void ILD_bit(uchar bit)
{
	ILD_pulse(PN);
	ILD_pause(PN);
	if(bit) {			// bitが 1 の場合はポーズ時間を３倍取る.
		ILD_pause(PN);
		ILD_pause(PN);
	}
}
/**********************************************************************
 *	赤外線: 1byte分のデータを送出
 **********************************************************************
 */
void ILD_byte(uchar byte)
{
	uchar i;
	for(i=0;i<8;i++) {
		ILD_bit(byte & 1);
		byte >>= 1;
	}
}
/**********************************************************************
 *	赤外線: プリアンブル(sync信号)
 **********************************************************************
 */
void ILD_sync()
{
	ILD_pulse(PN*8);
	ILD_pause(PN*4);
}
/**********************************************************************
 *	赤外線: ポストアンブル(stop信号)
 **********************************************************************
 */
void ILD_stop()
{
	ILD_pulse(PN);
	ILD_pause(PN*8);
	ILD_pause(PN*8);
}
/**********************************************************************
 *	赤外線: リモコン信号(6byte) 送信.
 **********************************************************************
 *	送出データは、あらかじめ ild_data[6] に設定されているものとする.
 */
void ILD_send(void)
{
	ILD_sync();	// Preamble
	uchar i;
	for(i=0;i<6;i++) {
		ILD_byte(ild_data[i]);	// 1byte送信
	}
	ILD_stop();	// Postamble
}

/**********************************************************************
 *	キースキャン: 初期化
 **********************************************************************
 */
void scan_init()
{
	trig_key=0xff;
	trig_keycnt=0;
	scan_cnt=0;
	return;
}

//
//	メーカー切り替えは、オフタイマー長押し.
//

/**********************************************************************
 *	キースキャン: 押されたキー値に対するアクション(信号送信)を実行
 **********************************************************************
 */
void keybd_exec(uchar key)
{
#if	1
	if(key == 30) {	// OFF_TIMERキー.
		if(old_key==key) {
			maker_cnt++;
			if(	maker_cnt>=10) {	//約8秒 OFF_TIMERが押されたら
				maker_cnt=0;
				maker_id ^= 1;	//メーカーID切り替え.
				print_maker();
				old_key=0;
			}
		}
		return;
	}
	if(old_key!=key) {maker_cnt=0;}
#endif

#if	1
	if(key == 24) {	// (POWER)
		if(old_key==key) {
			return;		// POWERボタンはリピートしない.
		}
	}
#endif

	// キーマトリクスコードから、リモコンコード(6byte)に変換.
	// 戻り値rcは、チャネルコード(1-12,その他) 0なら無効キー.
	uchar rc=set_code(ild_data,maker_id,key);

	if(rc) {
		led_on();
		ILD_send();	// 6byte 赤外線送出.
		led_off();
	}

	if(key == 24) {	// (POWER)
		ILD_send();	// POWERボタンは念のため再送する.
	}
}

/**********************************************************************
 *	キースキャン: １列(8bit)分のデータを検査
 **********************************************************************
 */
void keybd_trigger(uchar idx,uchar trig)
{
	uchar i,bit=0x01;
	for(i=0;i<8;i++,bit<<=1) {		// bitをLSBからMSBまで検査する.
		if(trig & bit) {
			trig_key= idx + i*8;	//マトリクス番号. 0～39
			trig_keycnt++;			//検出された押下キーの総数.
		}
	}
}

/**********************************************************************
 *	キースキャン: 初期化
 **********************************************************************
 */
void keybd_init(void)
{
	scan_init();
	old_key=0xff;
}


/**********************************************************************
 *	キースキャン: 定時処理
 **********************************************************************
 */
void keybd_scan()
{
	uchar i,c,trig;

	//
	//	一周 回ったあとの処理.
	//
	if(	scan_cnt>=MAX_SCAN_COLUM) {
		if(	trig_keycnt==1 ) {			//検出キーが１つの場合、
			keybd_exec(trig_key);		//そのキーに対応する処理を実行する.
		}
		old_key=trig_key;
		scan_init();		//次の周回のための変数初期化.
		return;
	}
	//
	//	逐次処理.
	//
	i=scan_cnt;
	{
//		DDRD=pulldn_tbl(i+2);	// プルダウン共通線のLow出力を1本のみセットする.(PD2-6)
		DDRD=pulldn_tbl(i);		// プルダウン共通線のLow出力を1本のみセットする.(PD0-6)

		delay8(2);
		scan_cnt++;

		c = PINB;c ^= 0xff;  c &= PBMASK;
		trig = (c & (kb_stat[i] ^ PBMASK));	//トリガ抽出.

//		if(trig) keybd_trigger(i,trig);		//押された初回のみに反応.
		if(c)    keybd_trigger(i,c);		//押されていれば常に反応.

		kb_stat[i]=c;
	}
}
/**********************************************************************
 *	
 **********************************************************************
 */
