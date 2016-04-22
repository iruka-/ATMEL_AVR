                                                               2008-11-28
                       キーボードマニア・ Part III
                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~

■ 概要

  本プログラムは、ＩＢＭ-ＰＣ互換機用に広く使用されているPS/2仕様の
キーボードデバイスをＡＴＭＥＬ　ＡＶＲマイコンに接続して、入力機器
として使用することを目的としています。

  一応汎用的な雛型を目指していますが、汎用性の無いところはご容赦ください。


■ 使い方／テスト方法

  ＨＥＸファイル、part3/main88-12.hex をATmega88 に焼きます。
  ATmega88は、下記配線図を参考に配線してください。
  電源（５Ｖ）を与え、接続先のキーボードを押すと、スピーカーから
　何か音が出たら正常です。

■ 配線図
                          ATmega88/168
                         ___    ___
    (PCINT14/_RESET)PC6 [1  |__| 28] PC5(ADC5/SCL/PCINT13) 
       (PCINT16/RxD)PD0 [2       27] PC4(ADC4/SDA/PCINT12)
       (PCINT17/TxD)PD1 [3       26] PC3(ADC3/PCINT11)
      (PCINT18/INT0)PD2 [4       25] PC2(ADC2/PCINT10)
 (PCINT19/OC2B/INT1)PD3 [5       24] PC1(ADC1/PCINT9)  ======> PS/2 keyb DATA
    (PCINT20/XCK/T0)PD4 [6       23] PC0(ADC0/PCINT8)  ======> PS/2 keyb CLK
                    Vcc [7       22] GND
                    GND [8       21] AREF
(PCINT6/XTAL1/TOSC1)PB6 [9       20] AVcc
(PCINT7/XTAL2/TOSC2)PB7 [10      19] PB5(SCK/PCINT5)
   (PCINT21/OC0B/T1)PD5 [11      18] PB4(MISO/PCINT3)
 (PCINT22/OC0A/AIN0)PD6 [12      17] PB3(MOSI/OC2A/PCINT3)
      (PCINT23/AIN1)PD7 [13      16] PB2(_SS/OC1B/PCINT2)
  (PCINT0/CLKO/ICP1)PB0 [14      15] PB1(OC1A/PCINT1)
                         ~~~~~~~~~~

   ---------------------------------------
   XTAL:    XTAL1,2 => Crystal 12MHz
            PC0 =====> PS/2 keyb CLK  (PCINT8)
            PC1 =====> PS/2 keyb DATA
            PD6 =====> Sound Output(PWM)
            PD7 =====> CW Output(DC Level,L=MAKE H=BRAKE)
   ---------------------------------------

■ FUSE

############################## ATMega88/168 ##############################
# Fuse Low byte: (0xFF)
#Low: 11111111
#     ||||++++-- CKSEL[3:0] システムクロック選択
#     ||++-- SUT[1:0] 起動時間
#     |+-- CKOUT (0:PB0にシステムクロックを出力)
#     +-- CKDIV8 クロック分周初期値 (1:1/1, 0:1/8)
#
# Fuse High byte: (0xDD)
#High:11-11101 
#     |||||+++-- BODLEVEL[2:0] (111:無, 110:1.8V, 101:2.7V, 100:4.3V)
#     ||||+-- EESAVE (消去でEEPROMを 1:消去, 0:保持)
#     |||+-- WDTON (1:WDT通常動作, 0:WDT常時ON)
#     ||+-- SPIEN (1:ISP禁止, 0:ISP許可) ※Parallel時のみ
#     |+-- DWEN (On-Chipデバッグ 1:無効, 0:有効)
#     +-- RSTDISBL (RESETピン 1:有効, 0:無効(PC6))
#
# Fuse Extend byte: (0xF8)
#Ext: -----001
#          ||+-- BOOTRST ※データシート参照 (1=応用領域、0=ブートローダー)
#          ++-- BOOTSZ[1:0] ※データシート参照
#
###############################################################################

■ CLOCK

  必要ならば、firmware/Makefile 内の記述を変更してください。

	#F_CPU = 12000000
	F_CPU = 20000000

■ 開発のヒント

  AVR-USBのリファレンスプロジェクトである、bootloadHID または、bootloadHID に
  モニタ機能を付加した拙作 bootmon を用いると、ＡＶＲライタを使用することなく
  USB経由でプログラムの更新と実行を行うことが出来るのでコンパイル＆ＲＵＮの
　繰り返しが楽に出来るようになります。お試しください。


■ ディレクトリ構成

   keybrd-mania --+-- part1/ ＰＣ８８０１mkIIＳＲキーボード補完計画(PIC16F84A)
                  |
                  +-- part2/ キーボードまにあ][ (PIC16F84A)
                  |            (林檎用キーボードをＩＢＭ互換機に接続して使おう)
                  +-- part3/ 汎用 PS2 キーボードライブラリ (ATmega88)
                  |
                  +-- part4/ 汎用 PS2 キーボードライブラリ (ATtiny2313)

   注意：part1,part2 はほぼ２０世紀の遺物です。(一部2001年正月のtimestampあり)


■ 無保証・免責条項

  改造して（あるいは適当なフレームワークとして）ご自由にお使いください。
  商用、非商用は問いません。

  但し、このプログラムを運用することによって発生したいかなる損害についても、
  原作者および改良者はその責を負わないものとさせていただきす。
  この条件に同意できない方は使用を許可しないものとさせていただきます。


■ 参考文献

  ＡＴＭＥＬ社のアプリケーションノート３１３番を参照させて頂きました。



###### EOF ######
