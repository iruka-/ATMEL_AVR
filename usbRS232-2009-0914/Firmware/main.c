/*******************************************************************************
 *			usbRS2313 :	  USB to RS232C	serial port	for	ATtiny2313
 *******************************************************************************
 *メモ欄:
 *
 *
 *
 *
 *
 *
 *
 *******************************************************************************
 */
#include "hardware.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <string.h>
#include <avr/eeprom.h>
#include "usbdrv.h"


/*******************************************************************************
 *	Switches
 *******************************************************************************
 */
#define	OPTIMIZE_SIZE		(1)		// Enable global register assignment

#if	OPTIMIZE_SIZE
#define	REGISTER	register
#define	ASM( reg )	asm( reg )
#else
#define	REGISTER
#define	ASM( reg )
#endif


/*******************************************************************************
 *	deviceDescriptor / configDescriptor for CDC Device.
 *******************************************************************************
 */
#include "usbdesc.h"


/*******************************************************************************
 *	Static Work
 *******************************************************************************
 */

//EEPROM 0番地に割りついていることを期待したコードなので注意
EEMEM uint8_t saved_mode[]={0x80, 0x25, 0,	0, 0, 0, 8};  /* default: 9600 bps,	8n1 */

uchar	modeBuffer[7];
// =	{0x80, 0x25, 0,	0, 0, 0, 8};  /* default: 9600 bps,	8n1	*/

REGISTER uchar	requestType     ASM("r4");
REGISTER uchar  sendEmptyFrame	ASM("r5");	// = 1;

#if	USE_DCD_REPORTING
REGISTER uchar	intr3Status     ASM("r6");
#endif

//	注意:	register r7 以降は usart.h 側で使用する.
//	*****************************************************************************


/*******************************************************************************
 *	Serial Port Control
 *******************************************************************************
 */
#include "usart.h"


//	注意!! このmemCopyは (char)len <= 0 なら何もしない.
void memCopy(uchar *d,uchar *s,uchar len);		// SRAM to SRAM copy


/* CDC class requests: */
enum {
	SEND_ENCAPSULATED_COMMAND =	0,
	GET_ENCAPSULATED_RESPONSE,
	SET_COMM_FEATURE,
	GET_COMM_FEATURE,
	CLEAR_COMM_FEATURE,
	SET_LINE_CODING	= 0x20,
	GET_LINE_CODING,
	SET_CONTROL_LINE_STATE,
	SEND_BREAK
};

/* defines for 'requestType' */
#define	REQUEST_TYPE_LINE_CODING	0	/* CDC GET/SET_LINE_CODING */

/* ------------------------------------------------------------------------- */

static void mode_read_eeprom(void)
{
	uint8_t *p=modeBuffer, i, ee_adr;

	i=7;
	ee_adr=0;	//ee_adr=(uint8_t)saved_mode;
	do {
		eeprom_busy_wait();
		EEAR = ee_adr++;
		EECR |= (1<<EERE);
		*p++=EEDR;
	} while (--i);
}

/* ------------------------------------------------------------------------- */
/* ----------------------------- USB interface ----------------------------- */
/* ------------------------------------------------------------------------- */

/******************************************************************
 *	デフォルト以外のコントロール転送発生時に呼び出されるエントリー.
 ******************************************************************
 *	ベンダリクエストもしくはクラスリクエストに応答する.
 *	引数   :usbRequset_t *rq;      リクエストパケット構造体.
 *          詳細は usbdrv.h を参照.
 *			特にrq->bRequest にコマンド番号を入れることが多い.
 *	戻り値 :uchar         replyLen
 *			バイト数を返した場合は usbMsgPtrにアドレスを入れておく.
 *			0xff を返した場合は usbFunctionRead()が呼び出される.
 *
 */
