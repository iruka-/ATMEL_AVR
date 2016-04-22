#ifndef	usart_h_
#define	usart_h_
/******************************************************************
 *	USART 送受信モジュール
 ******************************************************************
 *	ボーレート設定 (U2Xを使用)
	static	void USART_init(uchar baud_h);
 *
 *	ＲＳ２３２Ｃ送受信タスク.
	static	void USART_RecvTask(void);
	static	void USART_SendTask(void);
 */

#define	TXBUF_SIZE	14			// 送信バッファサイズ. 4に限定.
#define	RXBUF_SIZE	14			// 受信FIFOサイズ.

void USART_Init(int baud);

// ATtiny2313は RAMが少ないので、USARTのバッファやポインタを
//	そのまま公開してアプリ側から参照する使い方になっている.

// tinyclockでは、USARTの受け渡しは行単位になるため、いわゆるFIFO
//	的な扱いをしないで、一行溜まったら処理するパターンで行なう.

//	受信系.
char USART_RecvCh(void);		//最後に受信した文字を取得する.
extern	uchar USART_rxbuf[];			// 受信バッファ.
extern	uchar USART_rxptr;				// FIFO書き込み地点.

//	送信系.
void USART_SendTask(void);		//送信バッファのデータを１文字づつ送信.
extern	uchar USART_txbuf[];			// ＲＳ２３２Ｃ送信バッファ.
extern	uchar USART_txptr;			   	// 
extern	uchar USART_txend;			   	// 

//	デバッグ用.
void USART_SendDirect(char c);	//USARTに直接1文字送信する.
void USART_Tick();
void USART_cmdproc();

/******************************************************************
 *	
 ******************************************************************
 */
#endif
