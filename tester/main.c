/**********************************************************************
	時計
 **********************************************************************
	
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


#define	R220_OFFSET	22			// AVRのPULLUP 内部抵抗 と 220Ωの抵抗誤差の和Ω.
#define	R1K_OFFSET	22			// AVRのPULLUP 内部抵抗 と  1KΩの抵抗誤差の和Ω.
#define	R10K_OFFSET	150			// AVRのPULLUP 内部抵抗 と 10KΩの抵抗誤差の和Ω.

#define	PULLUP_OFFSET	4		// AVRのPULLUP 電圧と Vccとの誤差digit (5V/1024)
#define	PROBE_OFFSET	1		// プローブ線の内部抵抗Ω.

#define	TEST			0
#define	EVERY_SEC_TEST	0
#define	EVERY_SEC		1

/**********************************************************************
 *	define
 **********************************************************************
 */
void quit(void);				// bootloaderに戻る.
void led_task(void);			// LED点灯ドライバー
void led_set_number(int data);	// LED表示データのセット.
void led_set_number2(int hh,int mm);	// LED表示データのセット.
void led_set_pattern(uchar *data);// LED表示パターンのセット.
void led_set_comma(uchar digit);// 小数位置を2桁目に合わせる.
void led_set_zero_supress(void);// 上位桁のゼロを消す.
void wait_usec(uchar n);
/**********************************************************************
 *	work
 **********************************************************************
 */
#define		   MAX_SAMPLE		16
#define		   MAX_SAMPLE_SFT	4
uint  ain_data[MAX_SAMPLE];
uchar ain_cnt = 0;
uint  ain_val;		// 平均値.

ulong Rval;			// 抵抗値. 単位はΩ.
uint  r_val;		// 抵抗値. (4桁以内整数)
uchar r_exp;		// 抵抗値 小数点位置.
uchar r_mode;		// 抵抗測定:現在のPULL-UPモード(0=220,1=1K,2=10K)
uchar v_mode;		// 0=Ω測定 / 1=電圧測定 / 255=起動直後.
uchar v_cnt0;

enum {
	MODE_R220=0,
	MODE_R1K =1,
	MODE_R10K=2,
	MODE_VOLT=3,
};

#define	PIN_R220	1
#define	PIN_R1K		6
#define	PIN_R10K	7

/**********************************************************************
 *	A/D変換の実行.
 **********************************************************************
 */
static	int adc_exec(void)
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

/**********************************************************************
 *	A/D変換 task.
 **********************************************************************
 */
static	void adc_task(void)
{
	uint d = adc_exec();
	ain_data[ain_cnt] = d;
	ain_cnt++;
	if(	ain_cnt >= MAX_SAMPLE) {
		ain_cnt = 0;
	}
}

/**********************************************************************
 *	PORTD の指定bit を Hi レベル出力にする.
 **********************************************************************
 */
static	void set_HiD(uchar bit)
{
	uchar d = 1<<bit;
	PORTD |=  d;			// Hi
	DDRD  |=  d;			// PDx: H
}
/**********************************************************************
 *	PORTD の指定bit を オープン にする.
 **********************************************************************
 */
static	void set_HiZ(uchar bit)
{
	uchar d = ~(1<<bit);
	PORTD &= d;				//      Lo
	DDRD  &= d;				// PDx: Hi-Z
}

/**********************************************************************
 *	PULL UP モードを切り替える.
 **********************************************************************
 */
static	void set_pullup_mode(uchar mode)
{
	uchar pintbl[4]={PIN_R220,PIN_R1K,PIN_R10K,PIN_R10K};
	uchar i;

	r_mode = mode;

	for(i=0;i<3;i++) {
		if(i==mode) {
			set_HiD(pintbl[i]);
		}else{
			set_HiZ(pintbl[i]);
		}
	}
}

int get_pullup_R(void)
{
	uint regtbl[3]={
		  220+R220_OFFSET,
		 1000+R1K_OFFSET,
		10000+R10K_OFFSET,
	};
	return regtbl[r_mode];
}
/**********************************************************************
 *	PULL UP モードを自動で切り替える.
 **********************************************************************
 */
