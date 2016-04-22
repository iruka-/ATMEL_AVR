[2009-03](https://github.com/iruka-/ATMEL_AVR/blob/master/web/log/2009-03.html) 



- - - -


### 続：HIDmonを改造してＡ/Ｄにする。

- - - -

概要
- HIDaspxハードウェアに抵抗１個とコンデンサ１個を追加して、簡易的にＡ/Ｄ変換を行います。

<!-- dummy comment line for breaking list -->

- 回路図

		         ATtiny2313
		          ___    ___
		RESET    [1  |__| 20] Vcc
		PD0(NC)  [2       19] PB7(SCK)
		PD1(NC)  [3       18] PB6(MISO)
		XTAL2    [4       17] PB5(MOSI)
		XTAL1    [5       16] PB4(RST)
		PD2      [6       15] PB3(BUSY LED)
		PD3      [7       14] PB2(READY LED)
		PD4      [8       13] PB1(AIN1)  &lt;-------------*--0.047uF--| GND
		PD5(PUP) [9       12] PB0(AIN0)  &lt;------------ | &lt;--[Common Probe(+)]
		GND      [10      11] PD6        &lt;-----2.2kΩ --*----[Ohm  Probe(-)]
		        ~~~~~~~~~~~                GND |------------[Volt Probe(-)]
- Ａ/Ｄ変換部のみ抜粋した回路図

		(alt.Vref)            プローブは２択.
		AIN0&gt;----------===&gt;-   &lt;-+-- &lt;--+-(+)
		                        Rin     |
		PD6&gt;--R2.2K--*-===&gt;-   &lt;-+-     Vin
		             |                  |
		AIN1&gt;--------*       +===&gt;-  &lt;--+-(-)
		             |       |
		    0.047uF ===      |
		             |       |
		            GND     GND
- 電圧測定のときはVinにプローブを接続します。
- 抵抗測定のときはRinにプローブを接続します。

<!-- dummy comment line for breaking list -->

- 入力の電圧変動が（時間軸的に）激しい場合は、プローブ側に適当なＲＣフィルターを入れてください。

		例
		AIN0&gt;--*--R10K-----&gt;probe(+)
		       |
		      === 0.1uF
		       |
		      GND
- 入力の電圧が０〜５Ｖ以上に変動する場合はさらにクランプダイオードをAIN0手前に入れてください。

		例
		               Vcc
		                |
		                __
		                ▲ Di
		                |
		AIN0&gt;--*--------*---R10K-----&gt;probe(+)
		       |        |
		      === 0.1uF __
		       |        ▲ Di
		       |        |
		      GND      GND
- 抵抗測定時は、平滑コンデンサをプローブごと取り外してください。
    - （もしくは、測定用のピンをもうひとつ増やして、AIN0を電圧測定専用にすれば解決しますが、ファームの変更が必要です）
- 抵抗測定にはRC充電の時定数を利用しますので、測定対象の抵抗に容量成分が含まれていると測定できない場合があります。

<!-- dummy comment line for breaking list -->


仮ファームウェアとhidmon.exe公開

- [https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/hidtester.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/hidtester.zip) 

<!-- dummy comment line for breaking list -->



スクリーンショットはこんな感じ。

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/testerbmp.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/testerbmp.jpg) 

### Ａ/Ｄの精度を上げるには
- どうやら、内部基準電圧はあまりあてにならないようだ（理由は不明。USBホストによってはジッタが目立つ）
- むしろVref(Tref)を計測せずに、だいたいわかっている固定値を使ったほうが、はるかにジッタが減る傾向にある。
- 但し、Vccを3.3Vに安定化させておくと、ジッタはそれほどでもない。（あるいはUSBホストによるのかも）

<!-- dummy comment line for breaking list -->

    - ---> この件については、内部基準電圧を２回測定して後者を採用することで一応解決。
    - 但し、Tin==Trefになるような電圧を与えて、ＤＭＭ等でその電圧を測定し、monit.cの
    
    		#define	VOLT_REF	0.970
    - に値を定義する必要があります。
