[ARM](ARM.md) 　[LPCXpresso](LPCXpresso.md) 

- - - -
## ■ 概要

- これは、[LPCXpresso](LPCXpresso.md) 用の

<!-- dummy comment line for breaking list -->
HIDクラス・ブートローダー 兼 簡易モニターです。
- CQ出版のTRZ1010N付録基板でも同様に使用できます。

<!-- dummy comment line for breaking list -->

~
このブートローダーを使用することによって、以下のようなことが出来るようになります。
- Cortex-M3用のプログラム開発。
- 内部メモリーやI/Oポートの確認、変更。

<!-- dummy comment line for breaking list -->


- ![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ARM/1343a.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ARM/1343a.jpg) 

<!-- dummy comment line for breaking list -->

~
### TRZ1010Nのコネクタ配置

- 間違いがあればご指摘ください。

<!-- dummy comment line for breaking list -->

P3:　 OPAMP入力用
|1|2|3|4|5|6|
|3.3V|GND|A-D Ch6 in|A-D Ch7 in| AGND(1.25V)|OUT1|

~
P4:　 未解析 OPAMPとRTCへの接続用???
|GND|1|2|GND|
|+3.3V|3|4|+3.3V|
|PIO1-11(AD7)|5|6|OPAMP OUT3|
|PIO1-4 (AD5)|7|8|RTC nINT|
|PIO2-3|9|10|PIO1-2|
|PIO3-1|11|12|OPAMP AGND(1.25V)|
|PIO3-0|13|14|-|
|PIO2-11(SCK)|15|16|RTC SCL|
|PIO1-10(AD6)|17|18|OPAMP OUT4|
|PIO0-9 (MOSI)|19|20|RTC SDI|
|PIO0-8 (MISO)|21|22|RTC SDO|
|PIO2-2 |23|24|-|
|PIO2-10|25|26|-|
|PIO2-9|27|28|-|
|PIO2-5|29|30|-|
|PIO2-4|31|32|TOUCH SWITCH OUT|
|PIO1-9|33|34|OPAMP PIN1|
|PIO0-2 (SSEL)|35|36|RTC CE|
|PIO0-6|37|38|-|
|+3.3V|39|40|RTC Vdd|

~
P2:　 JTAG???
|+3.3V|1|2|PIO0-1(TMS)|
|GND|3|4|PIO0-10(SWCLK)|
|GND|5|6|PIO1-1(TDO)|
|-|7|8|PIO0-11(TDI)|
|GND|9|10|GND|
- LPC1343にはJTAGデバッグ機能は含まれていないはずなので・・・もしかしたらTAP専用???

<!-- dummy comment line for breaking list -->

~
P1:　 SWDデバッグ?
|1|2|3|4|5|6|7|8|
|+3.3V|PIO1-3(SWDIO)|PIO0-10(SWCLK)|PIO0-9(MOSI)|-|PIO0-0(nRESET)|-|GND|
~


P5:　 温度センサーへの接続PIN
|+3.3V|1|2|LM75B A2|
|PIO0-4(SCL)|3|4|LM75B SCL|
|PIO0-5(SDA)|5|6|LM75B SDA|
|PIO2-1|7|8|PIO2-8|
|PIO2-7|9|10|PIO1-8|

~
P6:　 USART?
|PIO2-0(DTR)|1|2|PIO2-6|
|PIO3-3|3|4|PIO1-7(TxD)|
|PIO1-6(RxD)|5|6|PIO1-5(RTS)|
|PIO3-2|7|8|+3.3V|
|GND|9|10|GND|

~
K1:　 2-3間ショートでブートローダー(Windowsからは仮想Diskになる)
|1|-|
|2|PIO0-1(BOOT SELECT)|
|3|GND|
- 2-3間オープンでユーザープログラムの実行

<!-- dummy comment line for breaking list -->

~
K2:　 2-3間ショートでLDO禁止???
|1|-|
|2|LDOレギュレータのCE|
|3|GND|
- デフォルトは全オープン

<!-- dummy comment line for breaking list -->

~

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ARM/lpc1343_lqfp48.png](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ARM/lpc1343_lqfp48.png) 

- - - -
## ダウンロード。

|種別|ファイル|対応基板|
|LPC-1343ブートローダー |[lpc-armon.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/LPC/lpc-armon.zip) |LPCXpresso/TRZ-1010N|



## アプリケーション・サンプルソース
|種別|ファイル|対応基板|
|LPC1343版 仮想COMサンプル|[lpc-sample.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/LPC/lpc-sample.zip) |LPCXpresso(LPC1343)、他|

2011-01-15: 仮想COMサンプルがUSBデバイス登録失敗するバグを直しました。
    - これは、PROD_ID=0003 だったところをHIDデバイスと被るために0004に変更したつもりが、変更に失敗して正しいディスクリプタを生成出来ていなかった問題です。
    - オリジナルのサンプルソースでは0003ですが正常動作していました。0004に書き変えるときに2byte->1byteにしてしまいエンバグさせていました。

<!-- dummy comment line for breaking list -->



- - - -
## ■ 現在のステータス

