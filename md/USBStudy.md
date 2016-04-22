[2014-08](2014-08.md) 

## [UsbStudy](http://www.kumikomi.net/interface/contents/201412.php) をPIC32MX に移植する


現時点では、
- Pinguino gccを使用してビルドが通る。
- シリアル、LEDは動作している
- ~~USB HOSTはAttach Detachの検出まで出来ているが、BDT周りに問題があって動いていない。~~ ---> なんとなく動いています。

<!-- dummy comment line for breaking list -->

~
非互換の原因になっている問題
- BDT(Buffer Descriptor Table)の構造が違う。
    - PIC24Fでは BDTは1個あたり4byte。内訳は BDSTAT(6bit)        , COUNT(10bit) , BufferAddress(16bit)
    - PIC32MXでは BDTは1個あたり8byte。内訳は BDSTAT(2byte中6bit) , COUNT(2byte中10bit) , BufferAddress(32bit)

<!-- dummy comment line for breaking list -->

- BDTの配置が異なる
    - PIC24FではNO PING PONGモードで使用。BDTは稠密に並んでいます ( EP0〜15 x IN/OUT )
    - PIC32MXではFULL PING PONGモードしか存在せず、EP0〜15(x Even / Odd)( x IN/OUT)でPING PONGモード（EvenバッファとOddバッファ交代を毎回行う）が強制的に採用されます。

<!-- dummy comment line for breaking list -->

~
~
~

## とりあえずダウンロード

- ソース：[UsbStudy.X.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/UsbStudy.X.zip) 
- 各種デバッグプリント(UART向け)の埋め込みは可能。

<!-- dummy comment line for breaking list -->

~
~
~

## ■ 配線         PIC32MX220F032B 8MHz水晶使用.

	               3.3V
	                |
	                *------10Ω--------------+
	               10k                       |
	                |       ___    ___       | 0.1u
	  ラ   -->  ----*-MCLR [1  |__| 28] AVDD-*-||---GND
	  イ   -->  --PGD3/RA0 [2       27] AVSS--------GND  LED
	  ター -->  --PGC3/RA1 [3       26] RB15--1kΩ-------|＞|--GND
	                   RB0 [4       25] RB14
	                   RB1 [5       24] RB13
	              SDA2/RB2 [6       23] Vusb3v3--------3.3V
	              SCL2/RB3 [7       22] usb D-
	   Xtal     GND----Vss [8       21] usb D+   +10uF
	+-----------------OSC1 [9       20] Vcap------||---GND
	*--|□|--*--------OSC2 [10      19] Vss------------GND
	|  8MHz  |    U1TX/RB4 [11      18] RB9                        *--10k------3.3V
	22pF    22pF  U1RX/RA4 [12      17] RB8                        |
	|        |   3.3v--Vdd [13      16] RB7 -----------------------*--BOOT_SW--GND
	|        |         RB5 [14      15] Vbus-----------USB Vbus(5V)
	GND    GND              ~~~~~~~~~~

- PIC32MX250F128Bでも回路図、ファームウェアは共通に使用できます。
    - （ファームウェアサイズはFlash32K,SRAM8Kに十分収まっています）

<!-- dummy comment line for breaking list -->


## UsbStudy ファームウェアの挙動

- USBのD+/D-は、適当なUSBデバイス(KB,Mouse,...)に繋ぎます。(Type-Aメスに配線する)
- PIC32MX側はUSB HOSTになります。

<!-- dummy comment line for breaking list -->

~

- U1TX/U1RXを適当な3.3VレベルのUSB-Serial変換器などに繋いで、WindowsのTeratermなどで接続します。
- ボーレートは9600BPS

<!-- dummy comment line for breaking list -->



~

	D:\pic32\UsbStudy.X>w32term -c3 -b9600
	9600
	Entering TerminalMode: Escape=^A
	
	>?
	's':    Get string
	'g':    get descriptor command
	'a':    Set address command
	'c':    set config command
	'I':    IN command
	'i':    IN command
	'o':    OUT command (use default DATA0/1 sequence)
	't':    SETUP command
	'w':    Clear & Write command
	'W':    WRITE command (Append to current send buffer)
	'r':    Reset command
	'd':    Mem Dump
	
	>
## 操作例

- TeraTerm等でファームウェアを操作します。
- 基本的にTeratermからコマンドを投入して、結果を表示するようになっています。
- 最初に'a'コマンド投入が必要です。