- USB1.1ハブなどを通した場合、あからさまにVccが落ちる(4.75V程度になる）ので、計算式を変えないと駄目。

<!-- dummy comment line for breaking list -->

- 手持ちＤＭＭの精度は一応１％未満なので、これを利用してなんらかの較正が出来ると良いな。

<!-- dummy comment line for breaking list -->

較正方法は検討中・・・

- ファーム側で、TIMER1捕獲イベント(AIN0<AIN1)が発生しないことがある（２とおり）
    - ＶＲが０Ｖになっていると常に（最初から）AIN0<AIN1であることがあり、Tin=0xffff（捕獲発生せず）になる。
    - ＶＲが５Ｖフルになっていると、AIN1よりAIN0の電圧がわずかに高くて、トリガーに到達しない。このときにTin=0xffff

<!-- dummy comment line for breaking list -->

- 区別するにはACOのビットを見ればいいはずだ。~~いまは区別していないので５Ｖ天井で０Ｖと勘違いする。~~
    - とりあえずファーム側で計測後に捕獲失敗していた場合はACOを見て判定するようにした。

<!-- dummy comment line for breaking list -->

~
~
~
- - - -
## 抵抗計の機能を追加。
![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/testerreg.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/testerreg.jpg) 


抵抗測定方法

	HIDaspx is Programmer mode.
	AVR> reg            ＜＝＝＝抵抗測定の実行（１回）
	 R =      0.353 K (Treg=   86/Tref=  536)
	AVR> reg 1000       ＜＝＝＝抵抗測定の実行（１０００回連続）
	 R =      0.365 K (Treg=   86/Tref=  519)
	AVR> reggraph       ＜＝＝＝抵抗測定の実行（グラフィカル表示）
	AVR> q

グラフ表示を終了する場合は、グラフ表示されているWindowをクリックして
フォーカスしたところで、[ESC]を押します。

グラフィカル表示は、伝統的なアナログテスターの文字盤の再現方法について
~~研究中なので、まだ表示できません。~~

- とりあえず作ってみました。

		#
- ~
- ~
- ~
- ~
- *hidmon.dllを更新しました。 [#laf2ac08]
- UsbAnalogInput() 関数を追加してあります。
- [https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/hidtester.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/hidtester.zip) 
- Ｃ言語からの呼び出しのみテスト済みです。

<!-- dummy comment line for breaking list -->



~
~
~
~



以下、初期のメモ

- - - -
## HIDmonを改造してＡ/Ｄにする。
- とりあえず、ファームだけ書いた。
- まだホスト側を書いていない。
- 配線等もまだ。
- 割り込みは使わずに、TIMER1の捕獲だけでやる。
- Pollコマンドの発行タイミングが早くて、まだ捕獲されていないときは次回のPollで取得できると思う。
    - （あるいは、ホスト側でSET_PAGEしたあとPollするまでに待ちを入れる）
- キャパシタ充電時間は~~1mS以下〜長くても数mS~~だと思われる。その場合TIMER1のカウント値は~~12000〜64000程度~~。
    - 嘘。t=RCでR=1k,C=0.047uFを仮定すると t=47uSecになり12MHzではTIMER1は564を指す。
    - そのときのVは0.63*5V=3V程度。
    - 10倍の時間が過ぎ、t=10*RCとすると、t=470uSecでTIMER1は5640。電圧は5*(1-exp(-10))=4.9997V。それでもUSBの１フレーム以下なので無問題。
    
    		#
- 精度は8bitも取れると思っていないので、もっと時定数を切り詰めてもいいのかもしれない。
- 充電電圧曲線は、

		V=V0*(1-exp(-t/RC))
- SIZE(10){... 自分では導出出来ず、Google先生に聞きました。ちなみに充電エネルギーの５０％は抵抗が熱に変換してしまうそうです。}
- なので、あとはホストＰＣ側で<math.h>をインクルードしてごにょごにょ。

<!-- dummy comment line for breaking list -->


アイディアメモ
	**********************************************************************
	*	アナログ比較器と キャパシタ充電時間計測による A/D 変換の実装.
	**********************************************************************

	*	変換回路について
	(alt.Vref)                          プローブは２択.
	AIN0>----------===>-          <-+-- <--+-
	                               Rin     | 
	PD6>--R2.2K--*-===>-          <-+-    Vin
	             |                         |
	AIN1>--------*       +===>-         <--+-
	             |       |
	    0.047uF ===      |
	             |       |
	            GND     GND

	*	  AIN1 にキャパシタとプルアップ抵抗を繋いで掃引します。
	*	  変換動作後は AIN1 を L にして キャパシタ放電します。
	*	  アナログ電圧入力 は AIN0 に接続してください。
	*
	*	A/D 変換動作について
	*  ★page_mode == 0xa0 のとき.
	*    AIN0 を 内部基準電圧入力(1.23V)に設定したのち、AIN1をアナログ入力にします。
	*    同時に、TIMER1をスタートさせます。
	*
	*  ★Pollコマンドに対する応答
	*    TIMER1捕獲カウンタ(捕獲条件：AIN1 > AIN0)の値を返します。
	*	  捕獲されていなかった場合は 0xffff を返します。
	*
	*  ★page_mode == 0xa1 のとき.
	*    AIN0 を 比較器入力に設定したのち、AIN1をアナログ入力にします。
	*    同時に、TIMER1をスタートさせます。
	*
	*  ★Pollコマンドに対する応答は page_mode == 0xa0のときと同じです。
	*

	* 抵抗測定動作について
	*	  AIN0 を出力ポートとして使い、被測定抵抗をPULLUPして掃引します。
	*	  PD6  は参照抵抗(2.2KΩ)です。
	*  ★page_mode == 0xa2 のとき.
	*    AIN0 を 内部基準電圧入力(1.23V)に設定したのち、AIN1をアナログ入力にします。
	*    同時に、TIMER1をスタートさせます。
	*    PD6はオープンにして、AIN0をHiレベルにします。
	*  抵抗測定の参照計測は ★page_mode == 0xa0 と同一です。

	*  Pollコマンドの戻りパケット構造.
	*  （ 5 bytes ）
	*  [Report ID] [ page_mode ] [ Timer1_L ] [ Timer1_H ] [ reserved ]
	*
	*/

電圧測定方法
	Vref(1.23V)のときのTrefと、
	Vin(xxx V)のときのTin をUSB経由で取得して、
	1.23V=5.0V * (1-exp(-Tref/RC)) にて、RCの積の値を得て
	Vin  =5.0V * (1-exp(-Tin/RC)) を計算すればＯＫ。  

ただし5Vの精度も影響してくると思われる。
- RCの逆算式は

		exp(-Tref/RC))=1-(1.23/5.0)=0.754
		RC = Tref/0.28236
- Tref=159カウント程度を予定。

<!-- dummy comment line for breaking list -->

もっとキャパシタをでかくするか、Rを4.7kとかにしたほうがよいかな。

