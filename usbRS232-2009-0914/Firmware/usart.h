/******************************************************************
 *	USART 送受信モジュール
 ******************************************************************
 *	ボーレート設定 (U2Xを使用)
	static	void USART_init(uchar baud_h);
 *
 *	RS232C送受信タスク.
	static	void USART_RecvTask(void);
	static	void USART_SendTask(void);
 */


/******************************************************************
 *
 ******************************************************************
 */
//#define F_CPU 12000000      	/* MCUクロック周波数 */
//#define BAUD 9600             	/* 目的USARTボーレート速度 */
//#define MYUBRR F_CPU/16/BAUD-1 	/* 目的UBRR値 */


#ifndef TXBUF_SIZE			// 送信バッファサイズ
#error Define TXBUF_SIZE in Makefile
#endif
#ifndef RXBUF_SIZE			// 受信FIFOサイズ.
#error Define RXBUF_SIZE in Makefile
#endif

uchar rsbuf[TXBUF_SIZE];// RS232C送信バッファ.
// REGISTER uchar rsptr ASM("r7");	 // 取り出し地点 (0..3) 、4 の時は空.

uchar USART_rxbuf[RXBUF_SIZE];	// 受信FIFO.
//REGISTER uchar USART_rxptr ASM("r7");	// FIFO書き込み地点.

REGISTER uint8_t RxRp ASM("r8");	//受信データ読み込み
REGISTER uint8_t RxWp ASM("r9");	//受信データ書き込み

REGISTER uint8_t TxRp ASM("r10");	//送信データ読み込み
REGISTER uint8_t TxWp ASM("r11");	//送信データ書き込み

//#define	 USART_default_baud  19200,0


//	baud_rateを ubrrにセットする値に変換する.
void baud_calc(void);	 // asm.S

uchar USART_getc(void);
static void USART_putc(uchar);

/******************************************************************
 *	USARTの初期化.
 ******************************************************************
 */
/******************************************************************
 *	ボーレート設定.
 *  引数: int baud_l  ボーレート(下位16bit)         [15:0]
 *      uchar baud_h  ボーレート(上位 8bit) [23:16]
 ******************************************************************
 */
static void USART_Init(void)
{
	uchar csrc=0x0e;	// 8N2

	baud_calc();
//	UBRRH = brr>>8; 		/* ボーレート設定(上位バイト) */
//	UBRRL = brr;     	/* ボーレート設定(下位バイト) */
	UCSRC = csrc;		/* フレーム形式設定(8ビット,2ストップ ビット) */
//	UCSRB = (1<<RXEN)|(1<<TXEN);      /* 送受信許可 */
	UCSRA = (1<<U2X);	// プリスケーラを 1/16 → 1/8 に変更.

//	rsptr = TXBUF_SIZE;

	RxRp = RxWp = 0;
	TxRp = TxWp = 0;
	UCSRB = (1<<RXEN)|(1<<TXEN) | (1<<RXCIE);      /* 送受信許可 & 受信割込み許可*/
}

#if 0	//@@kuga 削除
/******************************************************************
 *	RS232C受信タスク.
 ******************************************************************
 */
static void USART_RecvTask(void)
{
	while( UCSRA & (1<<RXC) ) {					// 受信完了? .
		uchar rxchr = UDR;    					// 受信データ取得.
		USART_rxbuf[USART_rxptr] = rxchr;		// FIFOの書き込みポインタ.
		// FIFO一杯なら、ポインタを更新しない.
		if(	USART_rxptr < (RXBUF_SIZE-1)) {
			USART_rxptr++;
		}
	}
}

/******************************************************************
 *	RS232C送信タスク.
 ******************************************************************
 */
static void USART_SendTask(void)
{
	if(	rsptr < TXBUF_SIZE ) {
		//送信バッファにデータが残っているなら,
		//
		//	1文字送信を行う.
		//
		if ( !(UCSRA & (1<<UDRE)) ) {
			return ;	/* 2:送信バッファ空き待機 */
		}
	    UDR = rsbuf[rsptr++];     			/* 送信 */
	}else{
        if( usbAllRequestsAreDisabled() ) {
            usbEnableAllRequests();
        }
	}
	return ;			/* 0:送信バッファ空 */
}

#endif


static void USART_SendTask(void)
{
	if ( (TxWp!=TxRp) && (UCSRA & (1<<UDRE)) ) {
		UDR=rsbuf[TxRp++];
		TxRp &= (TXBUF_SIZE-1);
	   if( usbAllRequestsAreDisabled() ) {
			uchar used=(TxWp-TxRp) & (TXBUF_SIZE-1);
	 		if ( used<=(TXBUF_SIZE-HW_CDC_PACKET_SIZE-1) ) {		//送信に空あり?
	            usbEnableAllRequests();
	        }
		}
	}
}


static void USART_putc(uchar data)
{
	rsbuf[TxWp]=data;
	TxWp++;
	TxWp &= (TXBUF_SIZE-1);
}


