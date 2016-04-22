関連リンク　[Pinguinoで遊ぼう](Pinguino.md) 　[ブートローダーを作る](HIDBootX.md) 　[USB仮想シリアル](PIC32MX_USBCDC.md) 　[USBカスタムデバイス](USB_CUSTOM.md) 　[USBホスト](USB_HOST.md) 　[2013-02](2013-02.md) 　[2013-03](2013-03.md) 

## PIC32MXを使用したUSBオシロスコープの試作

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/picscope.png](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/picscope.png) 

~
~
## 現在のステータス
- WinUSB.DLL経由にて、ファームウェアとWindowsパソコン間で連続したA/D変換サンプルデータの受け渡し

<!-- dummy comment line for breaking list -->
を行なっています。
- OpenGLを使用した波形表示ソフトが動作します。

<!-- dummy comment line for breaking list -->

~
出来ていないところ
- ~~マルチチャネル対応~~ --- 出来ました。
- ~~時間軸の変更（な、なんだってーー（AA略）~~ --- 出来ました。
- ドキュメント等

<!-- dummy comment line for breaking list -->

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/picscope2.png](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/picscope2.png) 


~

- - - -
## ■ 配線図         PIC32MX220F032B

	               3.3V
	                |
	                *------10Ω--------------+
	               10k                       |
	                |       ___    ___       | 0.1u
	  ラ   -->  ----*-MCLR [1  |__| 28] AVDD-*-||---GND
	  イ   -->  --PGD3/RA0 [2       27] AVSS--------GND  LED
	  ター -->  --PGC3/RA1 [3       26] RB15--1kΩ-------|＞|--GND
	  A/D変換入力----> RB0 [4       25] RB14
	                   RB1 [5       24] RB13
	              SDA2/RB2 [6       23] Vusb3v3--------3.3V
	              SCL2/RB3 [7       22] usb D-
	   Xtal     GND----Vss [8       21] usb D+   +10uF
	+-----------------OSC1 [9       20] Vcap------||---GND
	*--|□|--*--------OSC2 [10      19] Vss------------GND
	|  8MHz  |    U1TX/RB4 [11      18] RB9
	22pF    22pF  U1RX/RA4 [12      17] RB8
	|        |   3.3v--Vdd [13      16] RB7
	|        |         RB5 [14      15] Vbus-----------USB Vbus(5V)
	GND    GND              ~~~~~~~~~~
	           
	           
- A/D入力レンジは0V〜3.3Vです。
- A/D入力は~~最終的にはAN0〜AN3になる予定です。（現在はAN2を固定選択しています）~~ -- AN0〜AN3の4chを使用します。

<!-- dummy comment line for breaking list -->


~

- ファーム書き込みはPicKit2やPicKit3で書き込んでもＯＫですし、
- [HIDBoot.X](HIDBootX.md) というHIDブートローダーから書き込んでもＯＫです。
    - （このファームウェアHEXファイルはセルフブートとHID Bootloaderからのブートの両方に対応しています）

<!-- dummy comment line for breaking list -->

~
- - - -
## ダウンロード

作りかけですが、公開しています。
    - **ダウンロード:**[PICscope.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/PICscope.zip) 

<!-- dummy comment line for breaking list -->

~
PIC32MX用のファームウェアが必要です。こちらからダウンロード
    - **ダウンロード:**[pic32mon.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/pic32mon.zip) 

<!-- dummy comment line for breaking list -->

~


~

- このファームウェアは[USB簡易モニター](PIC32mon.md) にA/D連続変換のコマンドを追加したものとなっています。
    - USB簡易モニターの機能はすべて含んでいますので、pic32mon.exeを使用してファームウェアを対話的に操作可能です。

<!-- dummy comment line for breaking list -->
~
- - - -
## ビルド環境
- Windows(XP|Vista|7|8)側は、[MinGW32](http://www.mingw.org/)  gccを使用します。（無償）

<!-- dummy comment line for breaking list -->

~
#### ファームウェアを再ビルドされる場合は、
- MIPS32(PIC)側は、Pinguinoのmips-gccコンパイラを使用します。（無償）
- 但し、MChipUSBフレームワークを使用する為、MPLAB IDE 8.89と、それに付属のPIC32 Cコンパイラ環境が必要です。（無料版でＯＫです）
- それからMChipUSBフレームワーク(microchip-application-libraries-v2013-06-15-windows-installer.exe)も必要です。（無償）

<!-- dummy comment line for breaking list -->

~
- ビルド方法は、いずれの場合でも、Windows(32|64)のコマンドライン上からmakeを実行することで行います。

<!-- dummy comment line for breaking list -->

~
- WindowsXPへのWinUSBの導入は、MChipUSBフレームワークに含まれるサンプルソース(  USB/Device - WinUSB - High Bandwidth Demo/Driver and INF/MCHPWinUSBDevice_v2.inf )のドライバーを導入すると自動的にWinUSB.DLLが使用できるようになります。
- Windows Vista以降ではWinUSBはOSにバンドルされているようです。
- もし（万が一）Linuxやそれに類する(MACOSとか)環境からPIC32MXを叩きたい場合は、WinUSBの代わりにLibUSBを使用してください。USB周りのAPIが変わりますが、それほど本質的な違いはありません。