#define	R10000 5000
#define	R1000  500
static void change_mode(void)
{
	uchar mode;
	if(r_exp == 255)   {mode = MODE_R10K; }else 
	if(Rval >= R10000) {mode = MODE_R10K; }else 
	if(Rval >= R1000 ) {mode = MODE_R1K;  }else 
    { mode = MODE_R220; }

	if(r_mode != mode) {
		set_pullup_mode(mode);
	}
}
/**********************************************************************
 *	Ａ/Ｄ 入力の平均を取る.
 **********************************************************************
 */
static void measure(void)
{
	uint d=0;
	uchar i;
	for(i=0;i<MAX_SAMPLE;i++ ) {
		d += ain_data[i];
	}
	d >>= MAX_SAMPLE_SFT;	// 16で割る.
	ain_val = d;
}
/**********************************************************************
 *	Ａ/Ｄ 入力の平均を取る.
 **********************************************************************
 */
static void measure_volt(void)
{
	Rval = ain_val;
	Rval = (500 * Rval ) / 1024;	// 5.000 V
	r_val = Rval;
	r_exp = 2;
}
/**********************************************************************
 *	抵抗値計算.
 **********************************************************************

                     Vin
	Rval = Rref * ---------
                  Vcc - Vin
 */
static void calculate(void)
{
	ulong Vin;
	ulong Vcc;
	ulong Rref;

	Rref = get_pullup_R();
	if( ain_val >=(1023 - PULLUP_OFFSET )) {
		r_val = 9999;
		r_exp = 255;
		return ;
	}
	Vin =    ain_val;
	Vcc = (1023 - PULLUP_OFFSET ) - ain_val ;

	Rval = ( Vin * Rref ) / Vcc;
	if(	Rval>=PROBE_OFFSET) {
		Rval-=PROBE_OFFSET;
	}

	r_exp = 0;				//まず、小数点は 0 桁目とする.
	if(Rval < 1000) {		// 1kΩ未満.
		r_exp = 3;
		r_val = Rval;
		return;
	}
	
	if(Rval < 10000) {		// 10kΩ未満.
		r_exp = 2;
		r_val = (Rval/10);
		return;
	}
	
	if(Rval < 100000) {		// 100kΩ未満.
		r_exp = 1;
		r_val = (Rval/100);
		return;
	}
	
	if(Rval < 1000000) {		// 1000kΩ未満.
		r_exp = 0;
		r_val = (Rval/1000);
		return;
	}
	
		r_val = 9999;
		r_exp = 255;
		return ;
}
/**********************************************************************
 *	result		結果表示.
 **********************************************************************
 */
static void disp_result()
{
	static	uchar minus_data[4]={
		0b01000000,	// -
		0b01000000,	// -
		0b01000000,	// -
		0b01000000,	// -
	};

	if(r_exp==255) {
		led_set_pattern(minus_data);
		return;
	}

	led_set_number(r_val);	// LEDに設定する.
	led_set_comma(r_exp);	// 小数位置を2桁目に合わせる.
	led_set_zero_supress();	// 上位桁のゼロを消す.
}
/**********************************************************************
 *	task
 **********************************************************************
 */
static void volt_task(void)
{
	measure();
	measure_volt();
	disp_result();	//結果をLEDにセット.
	sleep_tsk(48);	//128 mS休む.
}
/**********************************************************************
 *	電圧モードへ移行?
 **********************************************************************
 */
static void check_volt_mode(void)
{
	if(	v_mode == 255) {	//モード未定?
		v_cnt0++;
		if(v_cnt0==3) {		//起動後３回目の計測.
			if(r_exp != 3) {	// 1kΩ未満でない.
				v_mode = 0;		// ohm_mode に固定;
			}else{				// 1kΩ以上.
				v_mode = 1;		// volt_modeに固定;
				set_pullup_mode(MODE_VOLT);		// ohm測定pullupを外す.
			}
		}
	}
}
/**********************************************************************
 *	task
 **********************************************************************
 */
