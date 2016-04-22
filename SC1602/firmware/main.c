/**********************************************************************
 *	PS2 Keyboard Library : Sample Main
 **********************************************************************
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "config.h"
#include "task.h"
#include "taskdef.h"
#include "timer.h"
#include "sound.h"

#define	LUX64	64	//00
#define	LUX8	8	//00

// PC4
#define	EN		0x10	//PC4	ENABLE=1

// PD0:1
#define	REGSEL	0x01	//PD0	REG=0/DATA=1
#define	RW		0x02	//PD1	R=1 / W=0
// PD3:4
#define	LED1	0x08	//PD3
#define	LED2	0x10	//PD4
// PD5
#define	BOOTSW	0x20	//PD5


#define	P_MIN	(15*60)
#define	P_MAX	(30*60)

uint  lux;
uchar l_cnt;	// 1/10 秒単位に+1する. １秒たったら0に戻す.
uchar l_on;		// LEDをONにするなら1
uint  p_min;	// 


void adc_init(void);
int  adc_exec(void);
/**********************************************************************
 *	
 **********************************************************************
 */
void delay8ck(char n);
/**********************************************************************
 *	
 **********************************************************************
 */
void wait_us(uchar us)
{
	while(us) {
		delay8ck(2);
		us--;
	}
}
void wait_ms(uchar ms)
{
	while(ms) {
		wait_us(250);
		wait_us(250);
		wait_us(250);
		wait_us(250);
		ms--;
	}
}

/**********************************************************************
 *	
 **********************************************************************
 */
uchar get_bootsw(void)
{
	return PIND & BOOTSW;
}
/**********************************************************************
 *	
 **********************************************************************
 */
void LCD_regsel(uchar regsel)
{
	if(regsel) {
		PORTD |=  REGSEL;	// DATA
	}else{
		PORTD &= ~REGSEL;	// REG
	}
}
/**********************************************************************
 *	1=read 0=write
 **********************************************************************
 */
void LCD_rwsel(uchar read)
{
	if(read) {
		PORTD |=  RW;	// R
	}else{
		PORTD &= ~RW;	// W
	}
}

/**********************************************************************
 *	
 **********************************************************************
 */
void LED1_sel(uchar on)
{
	if(on) {
		PORTD |=  LED1;
	}else{
		PORTD &= ~LED1;
	}
}

/**********************************************************************
 *	
 **********************************************************************
 */
void LED2_sel(uchar on)
{
	if(on) {
		PORTD |=  LED2;
	}else{
		PORTD &= ~LED2;
	}
}

/**********************************************************************
 *	
 **********************************************************************
 */
void LCD_cmd4(uchar cmd)
{
	uchar p;

	cmd &= 0x0f;		// cmdは下位4bitのみ有効.
	
	LCD_rwsel(0);		// RW=W(0)
	// 4bitをPC0:3に書き込む.
	p = PORTC;
		p &= ~(EN);		// E=0
		p &= 0xf0;
		p |= cmd;		// digit=cmd
	PORTC = p;
	
	delay8ck(1);

	{
		p |= EN;		// E=1
		PORTC = p;
		delay8ck(1);	// > 0.22uSec
		p &= ~EN;		// E=0
		PORTC = p;
	}
	delay8ck(1);

	p &= 0xf0;			// digit=0;
	PORTC = p;
	
	wait_us(100);	// 37uS以上待つ.
}

/**********************************************************************
 *	
 **********************************************************************
 */
void LCD_cmd8(uchar cmd)
{
	LCD_cmd4(cmd>>4);
	LCD_cmd4(cmd   );
}

void LCD_wait(uchar wait)
{
	if(wait) {
		wait_ms(wait);
	}
}

/**********************************************************************
 *	
 **********************************************************************
 */
void LCD_cls()
{
	LCD_regsel(0);		// REG
	LCD_cmd8(0x01);		// Clear		  0000_0001
//	Clear は1.52mS以上待つ.
	LCD_wait(2);
}


/**********************************************************************
 *	
 **********************************************************************
 */
void LCD_init()
{
	LCD_regsel(0);		// REG
//	4bit インターフェースに切り替えるために、3回、8bit指定する.
	LCD_cmd4(0x03);	// Function Set 001<DL>_NFxx	DL:1=8bit
	LCD_wait(5);
	LCD_cmd4(0x03);	// Function Set 001<DL>_NFxx
	LCD_wait(1);
	LCD_cmd4(0x03);	// Function Set 001<DL>_NFxx

//	8bitモードで、4bit指定を実行する.
	LCD_cmd4(0x02);	// Function Set 001<DL>_NFxx	DL:0=4bit

//	これ以降は4bitモードになっている.
	LCD_cmd8(0x28);	// Function Set 001<DL>_NFxx	DL:0=4bit, NLine:1=2Line, Font:0=5x8dot
	LCD_cls();
	LCD_cmd8(0x06);	// Entry Mode Set 0000_01DS D:0=inc S:0=NoShift
	LCD_cmd8(0x0c);	// On			  0000_1DCB	DispOn CursOff BlinkOff
}

/**********************************************************************
 *	
 **********************************************************************
 */
void LCD_putc(uchar c)
{
	LCD_regsel(1);		// DATA
	LCD_cmd8(c);
}

/**********************************************************************
 *	
 **********************************************************************
 */
void LCD_puts(char *s)
{
	while(*s) {
		LCD_putc(*s++);
	}
}

/**********************************************************************
 *	
 **********************************************************************
 */
void LCD_locate(uchar x,uchar y)
{
	uchar c = 0x80 | x;
	if(y) c|= 0x40;

	LCD_regsel(0);		// REG
	LCD_cmd8(c);
}


