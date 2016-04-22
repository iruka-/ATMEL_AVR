/**********************************************************************
 *	PWM Sound Driver
 **********************************************************************
 * TIMER0による高速ＰＷＭ（8bit D/A）出力
 *
 *	void sound_init(void)	PWM 初期化.
 *	void sound_mix(void)	ミキシング.
 *
 *
 *
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "config.h"
#include "task.h"
#include "sound.h"
#include "sinewave.h"

#define	CH_MAX	4
#define	SUSTAIN	40			// 音量は レベル 63で始めて、40で支持モードにする.
							// 鍵盤が離されたら、40 以下を急速に減衰させる.

uint  waveptr[CH_MAX];		// sine tableの読み出しpointer.
uint  wavepitch[CH_MAX];	// waveptr に加算する⊿値.
uchar wavevol[CH_MAX];		// 音量.
uchar wavedecay[CH_MAX];	// 減衰量.

/**********************************************************************
 *	初期化
 **********************************************************************
 */
void sound_init(void)
{
	uchar i;

	SPK_OUT_DDR |= SPK_OUT_BIT_MASK;	// PD6をout.
	TCCR0A = 0b10000011;	//COM0A=比較一致でLow,高速PWM
	TCCR0B = 0b00000001;
	OCR0A  = 128;

	for(i=0;i<CH_MAX;i++) {
		wavepitch[i]=0;
		waveptr[i]=0;
	}
}

/**********************************************************************
 *	8bit D/A出力.
 **********************************************************************
 */
#if	0
static	void sound_out(uchar x)
{
	OCR0A = x;
}
#else
// timer.c 側に実装.
void sound_out(uchar x);
#endif

#ifdef	__AVR_ATtiny2313__
//
//   c * ( gain / 256 ) の粗い計算を実行.
//
//static 
char amplitude(char c , uchar gain)
{
	char val=0;
	c>>=2;if(gain & 0b1000000) val=c;
	c>>=1;if(gain & 0b100000) val+=c;
	c>>=1;if(gain & 0b10000) val+=c;
	c>>=1;if(gain & 0b1000) val+=c;
	c>>=1;if(gain & 0b100) val+=c;
	c>>=1;if(gain & 0b10) val+=c;
	c>>=1;if(gain & 0b1) val+=c;
	return val;
}
#endif

/**********************************************************************
 *	チャネルミキシング.
 **********************************************************************
 */
static	void sound_mix(void)
{
	uchar i,p;
	char  out=0;
	char  c;
	int   v;
	for(i=0;i<CH_MAX;i++) {
		uchar off=wavevol[i];
		waveptr[i] += wavepitch[i];		//ピッチレジスタを加算.
		p = (waveptr[i] >> 8) & 0x3f;	//sine wave tableの読み出しポインタ.
#if	0
		p += wave_offset;				//フェーズを掛けてみる.
#else
		if(off < 30) off = 30;
		p += (off-30);					//音質を揺らす.
#endif
		c = __LPM( &sinewave[p] );
#ifdef	__AVR_ATtiny2313__
		v = amplitude(c , wavevol[i]);	//音量を乗算.
		c = v ;
#else
		v = ((c * wavevol[i]) >> 8);	//音量を乗算.
		c = v ;
#endif
		out += c;						//ミキシング.
	}
	sound_out(out + 0x80);
}

/**********************************************************************
 *	発振音を鳴らすタスク.
 **********************************************************************
 */
void sound_task(void)
{
	sound_mix();
}

/**********************************************************************
 *	発振音を鳴らす.
 **********************************************************************
 *	ch : (0..3)    発音チャネル.
 *	p  : (0..2047) ピッチ定数.
 *	v  : (0..255)  音量
 */
void sound_play(uchar ch,uint p,uchar v)
{
	if(ch<CH_MAX) {
		wavepitch[ch]=p;
		wavevol[ch]=v;
		wavedecay[ch]=1;
	}
}

/**********************************************************************
 *	発振音の減衰量を指定する.
 **********************************************************************
 *	p  : (0..2047) ピッチ定数.
 *	d  : tickあたりの減衰量.
 */
void sound_setdecay(uint p,uchar d)
{
	uchar i;
	for(i=0;i<CH_MAX;i++) {		//全ch検索しているのはダサいかも.
		if( wavepitch[i]==p ) {
			wavedecay[i]=d;
		}
	}
}
/**********************************************************************
 *	発振音を減衰させる. 定期的に呼ぶこと.
 **********************************************************************
 */
void sound_decay()
{
	static uchar n=0;
	static uchar m=0;
	uchar i,d;
	char v;

	n--;
	if(n & 0x1f) return;

	for(i=0;i<CH_MAX;i++) {
		v = wavevol[i];								//	現在の音量.
		if( v ) {
			d = wavedecay[i];						//	減衰量.
			if((d==1) && (v == SUSTAIN)) continue;	//  サスティンレベルで保持したい.
			v -= d;									//  減衰処理は、実はこれだけ.
			if(v<0) v = 0;							//  音量を負にしたら大音量に戻るので0にする.
			wavevol[i]=v;							//  書き戻し.
		}
	}
#if	0
	// mは鋸波,wave_offsetは三角波.
	m++; m &= 0x7f;
	if(m>=64) wave_offset = 128-m;
	else      wave_offset = m;
#endif
//	if(wave_offset==0) wave_offset = 1;

//	wave_offset += 128;
}
/**********************************************************************
 *	発振音を全部止める.
 **********************************************************************
 */
void sound_stop_all()
{
	uchar i;
	for(i=0;i<CH_MAX;i++) {
		wavepitch[i]=0;
		waveptr[i]=0;
		wavevol[i]=0;
		wavedecay[i]=1;
	}
}
/**********************************************************************
 *	
 **********************************************************************
 */
