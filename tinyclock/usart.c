/******************************************************************
 *	USART 送受信モジュール
 ******************************************************************
 *	ボーレート設定 (U2Xを使用)
	static	void USART_init(baud);
 *
 *	ＲＳ２３２Ｃ送受信タスク.
	static	void USART_RecvTask(void);
	static	void USART_SendTask(void);

 */


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "config.h"
#include "timer.h"
#include "usart.h"

#define	ASTERISK_GUARD		(0)	// (1):コマンドの前に'*'を必須とする.
								// '*p'
								// '*t' など.

/******************************************************************
 *	
 ******************************************************************
 */
//#define MYUBRR F_CPU/16/BAUD-1 	/* 目的UBRR値 */


uchar USART_txbuf[TXBUF_SIZE];// ＲＳ２３２Ｃ送信バッファ.
uchar USART_txptr;			   // 
uchar USART_txend;			   // 

uchar USART_rxbuf[RXBUF_SIZE];	// 受信FIFO.
uchar USART_rxptr;				// FIFO書き込み地点.
 char USART_rxc;

uchar txp;
uchar rxp;

#define	CPUCLK	3579545
#define	CLKDIV	8

unsigned int	div_x;
static  char    weeks[]="SunMonTueWedThuFriSat";
extern	char	cmd_tick;			//毎秒時刻printする.
extern	int   	adj_count;			//調整値

#if	0
/*
//	-----------------------------------------------------------------------
//
//	ubrr = ( r / baud ) - 1 を求める.
//	引数:
//	   baud: ボーレートそのもの.
//	変数:
//	   r:    定数： USARTに与えるクロック数
//	   i:	 ボーレートの逆数. UBRRにセットする値.
//	結果:
//	   (商-1) => i
//	備考:
//		割り算はコード量が多いので、引き算ループで実装.
//	-----------------------------------------------------------------------
 */
int baud_calc(int baud)
{
	int r = CPUCLK/CLKDIV/8;	// 

	int i=-2;	// 初期値=-1だが,さらに結果を -1 したい.
	do {
		i++;
		r -= baud;
	}while(r>=0);
	return i;
}
#endif


//	baud_rateを ubrrにセットする値に変換する.
//void baud_calc(int baud_l,uchar baud_h);	 // cpy.S

/******************************************************************
 *	ＵＳＡＲＴの初期化.
 ******************************************************************
 */
/******************************************************************
 *	ボーレート設定.
 *  引数: int baud  ボーレート
 ******************************************************************
 */
//static 
void USART_Init(int baud)
{
	uchar csrc=0x0e;	// 8N2
	int brr;

//	brr = baud_calc(baud);
	brr=5;	// (55930 / 9600) - 1 の近似値; 55930はCPUクロック/8== (3.579545MHz/8分周)/8.

	UBRRH = brr>>8; 	/* ﾎﾞｰﾚｰﾄ設定(上位ﾊﾞｲﾄ) */
	UBRRL = brr;     	/* ﾎﾞｰﾚｰﾄ設定(下位ﾊﾞｲﾄ) */
	UCSRC = csrc;		/* ﾌﾚｰﾑ形式設定(8ﾋﾞｯﾄ,2ｽﾄｯﾌﾟ ﾋﾞｯﾄ) */
	UCSRB = (1<<RXEN)|(1<<TXEN)|(1<<RXCIE)|(1<<UDRIE);      /* 送受信許可 | RX割り込み許可 */
	UCSRA = (1<<U2X);	  // プリスケーラを 1/16 → 1/8 に変更.

	USART_rxptr = 0;
	USART_txptr = 0;
	USART_txend = 0;
}

/******************************************************************
 *	デバッグ用.
 ******************************************************************
 */
void USART_SendDirect(char c)
{
	if ( !(UCSRA & (1<<UDRE)) ) {
		return ;	/* 2:送信ﾊﾞｯﾌｧ空き待機 */
	}
    UDR = c; 		/* 送信 */
}
char USART_RecvCh(void)
{
	return USART_rxc;
}

void putch1(uchar ch)
{
	USART_txbuf[txp++]=ch;
}
void putflush(void)
{
	USART_txend=0;
	USART_txptr=0;

	USART_txend=txp;
	USART_SendTask();
}
void putnum1(uchar num)
{
	putch1('0'+num);
}
void putnum2(uchar num)
{
	char n=0;
	while(num>=10) {
		num-=10;
		n++;
	}
	putnum1(n);
	putnum1(num);
}

void putweek(uchar week)
{
	char *p=&weeks[week*3];
	char i;
	for(i=0;i<3;i++) {
		putch1(*p++);
	}
}

void putcrlf()
{
	putch1(0x0d);
	putch1(0x0a);
}
void USART_Tick(void)
{
	txp = 0;

	putweek(cur.week);
		putch1(' ');
	putnum2(cur.hour);
		putch1(':');
	putnum2(cur.minute);
		putch1(':');
	putnum2(cur.sec);

	putcrlf();
	putflush();
}

void print_alarm(void)
{
	txp = 0;

		putch1(' ');
	putnum2(alarm.hour);
		putch1(':');
	putnum2(alarm.minute);
		putch1(':');
	putnum2(alarm.sec);

	putcrlf();
	putflush();
}