static void ohm_task(void)
{
	measure();
	calculate();
	disp_result();	//結果をLEDにセット.

	change_mode();
	check_volt_mode();
	sleep_tsk(48);	//128 mS休む.
}

static char mod10(char n)
{
	while(n>=10) {
		n -= 10;
	}
	return n;
}

/**********************************************************************
 *	task
 **********************************************************************
 */
static void main_task(void)
{
//	led_set_number2(time_hh,time_mm);	// LEDに設定する.
	led_set_number2(time_mm,time_ss);	// LEDに設定する.

	if(time_tick < (2000/2)) {
		led_set_comma(2);	// 小数位置を2桁目に合わせる.
	}

}
/**********************************************************************
 *	task
 **********************************************************************
 */
static void sound_task(void)
{
	static uchar beep0;
	uchar beep=0;
	
	if(beep0) {
		PIND  = 0b00000001;			// PD1: flip
	}

#if	TEST
	// 10秒
	if(mod10(time_ss) == 0) {
#else
	// 10分
	if((time_ss == 0) && (mod10(time_mm)==0)) {
#endif
		if(time_mm) {
			if(time_tick & 0x100) {
				beep=1;
			}
		}
	}


#if	EVERY_SEC_TEST
	//毎秒
//	if((time_ss == 0)) 
	if(time_tick <2) {
			beep=1;
	}
#endif

#if	EVERY_SEC
	//毎秒
	if((time_mm == 0)&&(time_ss < 5)) 
	if(time_tick <2) {
			beep=1;
	}
#endif

	if((beep0==0) && (beep!=0)) {
		DDRD |= 0b00000001;			// PD0: out
	}
	if((beep0!=0) && (beep==0)) {
		DDRD &= ~0b00000001;			// PD0: in
		PIND &= ~0b00000001;			// PD0: LO
	}

	beep0=beep;
}

/**********************************************************************
 *	task
 **********************************************************************
 */
static void task_regist(void)
{
//	reg_tsk(ID_adc, adc_task);
	reg_tsk(ID_adc ,sound_task);
	reg_tsk(ID_main,main_task);
	reg_tsk(ID_led, led_task);
}

/**********************************************************************
 *	
 **********************************************************************
 */
static void port_init(void)
{
	//ポート方向レジスタの初期化.
	PORTC=0;
	DDRC =0b11111110;		// PC0 == AIN0

	PORTD=0b00110000;		// PD7,PD6,PD1 == R10K,R1K,R220
	DDRD =0b00110000;

	PORTB=0;
	DDRB =0b00111111;

	// A/D変換 MUXレジスタ設定.
	ADMUX = 0b00000000;
	// A/D変換 電源On, A/D Clock = 1/64 ck
	ADCSRA = 0b10000110;
	// AINに対するデジタル入力禁止.
	DIDR0  = 0b00000001;


#if	0	// 比較器を使用する初期化.
	//A/D変換用レジスタ設定.
	ADCSRA = 0b1000_0000;
	//アナログ比較器初期化.
	ADCSRB = 0b0000_0000;
	//基準電圧1.1Vと比較 / Timer1捕獲On. / 立下りエッジでの割り込み.
	ACSR   = 0b0100_0110;
	//AIN1に対するデジタル入力禁止.
	DIDR1  = 0b0000_0010;
#endif


}
/**********************************************************************
 *	メイン
 **********************************************************************
 */
int	main(void)
{
	port_init();
						// 以下、20MHz時の周期:
//	timer_init(41666-1);// 2083μS(480.0  Hz)周期で割り込み.
	timer_init(10000-1);//  500μS (2 KHz)周期で割り込み.
//	timer_init(256-1);	// 12.8μS(78.125kHz)周期で割り込み.
	sei();

	task_regist();
	set_pullup_mode(MODE_R1K);
	v_mode = 255;		// モード未定.
	v_cnt0 = 0;
	do {
		timer_wait();
		dispatch_tsk();
	}while(1);

	return 0;
}

/**********************************************************************
 *	
 **********************************************************************
 */
