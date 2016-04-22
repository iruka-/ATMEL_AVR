[pic18spx](pic18spx.md) 

■ 配線            PIC18F2550


	                       (PICライターへの接続はファームの更新が不要な場合は省略できます)
	              +----------------------------- PIC書き込み器より
	              |   ___    ___
	             MCLR [1  |__| 28] RB7/PGD <---- PIC書き込み器より
	              RA0 [2       27] RB6/PGC <---- PIC書き込み器より
	              RA1 [3       26] RB5/PGM <---- PIC書き込み器より(LVP書き込みのみ)
	              RA2 [4       25] RB4
	              RA3 [5       24] RB3
	              RA4 [6       23] RB2         |書き込み側AVR-chip|
	              RA5 [7       22] RB1 ------->|   SCK            |
	   Xtal       Vss [8       21] RB0 ------->|   MISO           |
	+----------> OSC1 [9       20] Vdd         |                  |
	+---|□|-+-> OSC2 [10      19] Vss         |                  |
	|  20MHz |    RC0 [11      18] RC7 ------->|   MOSI           |
	15pF    15pF  RC1 [12      17] RC6 ------->|   RESET          |
	|        |    RC2 [13      16] RC5/D+  --------------------------> USB D+
	|        |  +Vusb [14      15] RC4/D-  --------------------------> USB D-
	GND    GND  |     ~~~~~~~~~~
	            |
	         0.3uF
	            |
	           GND

■ スイッチ、ＬＥＤ等

	            1:MCLR <--+-10kΩ--------- Vdd
	                      |
	                   RESET_SW
	                      |
	                     ~~~
	                     GND

	                               LED0
	            11:RC0 <----1kΩ---|＞|--- GND

	                               LED1
	            12:RC1 <----1kΩ---|＞|--- GND

	            13:RC2 <--+-10kΩ--------- Vdd
	                      |
	                   BOOT_SW
	                      |
	                     ~~~
	                     GND

	            26:RB5/PGM <----4.7kΩ---- GND  (LVP書き込みを行った場合必須)


■ 電源等

	            20:Vdd <---+---- USB +5V もしくは外部 +5V
	                       |
	                     10uF
	                       |
	            19:Vss <---+---- GND (USB GND)
	            
	             8:Vss <-------- GND

____________________________________________________________________________
PIC18F2550
	17:RC6=TX  = Rset
	21:RB0=SDI = MISO
	22:RB1=SCK = SCK
	18:RC7=SDO = MOSI


PIC18F4550
	25:RC6=TX  = Rset
	33:RB0=SDI = MISO
	34:RB1=SCK = SCK
	26:RC7=SDO = MOSI

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/pic18f4550.png](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/pic18f4550.png) 

- （準備中）

<!-- dummy comment line for breaking list -->

- - - -
使用部品

|CPU|PIC18F2550|1|
|Xtal|20MHz|1|
|C|15pF|2(入手難な場合は省略しても発振します。周波数に誤差が出ることがあります)|
|C|0.2〜0.3uF(うち１個は10uFと並列に入れてください)|2|
|C|10uF|1|
|R|4.7kΩ(PGMプルダウン用)|1|
|R|10kΩ(Resetプルアップ用)|1|
|R|1kΩ(LEDと直列に入れます)|2|
|LED|任意色（お好きな色で２色がいいかも）|2|
|SW|Push SW（BOOTとRESET）|2|

- 高電圧書き込みを行わないので、部品点数が非常に少ないです。

<!-- dummy comment line for breaking list -->
____________________________________________________________________________






■ 配線            PIC18F14K50

	             +------------------------------------------------> USB +5V
	Xtal 12MHz   +Vdd [1  |__| 20] Vss ---------------------------> USB GND
	+----------> OSC1 [2       19] USB D+/PGC --------------------> USB D+
	+---|□|-+-> OSC2 [3       18] USB D-/PGD --------------------> USB D-
	22pF    22pF MCLR [4       17] Vusb -----------0.3uF---GND
	GND     GND   RC5 [5       16] RC0 
	              RC4 [6       15] RC1 
	          PGM/RC3 [7       14] RC2 
	Reset<--------RC6 [8       13] RB4/SDI ------->|   MISO           |
	MOSI <--------RC7 [9       12] RB5             |                  |
	 ||           RB7 [10      11] RB6/SCK ------->|   SCK            |
	 ||                ~~~~~~~~~~                  |書き込み側AVR-chip|
	書き込み側AVR-chip


■ スイッチ、ＬＥＤ等

	            4:MCLR <--+-10kΩ--------- Vdd
	                      |
	                   RESET_SW
	                      |
	                     ~~~
	                     GND

	                               LED0
	            16:RC0 <----1kΩ---|＞|--- GND

	                               LED1
	            15:RC1 <----1kΩ---|＞|--- GND

	            14:RC2 <--+-10kΩ--------- Vdd
	                      |
	                   BOOT_SW
	                      |
	                     ~~~
	                     GND

	             7:RC3/PGM <----4.7kΩ---- GND  (LVP書き込みを行った場合必須)


■ 電源等

	             1:Vdd <---+---- USB +5V もしくは外部 +5V
	                       |
	                     10uF
	                       |
	            20:Vss <---+---- GND (USB GND)
	            

____________________________________________________________________________