void USART_prompt(void)
{
	txp = 0;
	putcrlf();
	putch1('>');
	putflush();
}

char spskip(void)
{
	char c;
	while(1) {
		c=USART_rxbuf[rxp];
		if(c!=' ') return c;
		rxp++;
	}

}
char getch1(void)
{
	char c;
	c=USART_rxbuf[rxp];
	if(c!=0x0d) rxp++;
	return c;
}
char getnum1(void)
{
	char c=getch1();
	c -= '0';
	return c;
}
char getnum2(void)
{
	uchar c1,c2;
	c1 = getnum1();
	if(c1<10) {
		c2 = getnum1();
		if(c2<10) {
			return c1*10 + c2;
		}else{
			rxp--;
			return c1;
		}
	}
	return -1;
}
void set_time(void)
{
	char c;
	spskip();
	c = getnum2();if(c<0) return;

	if(getch1()!=':')return;

	cur.hour = c;

	c = getnum2();if(c<0) return;
	cur.minute = c;

	cur.sec   = 0;

	c = getch1(); if(c!=':')return;
	c = getnum2();if(c<0) return;
	cur.sec   = c;

}
void set_alarm(void)
{
	char c;
	spskip();
	c = getnum2();if(c<0) return;
	if(getch1()!=':')return;

	alarm.hour = c;

	c = getnum2();if(c<0) return;
	alarm.minute = c;

	alarm.sec   = 0;
	c = getch1(); if(c!=':')return;
	c = getnum2();if(c<0) return;
	alarm.sec   = c;

}
void set_week(void)
{
	spskip();
	char c = getnum1();
	if(getch1()!=':')return;

	if(c<7) {
		cur.week = c;
		USART_Tick();
	}
}
int div10(unsigned int d)
{
	uchar i;
	uchar rc=0;
	for(i=0;i<10;i++) {
		if(	div_x >= d) {
			div_x -= d;
			rc++;
		}
	}
	return rc;
}
void putn10(unsigned int d)
{
	putnum1(div10(d));
}
void print_adjust(void)
{
	txp = 0;
	div_x=adj_count;
	putn10(10000);
	putn10(1000);
	putn10(100);
	putn10(10);
	putnum1(div_x);
	putcrlf();
//	putnum2(alarm.hour);
//	putcrlf();
	putflush();
}

void set_adjust(void)
{
	char c;
	spskip();
	c = getnum1();if(c<0) return;
	div_x = c;
	while(1) {
		c = getnum1();if(c<0) break;
		div_x = div_x * 10 + c;
	}
	adj_count = div_x;
}
void USART_cmdproc(void)
{
	uchar c;
	if(USART_RecvCh() != 0x0d) return;

	USART_rxc=0;

	rxp=0;

#if	ASTERISK_GUARD
	c=getch1();
	if(c!='*') {
		USART_prompt();	
		USART_rxptr = 0;	//次の１行入力を行なう.
		return;
	}
#endif

	c=getch1();	//	c = USART_rxbuf[rxp++];
	if(c=='s') {
		set_time();
		USART_Tick();
	}
	if(c=='a') {
		set_alarm();
		print_alarm();
	}
	if(c=='j') {
		set_adjust();
		print_adjust();
	}
	if(c=='w') {
		set_week();
	}
	if(c=='p') {
		USART_Tick();
	}
	if(c=='t') {
		cmd_tick ^= 1;
	}
	
	USART_rxptr = 0;	//次の１行入力を行なう.
}

/******************************************************************
 *	ＲＳ２３２Ｃ送信タスク.
 ******************************************************************
 */
//static 
void USART_SendTask(void)
{
	if(	USART_txptr < USART_txend ) {	//送信バッファにデータが残っているなら,
		//
		//	１文字送信を行う.
		//
		if ( !(UCSRA & (1<<UDRE)) ) {
			return ;	/* 2:送信ﾊﾞｯﾌｧ空き待機 */
		}
	    UDR = USART_txbuf[USART_txptr++];     			/* 送信 */
	}
	return ;			/* 0:送信バッファ空 */
}

/******************************************************************
 *	USART 送信割り込み.
 ******************************************************************
 */
ISR(USART_UDRE_vect)
{
	USART_SendTask();
}


/******************************************************************
 *	USART 受信割り込み.
 ******************************************************************
 */
ISR(USART_RX_vect)
{
	if( UCSRA & (1<<RXC) ) {					// 受信完了？ .
		uchar rxchr = UDR;    					// 受信データ取得.
		USART_rxbuf[USART_rxptr] = rxchr;		// FIFOの書き込みポインタ.
		USART_rxc = rxchr;
		USART_SendDirect(rxchr);

		// FIFO一杯なら、ポインタを更新しない.
		if(	USART_rxptr < (RXBUF_SIZE-1)) {
			USART_rxptr++;
		}
	}
}
/******************************************************************
 *	USART受信割り込み.（エコーバックテスト）
 ******************************************************************
 */
#if	0
ISR(USART_RX_vect)
{
	if( UCSRA & (1<<RXC) ) {					// 受信完了？ .
		uchar rxchr = UDR;    					// 受信データ取得.
		USART_SendDirect(rxchr);
	}
}
#endif

/******************************************************************
 *	
 ******************************************************************
 */
