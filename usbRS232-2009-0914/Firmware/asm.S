/* asm.S */

#define BAUDRATE_H ( ( ( F_CPU / 16 ) >> 16 ) & 0xff )
#define BAUDRATE_M ( ( ( F_CPU / 16 ) >> 8 ) & 0xff )
#define BAUDRATE_L ( ( F_CPU / 16 ) & 0xff )

/*
 *	void memCopy(*d,*s,len);		// SRAM to SRAM copy
 *
 *	注意: lenは1～127まで.	(tiny2313のSRAMは128バイトしかないので.)
 *		  len <= 0 のときは何もしない.
 */

/*************************************************************/
	.text
	.global	memCopy
	.type	memCopy, @function

memCopy:
	cp r1,r20	/* __zero_reg__,r20 */
	brge .L4

	movw r26,r24
	movw r30,r22

.L3:
	ld		r24,Z+
	st		X+,r24
	subi	r20,1
	brne	.L3
.L4:
	ret


#include <avr/io.h>		//@@kuga


/*************************************************************/
/*
 *	void  baud_calc(int baud_l,uchar baud_h);
 *
 *	ボーレートから bbrの値を求める.
 *	UBRRL,UBRRHに設定する.
 *
 */
	.global	baud_calc
	.type	baud_calc, @function
//	-----------------------------------------------------------------------
//
//	ubrr = ( r / baud ) - 1 を求める.
//	引数:
//	   baud:  r22:r25:r24  17bit整数. ボーレートそのもの.(r22は下位1bitのみ使用)
//	変数:
//	   r:     r22:r21:r20  24bit整数  定数： USARTに与えるクロック数/2.
//	   i:	      r19:r18  16bit整数. ボーレートの逆数. UBRRにセットする値.
//	結果:
//	   (商-1) => i を UBRR に書き込んで戻る(void)
//	-----------------------------------------------------------------------

baud_calc:
// パラメータをバッファーメモリーから直接引き取るように変更（コードエリア削減）
	ldi	r30,lo8(modeBuffer)
	ldi	r31,hi8(modeBuffer)
	ldd	r24,Z+0
	ldd	r25,Z+1
	ldd	r22,Z+2

	// 24bit のbaudを >>1 する.
	lsr r22
	ror r25
	ror r24

	// r = 12000000/8/2; 	USARTに与えるクロック数/2.(U2X=1のとき)
	// 0x0f4240 = 16000000/8/2 = 1000000
	ldi r20,BAUDRATE_L
	ldi r21,BAUDRATE_M
	ldi r22,BAUDRATE_H

	// i = (-2);		以下のループでのiの初期値は (-1).
	ldi r18,lo8(-2)	//  但し、結果をあとで-1するので、ここで(-2)を与える.
	ldi r19,hi8(-2)

	// i = (r/b) の割り算を引き算ループで実行する.
.L8:
	// i++;
	subi r18,lo8(-(1))
	sbci r19,hi8(-(1))

	// r = r - b;	   24bitの引き算を実行.
	sub r20,r24
	sbc r21,r25
	sbc r22,r1		// baudは16bitなので bit23:16 は ゼロ固定.
	brcc .L8		// まだ引ける.
.L9:
	out _SFR_IO_ADDR(UBRRH),r19
	out _SFR_IO_ADDR(UBRRL),r18

	ret

#if	0	/* これは使わない */

#define	int24 long

int baud_calc(int baud)
{
	int24 r = 12000000/8/2;
	baud /= 2;
	baud &= 0xffff;

	int i=-2;	// 初期値=-1だが,さらに結果を -1 したい.
	do {
		i++;
		r -= baud;
	}while(r>=0);
	return i;
}
#endif

;*****
;REGISTER uint8_t RxRp ASM("r8");	//受信データ読み込み
;REGISTER uint8_t RxWp ASM("r9");	//受信データ書き込み
;REGISTER uint8_t TxRp ASM("r10");	//送信データ読み込み
;REGISTER uint8_t TxWp ASM("r11");	//送信データ書き込み
;*****

	.global	USART_getc
	.type	USART_getc, @function

//
//  クリチカルな?xWp,?x,Rpを割り込み禁止にせず更新するために
//	アセンブラ関数とする
//

USART_getc:
	mov	r25,r8
	ldi	r30,lo8(USART_rxbuf)
	ldi	r31,hi8(USART_rxbuf)
	add	r30,r8
;	adc	r31,r1		//ramは128しかないので繰り上がりなし
	ld	r24,Z
	inc	r25
	andi	r25,(RXBUF_SIZE-1)
	mov	r8,r25
	eor	r25,r25		//ucharを返すのに必要か？
	ret

	.global	USART_putc
	.type	USART_putc, @function

