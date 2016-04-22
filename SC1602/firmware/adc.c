/**********************************************************************
 *	
 **********************************************************************
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "task.h"

void adc_init(void)
{
	// A/D変換 MUXレジスタ設定.
	ADMUX = 0b00000101;		// Aref,ADC5
//	ADMUX = 0b11000101;		// internal ref,ADC5
	// A/D変換 電源On, A/D Clock = 1/64 ck
	ADCSRA = 0b10000110;
	// AINに対するデジタル入力禁止.
	DIDR0  = 0b00000001;
}

/**********************************************************************
 *	A/D変換の実行.
 **********************************************************************
 */
int adc_exec(void)
{
	uchar c;
	uint  d;
	ADCSRA |= 0b01000000;	// ADSC (start)

	do {
		c = ADCSRA;
	} while(c & 0b01000000);
	//	変換終了
	d  = ADCL;
	d |=(ADCH<<8);
	return d;
}

