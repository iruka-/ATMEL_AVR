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
void putcrlf()
{
	putch1(0x0d);
	putch1(0x0a);
}
void USART_prompt(void)
{
	txp = 0;
	putcrlf();
	putch1('>');
	putflush();
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
