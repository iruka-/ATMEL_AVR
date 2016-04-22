                                                             2008年11月21日

        USB接続方式のドライバインストール不要なAVRライタ（HIDaspx）
        ￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣￣

■ このアーカイブは、senshuさんが中心となって開発しているHIDaspxを
   ATmega88 に対応させたものです。


■ 現在の問題点/ＢＵＧ情報

   デバイス認識(ispConnect)が不完全なため、ターゲットAVRを認識しないことがあります。

   書き込みデバイスのMISO,MOSI,SCKが出力モードで衝突する場合、書き込めないことがあります。

   ISPクロックはハードウェアクロックしか実装していないため、低速ターゲットに対応していません。
    -d0 | fOSC/4  ... 3MHz
    -d1 | fOSC/8  ... 1.5MHz
    -d2 | fOSC/16 ... 750kHz
    -d3 | fOSC/32 ... 375kHz
    -d4 | fOSC/64 ... 187kHz
    -d5 | fOSC/64 ... 187kHz
    -d6 | fOSC/128...  93kHz

    -d7以降は-d6と同じです.  d4とd5が同じ分周比になっていますが、これもハードウェアの仕様です。



■ 配線
                          ATmega88/168
                         ___    ___
    (PCINT14/_RESET)PC6 [1  |__| 28] PC5(ADC5/SCL/PCINT13)
       (PCINT16/RxD)PD0 [2       27] PC4(ADC4/SDA/PCINT12)
       (PCINT17/TxD)PD1 [3       26] PC3(ADC3/PCINT11)
      (PCINT18/INT0)PD2 [4       25] PC2(ADC2/PCINT10)
 (PCINT19/OC2B/INT1)PD3 [5       24] PC1(ADC1/PCINT9)
    (PCINT20/XCK/T0)PD4 [6       23] PC0(ADC0/PCINT8)
                    Vcc [7       22] GND
                    GND [8       21] AREF
(PCINT6/XTAL1/TOSC1)PB6 [9       20] AVcc                          書き込み側AVR-chip
(PCINT7/XTAL2/TOSC2)PB7 [10      19] PB5(SCK/PCINT5)      -------> SCK
   (PCINT21/OC0B/T1)PD5 [11      18] PB4(MISO/PCINT3)     -------> MISO
 (PCINT22/OC0A/AIN0)PD6 [12      17] PB3(MOSI/OC2A/PCINT3)-------> MOSI
      (PCINT23/AIN1)PD7 [13      16] PB2(_SS/OC1B/PCINT2) -------> RESET
  (PCINT0/CLKO/ICP1)PB0 [14      15] PB1(OC1A/PCINT1)
                         ~~~~~~~~~~
   ---------------------------------------
   USB:     PB0     ===> USB D-
            PB1,PD2 ===> USB D+
   XTAL:    XTAL1,2 => Crystal 12MHz
   ---------------------------------------

   ターゲットとのMISO,MOSIはクロス接続の必要はありません。ストレートでお願いします。

   ターゲットと接続する４本のラインは100Ω程度の抵抗を通して接続するほうが安全です。

   USB D+,D-は75Ω程度の抵抗を通してPD2,PD3とそれぞれ接続し、さらにD-は1.5kΩを通して
   プルアップする必要があります。

   AVcc,AREFは適切に処理してください。
   (Vccから低域濾波器を通してAVccに接続します。)

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
#          ||+-- BOOTRST ※データシート参照
#          ++-- BOOTSZ[1:0] ※データシート参照
#
###############################################################################

■ CLOCK

	F_CPU = 12000000 で、お願いします。

#################
2009-0206追記:

・wsNak #172基板に対応しました。

使い方：
（１）bootmon-ws172 を使用して、ATmega88の応用領域にHIDaspxファームを書き込んでください。
      例：
      C:hidspx-ws172> bootloadhid.exe firmware\main.hex
（２）起動のさせ方は、少し面倒ですが、タクトスイッチの'L'を押しながらＵＳＢのケーブル
　　　を接続します。PWR/BUSYのＬＥＤが点滅を始めたら、HIDaspxファームが起動した証拠です。
（３）自己プログラミング用のJP2 (1-2) ジャンパーが接続されていたら、外しておきます。
（４）適当なプログラム対象AVRマイコンを接続して、src/hidspx-gcc.exe を起動します。
      例：
      C:hidspx-ws172/src> hidspx.exe -r
（５）AVRマイコンの種類が表示出来たら成功です。

-----------------

注意点：

・ＨＷ ＳＰＩにしか対応していませんので、クロックの遅いAVRチップを認識できません。
・bootmonの支配下から、HIDaspxに切り替えるためにタクトスイッチ'L'を押しても、
　パソコンがUSBの切り替え（ファームの差し替え）を認識しませんので、hidspx-gcc.exe は
　うまく動いてくれません。
　対処法としては、Windowsのデバイスマネージャーに現れている当該HIDデバイス
　　ヒューマンインターフェースデバイス
　　  |
      +-- USBヒューマンインターフェースデバイス ＜＝これ
　をマウス右クリックで一度無効にし、再び有効にすると、AVRの電源は入ったままでケーブル
　を抜き差ししたのと同じ効果が得られるのでhidspx-gcc.exeが使えるようになります。

　	（このままでは非常に不便なので、改良予定です）

・別の方法としては、タクトスイッチ'L'をジャンパーにして閉じておくというやりかたか、
　あるいはATmega88のext-fuseを変更して、常に応用プログラムが起動するようにする方法
　もあります。
　（但しext-fuseの変更を行うと、ファームウェアの更新にbootmon(bootloadHID.exe)を
　　使えなくなり、外部のAVRライタが必要になります）


・hidspx-gcc.exe と同じディレクトリに hidspx-gcc.ini を用意して、以下の内容を
　設定してください。

;hidspx-gcc.ini
-d5
-ph:0001
;------

###### EOF ######