#if 0	//	送信割込みをやめたのでこれもやめる
//
//	送信の空きがない場合はすぐ帰るように変更すべきか？？
//	送信BUFFが8バイトしかとれない、USBパケットサイズと同一なので
//	効率的な制御ができない、rsbuf 16バイトに変更するとスタックが不足しそう
//
USART_putc:
	mov	r25,r11
	inc	r25
	andi	r25,0x0F	//(TXBUF_SIZE-1)
.L5:
	cp	r10,r25	//送信バッファーの空き確認
	breq	.L5		//空き無し空きができるまで待つ
	ldi	r30,lo8(rsbuf)
	ldi	r31,hi8(rsbuf)
	add	r30,r11		//TxWp
;	adc	r31,r1		//ramは128しかないので繰り上がりなし
	st	Z,r24
	mov	r11,r25
	sbi	_SFR_IO_ADDR(UCSRB),UDRIE
	ret

//
//
//USBの割込みを阻害しないよう,割込み禁止時間は極力短くする
//リエントラントではないので自身の割込みを禁止しうえで割り込み許可
//
#endif

	.text
    .global USART_RX_vect
    .type   USART_RX_vect, @function


USART_RX_vect:
	cbi	_SFR_IO_ADDR(UCSRB),RXCIE
	out	_SFR_IO_ADDR(GPIOR0),r18
	sei			//保留割込みがあってもseiの次の命令は割込まれず実行される
	in	r18,_SFR_IO_ADDR(UDR)	//データ引取りまでは割込み禁止で実行
	in	r2,_SFR_IO_ADDR(SREG)	//r2=SREGセーブレジスタ
;	push	r30
;	push	r31
	out	_SFR_IO_ADDR(GPIOR1),r30	//スタックが深くならないよう
	out	_SFR_IO_ADDR(GPIOR2),r31	//IOにレジスタ保存
	ldi	r30,lo8(USART_rxbuf)
	ldi	r31,hi8(USART_rxbuf)
	add	r30,r9		//RxWp
;	adc	r31,r1		//ramは128しかないので繰り上がりなし
	st	Z,r18
	mov	r18,r9
	inc	r18
	andi	r18,(RXBUF_SIZE-1)
	cpse	r8,r18		//バッファーFULLなら
	 mov	r9,r18		//ポインタの更新をしない(受信データは上書きで破棄される)

	in	r31,_SFR_IO_ADDR(GPIOR2)	;	pop	r31
	in	r30,_SFR_IO_ADDR(GPIOR1)	;	pop	r30

	in	r18,_SFR_IO_ADDR(GPIOR0)
	out	_SFR_IO_ADDR(SREG),r2
	cli	//この時点で受信割込みが保留中だとまずい
	sbi	_SFR_IO_ADDR(UCSRB),RXCIE	//再度受信割込みを許可
	reti
;
#if 0		//送信割込み使用中止（性能改善見られない）

	.text

	.global USART_UDRE_vect
	.type	USART_UDRE_vect, @function


USART_UDRE_vect:
	cbi	_SFR_IO_ADDR(UCSRB),UDRIE
	sei
	in	r3,_SFR_IO_ADDR(SREG)
	cp	r10,r11		//送信データがあるか確認
	breq	.L6		//送信データなし
;	push	r30
;	push	r31
	out	_SFR_IO_ADDR(OCR0A),r30	//スタックが深くならないよう
	out	_SFR_IO_ADDR(OCR0B),r31	//IOにレジスタ保存(タイマー0比較レジスタを転用)
	ldi	r30,lo8(rsbuf)
	ldi	r31,hi8(rsbuf)
	add	r30,r10		//TxRp
;	adc	r31,r1		//ramは128しかないので繰り上がりなし
	ld	r31,Z
	out	_SFR_IO_ADDR(UDR),r31
	mov	r31,r10
	inc	r31
	andi	r31,0x0F	//(TXBUF_SIZE-1)
	mov	r10,r31
	in	r31,_SFR_IO_ADDR(OCR0B)	;	pop	r31
	in	r30,_SFR_IO_ADDR(OCR0A)	;	pop	r30
	out	_SFR_IO_ADDR(SREG),r3
	cli	//ここまで実行の間に割込まれてその間に送信は完了しているかも知れない
	sbi	_SFR_IO_ADDR(UCSRB),UDRIE
	reti
.L6:		//送信割込み禁止のまま終了
	out	_SFR_IO_ADDR(SREG),r3
	reti

#endif




/*************************************************************/