- ・コンパイラ "CodeSourcery G++ Lite" にて、ビルドは通ります。
- ・通電すると、LEDが点灯します。
- ・HIDクラスとしてＰＣから認識されます。
- ・host/armon.exe を起動すると、メモリーダンプなどが実行できます。
- ・host/armboot.exe を起動すると、HEXファイルをFlashに書き込むことが出来ます。
- ・host/ディレクトリをLinuxでビルドすればLinux上でも同じことが出来るようになっています。
    - Linux上では、CodeSourceryでない、普通のarm用クロスgccでファームウェアをビルドできます。
    - 参考：[ARMクロスコンパイラ構築（Linux）](2010-04#b817f226.md) 

<!-- dummy comment line for breaking list -->


STM32版では、ブートジャンパーあるいはUSERスイッチによって、電源ON直後にブートローダーになるかアプリケーションを起動するかを切り替えることができますが、
- LPC1343版では、今のところブートジャンパーのピン割り当てがありませんので、電源ON直後はブートローダーモードになります。

<!-- dummy comment line for breaking list -->
~
- アプリケーションを書き込んで起動するには

		C:&gt; host\armboot.exe -r main-2000.hex
- を実行します。

<!-- dummy comment line for breaking list -->

- 一度書き込み済みのアプリケーションを起動するには

		C:&gt; host\armboot.exe -r
- もしくは

		C:&gt; host\armon.exe
		ARM&gt; boot 2000
		ARM&gt; q
- を実行します。

<!-- dummy comment line for breaking list -->


## ■ 使い方

- （１）LPC1343のUSBブート機能を利用して、firmware/firmware.bin を書き込みます。（USBマスストレージで出現するCRP DISABLEDというドライブ名のドライブのルートにあるfirmware.binに上書きすればＯＫです。）
- （２）USBでＰＣと接続するとLEDが点灯します。
- （３）host/armon.exe を起動して接続できれば成功です。メモリーダンプ('d')

<!-- dummy comment line for breaking list -->
　　　　 と、メモリー書き換え('ew')が出来ます。
- （４）host/armboot.exe -r firmware/main-2000.hex を実行すると、0000_2000番地に

<!-- dummy comment line for breaking list -->
　         アプリケーションとしてのHIDmonを書き込んで実行します。
- （５）アプリケーション側(0000_2000番地で動作するバージョン)の動作中はLEDが

<!-- dummy comment line for breaking list -->
　　　　　 １秒周期で点滅を行うことで動作の区別をつけるようにしました。
- （６）ブートローダーに制御を戻すには、

		C:&gt; host\armon.exe
		ARM&gt; boot 0
		ARM&gt; q
    - を実行します。
    - もしくはUSBケーブルを一旦抜いて挿しなおします。
- （７）アプリケーションの再度書き換えを実行するときにはブートローダーモードに制御を戻す必要がありますが、アプリケーションがmain-2000.hexと同等の機能を持つものであれば、armboot.exeは自動的にブートローダーを再起動したのち、アプリケーション部分の書き込みを実行することが出来ます。

<!-- dummy comment line for breaking list -->





## ■ userコマンドについて

	  ARM> user
	       ~~~~
- というコマンドを実行すると、usercmd.c に記述されたユーザールーチン

<!-- dummy comment line for breaking list -->
　 を起動することが出来ます。

- 現在は、簡単な _user_puts のテストが出来るようになっています。
- printfを呼び出すようにusercmd.cを変更することも出来ますが、LPC1343のFLASH容量をすこしオーバーするため、書き込んで実行することが出来ません。

<!-- dummy comment line for breaking list -->
（なんらかの容量節減を行う必要があります）


## ■ ＴｏＤｏ

未実装項目

- ・ブートジャンパーによるアプリケーションモードへの切り替え

<!-- dummy comment line for breaking list -->


実装完了項目

- ・高速化
- ・runコマンドが変.
- ・bootコマンド欲しい.
- ・(Application modeで)printが欲しい.
- ・armboot -r オプション.
- ・USB disconnect
- ・他の基板への移植(CQ-STARM,STBeeなど)
- ・逆アセンブラがPIC18Fのまま.

<!-- dummy comment line for breaking list -->

## ■ 補足

- （１） armboot.exeは '-r'オプション指定により、アプリケーションの自動起動が出来るようになりました。

<!-- dummy comment line for breaking list -->

- （２） armon.exe の boot コマンドを用いることで、互いに相手側のファームウェアを再起動することが出来るようになりました。

<!-- dummy comment line for breaking list -->

    - 例： BOOTLOADERからAPPLICATION側に切り替える。
    
    		ARM&gt; boot 2000
    		ARM&gt; q
    - 例： APPLICATION側からBOOTLOADERに切り替える。
    
    		ARM&gt; boot 0
    		ARM&gt; q
- （３）互いに、相手側のファームウェアを更新することが可能です。
    - b.bat  ・・・ BOOTLOADERからAPPLICATION側のファームを書き換えるバッチファイル。
    - bb.bat ・・・ APPLICATION側からBOOTLOADERのファームを書き換えるバッチファイル。

<!-- dummy comment line for breaking list -->

    - BOOTLOADER側の書き換えに失敗した（ファームウェアをバグらせてしまった）場合は、~~外部のJTAGライターが再度必要になります。~~ ---LPC1343の場合はBOOTROMに戻せばUSBマスストレージが出現しますので簡単にファーム書き戻しが可能です。