uchar usbFunctionSetup(uchar data[8])
{
	usbRequest_t	*rq	= (void	*)data;
	uchar			rqType = rq->bmRequestType & USBRQ_TYPE_MASK;

	if(rqType == USBRQ_TYPE_CLASS){	   /* class	request	type */
		if(rq->bRequest	== GET_LINE_CODING){
			mode_read_eeprom();
			usbMsgPtr = modeBuffer;
			return 7;
//			requestType	= REQUEST_TYPE_LINE_CODING;
//			return 0xff;
		}
		if(rq->bRequest == SET_LINE_CODING){
			requestType	= REQUEST_TYPE_LINE_CODING;
			return 0xff;
		}

#	if USE_DCD_REPORTING
		if(rq->bRequest	== SET_CONTROL_LINE_STATE){
			/* Report serial state (carrier	detect). On	several	Unix platforms,
			 * tty devices can only	be opened when carrier detect is set.
			 */
			intr3Status	= 2;
		}
#	endif

#if	0	//kuga これがあるとrs232をhostに送信する処理がlockする??
		/*  Prepare bulk-in endpoint to respond to early termination   */
		if((rq->bmRequestType & USBRQ_DIR_MASK) == USBRQ_DIR_HOST_TO_DEVICE) {
			sendEmptyFrame  = 1;
		}
#endif
	}
	return 0;
}



#if	0	//@@kuga
/*******************************************************************************
 *	Control-In 実行時に呼ばれる.
 ******************************************************************
 *	戻り値は、ホストに転送するバイト数.(<= 8)
 *				0xffの時はstall
 *	これが呼びだされるには、まず usbFunctionSetup() の返り値が
 *  0xffでなければならない.
 */
uchar usbFunctionRead(uchar	*data, uchar len)
{
	if(requestType == REQUEST_TYPE_LINE_CODING){
		/* return the "virtual"	configuration */
		mode_read_eeprom();
		memCopy(data, modeBuffer, 7);
		return 7;
	}
	return 0;	/* error ->	terminate transfer */
}
#endif

/*******************************************************************************
 *	Control-Out 実行時に呼ばれる.
 *******************************************************************************
 */
uchar usbFunctionWrite(uchar *data,	uchar len)
{
	if(requestType == REQUEST_TYPE_LINE_CODING){
		uint8_t  i,ee_adr,*p;
		/* Don't know why data toggling	is reset when line coding is changed, but it is... */
		USB_SET_DATATOKEN1(USBPID_DATA1);	/* enforce DATA0 token for next	transfer */
		/* store the line configuration	so that	we can return it on	request	*/
		memCopy(modeBuffer, data, 7);

		// mode情報をEEPROMに保存する
		//標準ライブラリは使えない		eeprom_write_block(modeBuffer,(void *)ee_adr,7);
		ee_adr=0; i=7; p=modeBuffer;
		do {
			eeprom_busy_wait();
			EECR=(0<<EEPM1)|(0<<EEPM0);
			EEAR=ee_adr++;
			EECR |= (1<<EERE);
			if (EEDR!=*p) {
				EEDR=*p;
				cli();		//EEMPEセットからEEPEセットまでは4clk以内に実行する必要がある
				EECR |= (1<<EEMPE);
				sei();	//seiの次の命令は割込み保留中でも実行される
				EECR |= (1<<EEPE);
			}
			p++;
		} while (--i);
//		USART_Init( *((int*)modeBuffer) , modeBuffer[2] );
		USART_Init();
//		return 1;
	}
	return 1;	/* error ->	accept everything until	end	*/
}

/*******************************************************************************
 *	HOST->DEVICE:(Bulk-out) 実行時に呼ばれる.
 *******************************************************************************
 */
void usbFunctionWriteOut(uchar *data, uchar	len)
{
#if 1	//@@kuga
//	if (len) {
		do {
			USART_putc(*data++);
		} while (--len);

		len=(TxWp-TxRp) & (TXBUF_SIZE-1);	//バッファー使用量
 		if ( len>(TXBUF_SIZE-HW_CDC_PACKET_SIZE-1) ) {		////残がパケットサイズより小さい場合
			usbDisableAllRequests();	//次の要求をとめておく
		}
//	}
#else
//========iruka============
	if (len) {
#if	1
		// 未送信データがあるなら、ここで全部送信するまで待つ.
		while( rsptr < TXBUF_SIZE ) {
	//		USART_RecvTask();
			USART_SendTask();
		}
#endif
		// 送信バッファに溜める.
		rsptr = 8 - len;
		memCopy(rsbuf+rsptr,data,len);
		USART_SendTask();
		if( rsptr < TXBUF_SIZE ) {	//送信バッファが空でないなら、フロー制御する.
			usbDisableAllRequests();
		}
	}
//=======iruka=============
#endif
}


/*******************************************************************************
 *	usbSetInterrupt :	ＲＳ２３２受信データをホストＰＣに送る.
 *******************************************************************************
 */
