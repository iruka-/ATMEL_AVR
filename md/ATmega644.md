[2009-10](2009-10.md) 

## ATmega644によるV-USB実験

実験装置です。

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/AVR/m644a.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/AVR/m644a.jpg) 

~
裏面配線はあまりきれいではありません。

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/AVR/m644b.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/AVR/m644b.jpg) 

- 配線はWS#172基板互換に作ります。USB周りでは

		  ---------------------------------------
		  USB:     PB0     ===&gt; USB D-
		           PB1,PD2 ===&gt; USB D+
		  XTAL:    XTAL1,2 =&gt; Crystal 16MHz
		  ---------------------------------------
- USBへは直結ではなくて75Ωを挟みます。
- PB1とPD2を接続しないとINT0割り込みが発生しませんので動作しません。
- 「USB D-」は2kΩで5VにPullUpします。

<!-- dummy comment line for breaking list -->

mega644のfuseはこんな感じです。
	Low: 11110111
	     ||||++++-- CKSEL[3:0] システムクロック選択
	     ||++-- SUT[1:0] 起動時間
	     |+-- CKOUT (0:CLKOにシステムクロックを出力)
	     +-- CKDIV8 クロック分周初期値 (1:1/1, 0:1/8)
	
	High:11-11011  (ブートローダーを使用しないで直接焼く場合)  
	High:11-11010  (ブートローダーを使用する場合)
	     |||||||+-- BOOTRST ※データシート参照
	     |||||++-- BOOTSZ[1:0] ※データシート参照
	     ||||+-- EESAVE (消去でEEPROMを 1:消去, 0:保持)
	     |||+-- WDTON (1:WDT通常動作, 0:WDT常時ON)
	     ||+-- SPIEN (1:ISP禁止, 0:ISP許可) ※Parallel時のみ
	     |+-- JTAGEN (1:JTAG禁止, 0:JTAG許可)
	     +-- OCDEN (JTAGデバッグを 1:無効, 0:有効)
	
	Ext: -----101
	          +++- BODLEVEL[2:0] (111:無し, 110:1.8V, 101:2.7V, 100:4.3V)


- ファームウェアは[ATmega88生活](ATmega88生活.md) のATmega328P対応HIDaspx：hidspx88-ws172-m328.zip

<!-- dummy comment line for breaking list -->
を使いました。(main644-16.hex)

- [ATmega88生活](ATmega88生活.md) のATmega328P対応HIDaspx：bootmon-ws172-m328.zip (main644-16.hex) のほうは、
- そのままでは動きませんでした。

<!-- dummy comment line for breaking list -->

- main.c の

		#define OPTIMIZE_SIZE 1
- を

		#define OPTIMIZE_SIZE 0
- に変更してビルドすると動きました。

<!-- dummy comment line for breaking list -->




- とりあえずありあわせのハードウェアでATmega644の動作を確認してみました。
- この基板は市販されていないようですので、WS★NAKさんに問い合わせするのはご遠慮ください。
- もともとはビデオゲーム基板ですので、ＵＳＢコネクタは刺さりません。（無理やり差し込んでいます）

