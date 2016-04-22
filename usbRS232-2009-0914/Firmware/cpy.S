このソースは使用しません.

//
//	int  baud_select(uchar baud_h);
//		 baud_h はボーレート(9600等) を256で割ったほうの値.
//
//	void memCopy(*d,*s,len);		// SRAM to SRAM copy
//
//	注意: lenは1～127まで.	(tiny2313のSRAMは128バイトしかないので.)
//		  len <= 0 のときは何もしない.
//
	.text
//	.global	pgmemcpy
//	.type	pgmemcpy, @function
//
//	pgmemcpy(*d,*s,len);	// PGMEM to SRAM copy
//pgmemcpy:
//	movw r26,r24	// dst = X
//	movw r30,r22	// src = Z
//.L2:
//	lpm		r24,Z+
//	st		X+,r24
//	subi	r20,1	// size--
//	brne	.L2
//
//	ret


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

/*

//	以下、U2X=1のとき. (プリスケーラー=1/8)
#define	USART_default_baud 1249,0,0x0e	//   1200,8N2
#define	USART_default_baud 624,0,0x0e	//   2400,8N2
#define	USART_default_baud 311,0,0x0e	//   4800,8N2
#define	USART_default_baud 155,0,0x0e	//   9600,8N2
#define	USART_default_baud  77,0,0x0e	//  19200,8N2
#define	USART_default_baud  38,0,0x0e	//  38400,8N2
#define	USART_default_baud  25,0,0x0e	//  57600,8N2
#define	USART_default_baud  18,0,0x0e	//  76800,8N2
#define	USART_default_baud  12,0,0x0e	// 115200,8N2
#define	USART_default_baud   5,0,0x0e	// 250000,8N2
#define	USART_default_baud   4,0,0x0e	// 375000,8N2
#define	USART_default_baud   3,0,0x0e	// 400000,8N2
#define	USART_default_baud   2,0,0x0e	// 500000,8N2
#define	USART_default_baud   1,0,0x0e	// 750000,8N2
#define	USART_default_baud   0,0,0x0e	//1500000,8N2
//
// 注意:ポーリング受信の周期より速くすると文字の取りこぼしが発生します.
//
    1200 :      4b0
    2400 :      960
    4800 :     12c0
    9600 :     2580
   19200 :     4b00
   38400 :     9600
   57600 :     e100
   76800 :    12c00
*/

/*
int	baud_select(uchar baud_h,uchar )
{
	switch(baud_h) {
	 case 0x04:	return 1249;	//   1200,8N2
	 case 0x09:	return  624;	//   2400,8N2
	 case 0x12:	return  311;	//   4800,8N2
	 case 0x25:	return  155;	//   9600,8N2
	 case 0x4b:	return   77;	//  19200,8N2
	 case 0x96:	return   38;	//  38400,8N2
	 case 0x2c:	return   25;	//  57600,8N2
	 default:   return   77;	//  19200,8N2
	}
}
*/

/*
#define	BAUD_LH(nbaud)	((nbaud) & 0xff),((nbaud) / 256)
#define	BAUD_TAB_MAX	7
PROGMEM char baud_select_tab[] = {
	0x04,	BAUD_LH(1249)	,	//   1200,8N2
    0x09,	BAUD_LH( 624)	,   //   2400,8N2
	0x12,	BAUD_LH( 311)	,   //   4800,8N2
    0x25,	BAUD_LH( 155)	,   //   9600,8N2
    0x4b,	BAUD_LH(  77)	,   //  19200,8N2
    0x96,	BAUD_LH(  38)	,   //  38400,8N2
    0xe1,	BAUD_LH(  25)	,   //  57600,8N2
};
*/

	.section	.progmem.data,"a",@progbits
	.type	baud_select_tab, @object
	.size	baud_select_tab, 24
baud_select_tab:
	.byte	0x04,0xe1,0x04		/*   1200 */
	.byte	0x09,0x70,0x02		/*   2400 */
	.byte	0x12,0x37,0x01		/*   4800 */
	.byte	0x25,0x9b,0x00		/*   9600 */
//	.byte	0x4b,0x4d,0x00		/*  19200 */
	.byte	0x7a,0x2f,0x00		/*  31250 */
	.byte	0x96,0x26,0x00		/*  38400 */
	.byte	0xe1,0x19,0x00		/*  57600 */
	.byte		 0x4d,0x00		/*  19200 default */
	.byte   0
/*
int	baud_select(uchar baud_h)
{
	char *t = baud_select_tab;
	uchar i,l,h;
	for(i=0;i<BAUD_TAB_MAX;i++) {
		if(baud_h == PRG_RDB(t)) {
			t++; l = PRG_RDB(t);
			t++; h = PRG_RDB(t);
			return (l+(h<<8));
		}
		t+=3;
	}
	return 25;
}
*/


	.text
	.global	baud_select
	.type	baud_select, @function
baud_select:
	mov r18,r24		/* baud_h */
	ldi r30,lo8(baud_select_tab)
	ldi r31,hi8(baud_select_tab)
	ldi r25,7		/* Loop Count = 7 */
.L01:
	lpm	r24,Z+		/* table[0] */
	cp  r18,r24
	breq .L02
		lpm	r24,Z+	/* table[1] */
		lpm	r24,Z+	/* table[2] */
		subi r25,1
	brne .L01
//no hit:
//	ldi r24,lo8(25)
//	ldi r25,hi8(25)
//	ret

/* | fall throw */
/* v */

// hit !!!
.L02:
	lpm	r24,Z+
	lpm	r25,Z+
	ret