/**********************************************************************
 *	
 **********************************************************************
 */
void print_chr(uchar c)
{
	LCD_putc(c);
}

void putnum1(uchar num)
{
	print_chr('0'+num);
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
/**********************************************************************
 *	
 **********************************************************************
 */
void print_dec2(uchar c)
{
	putnum2(c);
}
/**********************************************************************
 *	
 **********************************************************************
 */
uint div10(uint v,uchar *mod)
{
	uint ret = v/10;
	*mod = v - (ret*10);	//余り.
	return ret;
}
/**********************************************************************
 *	
 **********************************************************************
 */
void print_dec5(uint v)
{
	static char buf[5];
	uchar i,m;
	for(i=0;i<5;i++) {
		v=div10(v,&m);
		buf[4-i]=m;
	}
	for(i=0;i<5;i++) {
		putnum1(buf[i]);
	}
}

/**********************************************************************
 *	
 **********************************************************************
extern	Time cur;
	uchar sec;		// 0..59
	uchar minute;	// 0..59
	uchar hour;		// 0..23
 */
void print_time()
{
	LCD_locate(0,0);
	print_dec2(cur.hour);print_chr(':');
	print_dec2(cur.minute);print_chr(':');
	print_dec2(cur.sec);

//	print_chr('.');
//	int msec = cur.msec * 4;
//	print_dec2(msec/10);

}


/**********************************************************************
 *	毎秒処理.
 **********************************************************************
 */
void LED_off()
{
	LED1_sel(0);
	LED2_sel(0);
	l_on=0;
}
/**********************************************************************
 *	毎秒処理.
 **********************************************************************
 */
void LED_mute()
{
	LED1_sel(1);
	LED2_sel(0);
	l_on=1;
}
/**********************************************************************
 *	毎秒処理.
 **********************************************************************
 */
void LED_on()
{
	if(l_on!=2) {
		LED_mute();
		p_min=0;
		l_on=2;
	}else{	//暗い状態の継続.
		p_min++;
		if(p_min >= P_MIN) {
			LED1_sel(0);
			LED2_sel(1);
		}
		if(p_min >= P_MAX) {
			LED_mute();
			p_min=0;
		}
	}
}
/**********************************************************************
 *	毎秒処理.
 **********************************************************************
 */
void LED_control()
{
	if(lux>=LUX64) {	//かなり明るい.
		LED_off();
		return;
	}
	if(lux>=LUX8) {	//少し明るい.
		LED_mute();
		return;
	}
		LED_on();	//全く暗い.
}

/**********************************************************************
 *	毎秒処理.
 **********************************************************************
 */
void SW_task()
{
	static	uchar oldsw=0;
	
	uchar sw=get_bootsw();

	// TRIGGER?
	if( (oldsw!=0) && (sw==0) ) {
		p_min = P_MIN;
	}
	oldsw = sw;
}
/**********************************************************************
 *	task
 **********************************************************************
 */
void LED_control_tick(int v)
{
	if(	l_cnt>=10) {
		LED_control();
		SW_task();
		l_cnt=0;
		lux=0;
	}else{
		lux += v;
		l_cnt++;
	}
}
/**********************************************************************
 *	task
 **********************************************************************
 */
void main_task(void)
{
	uchar led1;
	uchar led2;
	print_time();

	int v = adc_exec();
	LCD_locate(0,1);
	print_dec5(v);
	print_chr(' ');
	print_dec5(p_min);


	led1 = PORTD & LED1;if(led1!=0) led1=1;
	led2 = PORTD & LED2;if(led2!=0) led2=1;

	LCD_locate(12,1);
	putnum1(led1);
	putnum1(led2);


	if(v>=256) v=255;
	LED_control_tick(v);

	sleep_tsk(25);
}


void task_regist(void)
{
	init_tsk();
	reg_tsk(ID_main ,main_task);
//	reg_tsk(ID_sub  ,sub_task);
}

static	void led_blink(uchar c)
{
	uchar x=0;
	DDRC|=0x03;

	do {
		PORTC=x;x++;
		wait_ms(250);
	}while(--c);
}
/*
---------------------------------
フラットケーブル配線
信号	AVR	色
------- --- --
REG_SEL	PD0	赤

RW		PC5	白
E		PC4	黒
DB7		PC3	赤
DB6		PC2	茶
DB5		PC1	黄
DB4		PC0	橙
------- --- --
*/
/**********************************************************************
 *	
 **********************************************************************
 */
void port_init()
{
	PORTD&= ~RW;		// PD1:R/W=W(0)
	PORTD|=0x01;		// PD0:REGSEL=DATA(1)
	PORTC|=0x1f;		// All High.
	PORTC&= ~(EN);		// E=0
	LED1_sel(0);
	LED2_sel(0);

	DDRD|=0x03;			// PD0:1 out
	DDRC|=0x1f;			// PC0:4 out

	wait_ms(50);
}
/**********************************************************************
 *	メイン
 **********************************************************************
 */
int	main(void)
{
	port_init();
	adc_init();
						// 以下、20MHz時の周期:
//	timer_init(400-1);	// 20  μS(50    kHz)周期で割り込み.
//	timer_init(16000-1);// 1kHz  周期で割り込み.
	timer_init(64000-1);// 250Hz 周期で割り込み.
//	割込周期 = CLK(16000000) / 分周

	LCD_init();
#if	0
	LCD_puts("Hello World.");
	LCD_locate(2,1);
	LCD_puts("\xb1\xb2\xb3");
	led_blink(132);
#endif
	sei();
	task_regist();

	while(1) {
		timer_wait();
		dispatch_tsk();
	}

	return 0;
}

/**********************************************************************
 *	
 **********************************************************************
 */
