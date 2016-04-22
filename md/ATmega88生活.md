*[ｗｓ☆Ｎａｋ](http://www.wsnak.com) ＃[１７２基板](http://www.wsnak.com/kit/172/index.htm) で始めるATmega88生活 [#ie1f34c8]



![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ws0.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ws0.jpg) 


- - - -
## Hardwareについて
- WS#172基板は、回路図を含め仕様が公開されています。
- [http://www.wsnak.com/kit/172/index.htm](http://www.wsnak.com/kit/172/index.htm) 

<!-- dummy comment line for breaking list -->

- ここでは、WS#172基板を料理していきたいと思います。

<!-- dummy comment line for breaking list -->

- まず、必要最小限と思われる部品を載せます。

<!-- dummy comment line for breaking list -->

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ws1.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ws1.jpg) 

- ３.３Ｖ動作させたかったので、バスパワージャンパーJP1の表側2-3間をパターンカットして、ジャンパーの代わりに３端子レギュレータを実装しました。

<!-- dummy comment line for breaking list -->

- 裏面は３端子レギュレータの真ん中の足をＧＮＤに落としています。

<!-- dummy comment line for breaking list -->

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ws2.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ws2.jpg) 

- ちょっと汚いですが、改造箇所は、表面の３端子のJP1 (2-3) パタンカット１箇所と、裏面のJP1 (2) GNDの２つです。
- ポリスイッチR7は手持ちが無かったため、０Ω直結しています。
- ZD1,ZD2は青色LEDで代用し、ＺＤとは逆極性（つまりLEDが点灯するように順方向）接続にしています。
- ５Ｖ動作時は青色LEDがまばゆいほどに点灯しますが、３．３Ｖ動作では丁度良い明るさで点灯し、USBバスの動作の様子を表わしてくれています。
- ３．３Ｖ動作させるので、ＺＤ（もしくは代用LED）は省略可能です。（単に目視チェック用に点灯させているだけです）

<!-- dummy comment line for breaking list -->



- スイッチは'Ｌ'のみを載せています。（bootloadHIDのユーザープログラム起動が出来ます）
- PWR LED , BSY LEDはちゃんと実装しました。
- 水晶はソケット式にして、差し替え可能（クロック変更自在）としました。

<!-- dummy comment line for breaking list -->

~
~
~
- - - -
## Softwareについて

第一弾：

**HIDmonとbootloaderHIDの詰め合わせ**

    - [ATmega88専用ｂｏｏｔｍｏｎ：bootmon-ws172.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/bootmon-ws172.zip) 

<!-- dummy comment line for breaking list -->

    - [ATmega328P対応ｂｏｏｔｍｏｎ：bootmon-ws172-m644.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/bootmon-ws172-m644.zip) 

<!-- dummy comment line for breaking list -->

- これは、bootmon-1127.zip のbootloaderconfg.hを書き換えて、USBの配線をWS#172に合わせただけのものです。

<!-- dummy comment line for breaking list -->

- hidmon88.exeを起動して、PWR LED , BSY LEDを手動で点灯させることが出来ます。
- bootloadHID.exeを起動して、ATmega88の応用領域6kBに任意のhexファイルを転送して実行させることが出来ます。
- 実行後hidmonに制御を移す、あるいはリセットするという操作により、自作プログラムの実行後のメモリー内容

<!-- dummy comment line for breaking list -->
を読み出すことも出来ます。
~

注意:
- ATmega328P用HEXをビルドするときはATmega328Pに対応したAVR-GCCが必要です。
- ATmega88/168用HEXをビルドするときはWinAVRの20060421版もしくはそれに近いバージョンのコンパイラを使用します。（ファームサイズを2kに納める為です。）

<!-- dummy comment line for breaking list -->

~

~
Borland-Cに対応してみました。

**Borland-C対応版HIDmon**ソース入り (ファームはmega328/644用です)
    - [ATmega328P対応ｂｏｏｔｍｏｎ-BorlandC用：bootmon-ws172-bcc55.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/bootmon-ws172-bcc55.zip) 

<!-- dummy comment line for breaking list -->


~
- - - -
第２弾：
#### HIDaspx for ATmega88
- [ATmega88専用HIDaspx：hidspx88-ws172.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/hidspx88-ws172.zip) 

<!-- dummy comment line for breaking list -->


- [ATmega328P対応HIDaspx：hidspx88-ws172-m644.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/hidspx88-ws172-m644.zip) 

<!-- dummy comment line for breaking list -->

注意:
- ATmega328P用HEXをビルドするときはATmega328Pに対応したAVR-GCCが必要です。

<!-- dummy comment line for breaking list -->


~





- - - -
第３弾以降の予定稿
- キーボードまにあ!!!（スリー) for WS#172基板
- microSD access TEST

<!-- dummy comment line for breaking list -->


~
~
~
~
- - - -
## ATmega328P/ATmega644Pのfuseについて。
- 以下のように設定しています。

<!-- dummy comment line for breaking list -->

#### ATmega328P

	E:\bootmon-ws172\bin>hidspx -d4  -rf
	Detected device is ATmega328P.

	Low: 11111111
	     ||||++++-- CKSEL[3:0] システムクロック選択
	     ||++-- SUT[1:0] 起動時間
	     |+-- CKOUT (0:PB0にシステムクロックを出力)
	     +-- CKDIV8 クロック分周初期値 (1:1/1, 0:1/8)

	High:11-11000
	     |||||||+-- BOOTRST (1:Normal, 0:BootLoader)
	     |||||++-- BOOTSZ[1:0] (11:256W, 10:512, 01:1024, 00:2048)
	     ||||+-- EESAVE (消去でEEPROMを 1:消去, 0:保持)
	     |||+-- WDTON (1:WDT通常動作, 0:WDT常時ON)
	     ||+-- SPIEN (1:ISP禁止, 0:ISP許可) ※Parallel時のみ
	     |+-- DWEN (On-Chipデバッグ 1:無効, 0:有効)
	     +-- RSTDISBL (RESETピン 1:有効, 0:無効(PC6))

	Ext: -----101
	          +++-- BODLEVEL[2:0] (111:無, 110:1.8V, 101:2.7V, 100:4.3V)

- - - -
#### ATmega644

	Detected device is ATmega644P.
	
	Low: 11110111
	     ||||++++-- CKSEL[3:0] システムクロック選択
	     ||++-- SUT[1:0] 起動時間
	     |+-- CKOUT (0:CLKOにシステムクロックを出力)
	     +-- CKDIV8 クロック分周初期値 (1:1/1, 0:1/8)

	High:11-11010
	     |||||||+-- BOOTRST ※データシート参照
	     |||||++-- BOOTSZ[1:0] ※データシート参照
	     ||||+-- EESAVE (消去でEEPROMを 1:消去, 0:保持)
	     |||+-- WDTON (1:WDT通常動作, 0:WDT常時ON)
	     ||+-- SPIEN (1:ISP禁止, 0:ISP許可) ※Parallel時のみ
	     |+-- JTAGEN (1:JTAG禁止, 0:JTAG許可)
	     +-- OCDEN (JTAGデバッグを 1:無効, 0:有効)

	Ext: -----101
	          +++- BODLEVEL[2:0] (111:無し, 110:1.8V, 101:2.7V, 100:4.3V)

