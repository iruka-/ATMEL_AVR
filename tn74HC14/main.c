/**********************************************************************
 *	74HC14の発振器のような代用品
 **********************************************************************
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

typedef unsigned char uchar;
void delay(char n);

/**********************************************************************
 *	メイン
 **********************************************************************


  PD5  = 周波数切り替え.
  
  PB0  = 出力.

  500kHz 

 */
int	main(void)
{
//	uchar c;
	unsigned int c=0;
	
	DDRD=0xff;		// PORTD 0-5 出力.
	DDRB=0xff;		// PORTB 出力
//	PORTD=0x3f;		// PORTD 0=5 PullUp.

	DDRA=0x00;		// PORTA 0-1 入力.
	PORTA=0x03;		// PORTA 0-1 pullup.

	while(1) {
		PORTB= c;		// 1
		PORTD=(c>>8);	// 1
		c++;			// 2
		asm("nop");		// 1
		asm("nop");		// 1

		asm("nop");		// 1
		asm("nop");		// 1
		asm("nop");		// 1
		asm("nop");		// 1

		asm("nop");		// 1
		asm("nop");		// 1
		asm("nop");		// 1
		asm("nop");		// 1

	}					// 2
}

#if	0
{	
	c = PIND;
	if(c & 0x20) {	// 0b0010_0000 bit5
		while(1) {
			PORTB=c;
			delay(4);
			c++;
		}
	}else{
		while(1) {
			PORTB=c;
			asm("nop");
			asm("nop");
			asm("nop");
			asm("nop");
			c++;
		}
	}
	return 0;
}
#endif

/**********************************************************************
 *	
 **********************************************************************
 */