static	void setInterrupt_xfer(void)
{
//	static uchar buffer[HW_CDC_PACKET_SIZE];
#define buffer modeBuffer		//modeBufferを転用 @@kuga

#if 1	//@@kuga
	uchar rxcnt=(RxWp-RxRp) & (RXBUF_SIZE-1);
	if((rxcnt > 0) || sendEmptyFrame){
		if (rxcnt) {
			uchar i,*p;
			if(rxcnt >= HW_CDC_PACKET_SIZE) {
				rxcnt = HW_CDC_PACKET_SIZE;	//１回の転送は８バイトまで.
			}
			p=buffer; i=rxcnt;
			do {
				*p++=USART_getc();
			} while (--i) ;
		}
		usbSetInterrupt(buffer,rxcnt);
		sendEmptyFrame=0;
		if(rxcnt==HW_CDC_PACKET_SIZE) {
			sendEmptyFrame=1;
		}
	}
#else
	uchar rxcnt = USART_rxptr;		//受信文字数.
	if((rxcnt > 0) || sendEmptyFrame){
		if(rxcnt >= 8) rxcnt = 8;	//１回の転送は８バイトまで.

		// 受信済データをbufferにコピー.
		memCopy(buffer,USART_rxbuf,rxcnt);

		// 受信済データ(max 8byte) をUSART_rxbuf[]から削除.
		USART_rxptr -= rxcnt;
		memCopy(USART_rxbuf,USART_rxbuf+8,USART_rxptr);	//バッファずらし.

		/* send	an empty block after last data block to	indicate transfer end */
		usbSetInterrupt(buffer,rxcnt);
		sendEmptyFrame=0;
		if(rxcnt==8) {sendEmptyFrame=1;}
	}
#endif
}


/*******************************************************************************
 *	usbSetInterrupt3 :
 *******************************************************************************
 */
static	void setInterrupt3(void)
{
#if	USE_DCD_REPORTING
	/* We need to report rx	and	tx carrier after open attempt */
	if(intr3Status != 0	&& usbInterruptIsReady3()){
		static uchar serialStateNotification[8]	= {0xa1, 0x20, 0, 0, 0,	0, 2, 0};
		static uchar serialStateData[2]	= {3, 0};
		if(intr3Status == 2){
			usbSetInterrupt3(serialStateNotification, 8);
		}else{
			usbSetInterrupt3(serialStateData, 2);
		}
		intr3Status--;
	}
#endif
}

static void LED_Control()
{
#if	HW_USE_LED_CONTROL
#if 1	//@@kuga
	if (RxWp!=RxRp)
#else
	if(USART_rxptr)
#endif
	{
		PORTB &= ~(1<<HW_LED_PIN);
	}else{
		PORTB |=  (1<<HW_LED_PIN);
	}
#endif
}
/*******************************************************************************
 *
 *******************************************************************************
 */
static	void hardwareInit()
{
#if 1	// HIDaspxの回路用.
	PORTD |= (1<<PD5) | (1<<PD0);		/* PD5 USB D- pullup  */
#else
	PORTD = 1;				// RxD Pullup
#endif

	DDRD = ~USBMASK;        /* all outputs except USB data */
	DDRB = 0x0f;			/* PB7-4=in PB3-0=out*/

	sendEmptyFrame	= 1;	//@@kuga

#if	USE_DCD_REPORTING
	intr3Status = 0;
#endif
}

/*
 *	*******************************************************************
 * 	回路図は、ＨＩＤａｓｐ（ｘ）、ＨＩＤｓｐｈのものと共通です。
 *	*******************************************************************
 */

/*******************************************************************************
 *	main Loop
 *******************************************************************************
 */
int	main(void)
{
	hardwareInit();
	usbInit();
	mode_read_eeprom();		//eepromに保存してある設定を初期値とする
	USART_Init();
//	USART_Init(USART_default_baud);
	sei();
	for(;;){	/* main	event loop */
		usbPoll();
//@@kuga		USART_RecvTask();
		USART_SendTask();
		LED_Control();
		if(	usbInterruptIsReady() ) {
	//		PORTB &= ~(1<<PB2) ;	//@@kuga for debug
			setInterrupt_xfer();		// 受信データをホストＰＣに転送.
	//		PORTB |= (1<<PB2) ;		//@@kuga for debug
		}
		setInterrupt3();				// EndPoint3: DCD_REPORTING
	}
	return 0;
}