<!-- dummy comment line for breaking list -->

	D:\pic32\UsbStudy.X>w32term -c3 -b9600
	9600
	Entering TerminalMode: Escape=^A
	
	>?
	's':    Get string
	'g':    get descriptor command
	'a':    Set address command
	'c':    set config command
	'I':    IN command
	'i':    IN command
	'o':    OUT command (use default DATA0/1 sequence)
	't':    SETUP command
	'w':    Clear & Write command
	'W':    WRITE command (Append to current send buffer)
	'r':    Reset command
	'd':    Mem Dump
	
	>
	>a
	Address
	Ok
	>s1
	Microsoft
	>s2
	ゥ 2000 Microsoft Corp. All rights reserved.
	>s3
	Microsoft 3-Button Mouse with IntelliEye(TM)
	>g
	12 01 10 01 00 00 00 08 5e 04 40 00 00 03 01 03
	00 01
	>



~
~
~
~
~
- - - -
## おまけ：PIC32MXでUSB HOSTを作る場合

- 普通にMicrochipのサンプルがあるので、それをもとに作ればハードウェアの操作を直接行う必要はありませんが、
- レジスタ手打ちの場合のやりかたなど

<!-- dummy comment line for breaking list -->

基本は、[マニュアル](http://ww1.microchip.com/downloads/jp/DeviceDoc/jp534259.pdf) のHOST Mode Operationを読めばだいたいの手順が書いてあります。

- HOSTモードではEP0しか使わない。
    - Deviceで動作させるときはEP0〜15が使えますが、HOSTモードではEP0のみ有効にしてEP1〜15はDisableにします。
- HOSTからDeviceへのUSBパケットはU1TOKレジスタにトークンを書き込む。
    - トークン(PID)は3種類しかありません。

<!-- dummy comment line for breaking list -->
,Packet ID 4bit,意味
,0001,OUT(TX)
,1001,IN(RX)
,1101,SETUP(TX)

~
~
~
~
- - - -
## USB Hostを弄れるマイコン/基板をリストアップ
- Raspberry-Piとか使うと、普通にLinuxでUSB HOSTでドライバーは全部アリ。もちろんHubにも対応している。
- それだとUSB HOSTを弄るというよりは、普通にPC使うのと同じ。

<!-- dummy comment line for breaking list -->

~
マイコンの場合は、

,ICチップ、あるいは基板名称(リンク先は全部秋月です),お値段（秋月）,スペック,備考
,[PIC24FJ64GB002](http://akizukidenshi.com/catalog/g/gI-08168/) ,DIP28 @340円,64KFlashROM/8KRAM/16bitCPU 32MHz,(16MIPS)
,[PIC32MX220F032B](http://akizukidenshi.com/catalog/g/gI-05852/) ,DIP28 @250円,32KFlashROM/8KRAM/32bitCPU 48MHz,
,[PIC32MX250F128B](http://akizukidenshi.com/catalog/g/gI-07644/) ,DIP28 @360円,128KFlashROM/32KRAM/32bitCPU 48MHz,
,[STM32 NUCLEO-F401RE](http://akizukidenshi.com/catalog/g/gM-07723/) ,1500円,[STM32F401RET6](http://www.st-japan.co.jp/web/jp/catalog/mmc/FM141/SC1169/SS1577/LN1810/PF258797)  Flash512K/96kSRAM/84MHz,参考URL = [http://developer.mbed.org/users/va009039/code/F401RE-USBHost/](http://developer.mbed.org/users/va009039/code/F401RE-USBHost/) 
,[STM32 STM32F4DISCOVERY](http://akizukidenshi.com/catalog/g/gM-05313/) ,1750円,[STM32F407VGT6](http://akizukidenshi.com/download/ds/st/STM32F4DISCOVERY.pdf)  Flash1MB/192kSRAM/168MHz,参考URL = https://github.com/YuuichiAkagawa/STM32F4_ADK/wiki

- 予算2000円以内という感じでは、こんなところ。
- PIC18FにはUSB-HOSTは付いてない。
- PIC24Fは1命令2CLOCK
- PIC32MXは40MHzもしくは48MHz(規格上は50MHzが上限で、USB縛りがある時は8MHzの倍数で動作)

<!-- dummy comment line for breaking list -->

~

- ARM Cortex-M3系でUSB-HOST機能付きMCUは、そんなに多くない。
- 意外とRasp-Piのほうがお勧めかなー。半田付けとか**電子工作さえ不要**だし。
    - ドライバー書かなくても、全部揃ってる(^笑^)
    - いやーLinuxって、ほんと便利っすね。ソース全部あるし（当然）
    - こんな2000円程度のARMパソコン（Rasp-Piのこと)でも、フルサイズのLinuxが動くんだよー。PC用のLinuxと全く同じものなんだよー(CPUアーキテクチャーと性能は当然異なります)。すごいだろー（今となっては全部当たり前）

<!-- dummy comment line for breaking list -->


~
~
~
~
~

