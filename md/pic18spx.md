[2010-02](2010-02.md) 　[PICspx](PICspx.md) 

## PIC18F2550/4550/14K50によるAVR/PIC両用ライター
- およびＣ言語でリライトしたHIDmonが１つのファームウェアに納まっています。

<!-- dummy comment line for breaking list -->

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/pic3.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/pic3.jpg) 

左：PIC18F2550　中：18F14K50+PIC24FJ64　右：18F14K50+ATmega88((写真に写っているPIC24FJ64やATmega88チップはライターの書き込みターゲット、もしくはシリアル通信の相手としてオンボード実装しているだけです。pic18spxそのものの動作に必要と言う訳ではありません。))


- 10PinのISPコネクタは上段がファーム更新用で、下段がターゲットマイコンへの接続用です。

<!-- dummy comment line for breaking list -->
~
- 真ん中の基板は18F14K50を3.3V動作させて、オンボードのPIC24FJ64にそのまま接続されています。
- PIC24F用の書き込みソフトも作成しました。

<!-- dummy comment line for breaking list -->

~
- 右側の基板はArduino互換機兼PIC/AVR両用ライターです。
    - Arduinoとして動作させるときはPIC18F14K50のファームを差し替えてUSBシリアル変換器として動作させます。

<!-- dummy comment line for breaking list -->


~
- - - -
ここに書かれている内容は分かりにくいと思いますので、初めて制作される方は~
以下のkumanさんの解説ページも合わせてお読みください。
- [http://www.geocities.jp/kuman2600/o19pic18spx.html](http://www.geocities.jp/kuman2600/o19pic18spx.html) 

<!-- dummy comment line for breaking list -->

- 分かりやすい解説を作られたkumanさんに感謝します。

<!-- dummy comment line for breaking list -->

~
~



- - - -

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/14k50.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/14k50.jpg) 
![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ae18f2550.jpg](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/ae18f2550.jpg) 

■ 概要

	 これは、MicroChipが提供している PIC18 のUSBアプリケーションフレームワーク
	 に含まれているHID-Bootloader をもとにして作成した PIC18F専用の簡易モニタ（兼PIC/AVR両用ライター）です。

■ 現在のステータス

	 （１）PIC 18F2550/4550/14K50上で動作します。
	 （２）簡易モニタとして、picmon.exe が使えます。（メモリー、I/Oの参照と変更が可能です。）
	 （３）HIDaspx の仮バージョンが動きます。（AVRライターとして使えます。）
	 （４）picwrite.exe の仮バージョンが動きます。 (LVP書き込み専用ですがPICライターとして使えます。)
	 （５）writer24.exe の仮バージョンが動きます。 (PIC24Fライタ)

■ 試し方

- まず、[pic18boot](pic18boot.md) を使用してファームウェアを書き込みます。

<!-- dummy comment line for breaking list -->

	 C:> picboot -r firmware/picmon-18F2550.hex

- デバイスが18F2550/18F4550の場合は、picmon-18F2550.hexを、18F14K50の場合はpicmon-18F14k50.hexを書き込んでください。

<!-- dummy comment line for breaking list -->


	 次に、picmon を起動します。
	 C:> cd host
	 C:host> picmon.exe 

- ~~現在実装されているのは HIDCMD_PEEK と POKEだけです。~~
- 以下のコマンドは有効です。

		"* HID_Monit Ver 0.1\n"
		"Command List\n"
		" d  &lt;ADDRESS1&gt; &lt;ADDRESS2&gt;    Dump Memory(RAM)\n"
		" dr &lt;ADDRESS1&gt; &lt;ADDRESS2&gt;    Dump Memory(EEPROM)\n"
		" dp &lt;ADDRESS1&gt; &lt;ADDRESS2&gt;    Dump Memory(PGMEM)\n"
		" e  &lt;ADDRESS1&gt; &lt;DATA&gt;        Edit Memory\n"
		" f  &lt;ADDRESS1&gt; &lt;ADDRESS2&gt; &lt;DATA&gt; Fill Memory\n"
		" l  &lt;ADDRESS1&gt; &lt;ADDRESS2&gt;    List (Disassemble) PGMEM\n"
		" p ?                         Print PortName-List\n"
		" p .                         Print All Port (column format)\n"
		" p *                         Print All Port (dump format)\n"
		" p &lt;PortName&gt;                Print PortAddress and data\n"
		" p &lt;PortName&gt; &lt;DATA&gt;         Write Data to PortName\n"
		" p &lt;PortName&gt;.&lt;bit&gt; &lt;DATA&gt;   Write Data to PortName.bit\n"
		" sleep &lt;n&gt;                   sleep &lt;n&gt; mSec\n"
		" bench &lt;CNT&gt;                 HID Write Speed Test\n"
		" boot                        Start user program\n"
		" run  &lt;address&gt;              Run user program at &lt;address&gt;\n"
		" user &lt;arg&gt;                  Run user defined function (usercmd.c)\n"
		" poll &lt;portName&gt;             Continuous polling port\n"
		" poll &lt;portName&gt; &lt;CNT&gt;       continuous polling port\n"
		" poll  *  &lt;CNT&gt;              continuous polling port A,B,D\n"
		" graph &lt;portName&gt;            Graphic display\n"
		" q                           Quit to DOS\n"
- 実行例

		TARGET DEV_ID=25
		PIC&gt; d
		000000 00 00 40 00 20 78 02 00
		000008 08 41 22 68 05 00 07 20
		000010 bf 04 02 80 ac 40 84 41
		000018 5d 44 88 83 05 03 c4 41
		000020 fe 20 85 84 0c e9 24 cb
		000028 36 50 8c 22 22 f8 51 21
		000030 04 44 25 dc 02 c0 31 14
		000038 81 09 48 d4 43 10 d6 35
		PIC&gt; l 0
		0000 6af7        clrf   0xf7, 0
		0002 6af8        clrf   0xf8, 0
		0004 d054        bra    0xae
		0006 d330        bra    0x668
		0008 ef04 f004   goto   0x808
		000c d05d        bra    0xc8
		000e d27b        bra    0x506
		0010 d328        bra    0x662
		0012 d32c        bra    0x66c
		0014 d32f        bra    0x674
		0016 d345        bra    0x6a2
		0018 ef0c f004   goto   0x818
		001c 0112        movlb  0x12
		001e 0200        mulwf  0, 0
		0020 0000        nop
		0022 4000        rrncf  0, 0, 0
		0024 04d8        decf   0xd8, 0, 0
		0026 003c        dw     0x3c  ;unknown opcode
		PIC&gt; q
		Bye.
		#
		C:&gt;
- ■ ディレクトリ構成

		+- picspx/ -+-- picspx/     AVRライター    (gcc)
		            |
		            +-- picwriter/  PIC18Fライター (gcc)
		            |
		            +-- writer24/   PIC24Fライター (gcc)
		            |
		            +-- picmon/     簡易モニター   (gcc)
		            |
		            +-- firmware/   ファームウェア (mcc18)
- ■ 回路図 (PIC 18F4550)
- [circuit.txt](circuit.txt.md)  を参照してください
- ■ AVRライターとしての試し方
- 18F2550/18F4550の４本のSPI信号を書き込み先のAVRチップに接続します。
- 18F4550上のpin番号:信号名  = SPI端子(AVRチップ上のピン名称)

				-----------------------------
		 				25:RC6=TX  = Reset
						33:RB0=SDI = MISO
		 				34:RB1=SCK = SCK
		 				26:RC7=SDO = MOSI
				-----------------------------
		C:&gt; cd picspx
		C:picspx&gt; picspx-gcc.exe  -r
- のようにして使います。

		C:picspx&gt; picspx-gcc.exe
- だけを入力すると使い方の表示が出ます。
- picspx-gcc.ini が同じディレクトリにない場合は、少なくとも -ph
- オプションを手動で指定する必要があります。
- ■ PICライターとしての試し方

		 接続:
		AVR用ISP 6PIN          書き込み対象 PIC18F2550/14K50

<!-- dummy comment line for breaking list -->
- - - -
	1 MISO    ------------------>   PGD
	2 Vcc     ------------------>   Vcc
	3 SCK     ------------------>   PGC
	4 MOSI    ------------------>   PGM
	5 RESET   ------------------>   MCLR
	6 GND     ------------------>   GND

	・picwriter/picwrite.exe -r 
	　を実行して、PICのデバイスIDが表示できれば、接続はうまくいっています。
	・LVP書き込みモードのないPICや、既にHVP(高電圧)書き込み済みのPICは
	  このPICライターでは読み書きできません。
	・LVP書き込み後のマイコン使用時は常時PGM端子を「プルダウン」して使用する
　 必要があります。


- - - -
■ hidmon-14k50 や hidmon-2550との違い

	・一応、Ｃ言語で書かれています。(mcc18)

	・コードサイズが大きいです。(5kB程度)

	・USB のPIDが違います。

	・プロトコルも違います。

	・HID Reportの転送方法が異なります。
	  hidmonでは、全てコントロール転送(HidD_SetFeature/HidD_GetFeature)
	  で行っていますが、picmonit.exeでは、EndPoint1に対するインタラプト転送
	  によりデータの受け渡しを行っています。

	  このため、インタラプト転送パケットがデバイスより送出されない限りは
	  HID Reportを受け取ることができません。（ホストがタイムアウトになります）
	  送出された場合は、必ず受け取っておかないと、次パケットの受信時に、バッファ
　 にたまった古いパケットを受け取って混乱することになります。


■ 何に使えますか？

	・picmon.exe 上のコマンドと、firmware上のコマンド受け取り処理の対を
	　自分で追加することにより、いろいろな機能を追加することが出来ます。


graphコマンドによる、10kHzサンプリング画像

	PIC> graph

	PIC> graph portc trig 1

![https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/infrared2.png](https://github.com/iruka-/ATMEL_AVR/blob/master/web/jpg/PIC/infrared2.png) 

アナログサンプリングは仮実装

	PIC> graph analog

	PIC> graph analog trig

サンプリング周期の変更方法
- firmware/main.c の修正によるもの
- picmon.exe から、 p pr2 XX のようにしてPeriod2レジスタを書き換える方法
- さらに、T2CONレジスタをいじってプリスケーラーも変更する方法、など。

<!-- dummy comment line for breaking list -->

- ただし、データ引取り周期は1mS/パケット(64bye)が限度なので、60kHzが理論上の上限です。（これより速くしたい場合はCDCデバイスなどに変更したほうが良いでしょう）
- 30kHzでもUSBのフレームが間引かれた場合データ落ちが起きる可能性があります。（一応256byteのバッファは存在します）
- アナログの場合１サンプル2byteなので30kHz以上には出来ません。

<!-- dummy comment line for breaking list -->




- - - -

## ダウンロード

PIC側ファームウェア・HEXファイル、ソースファイル、
Windows側コマンドラインツール等一式。(2014-03-18: Windows8.1でデバイス認識しない問題を改善)
- [pic18spx-2010-0416.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/pic18spx-2010-0416.zip) 

<!-- dummy comment line for breaking list -->

~

Windows8.1対応PIC側ファームウェアのみ(ソース込み)
- [pic18spx-2014-0317.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/pic18spx-2014-0317.zip) 
        - こちらは、新しいＭＬＡ(microchip-libraries-for-applications-v2013-06-15-windows-installer.exe)をベースに新規実装したものです。
        - このバージョンならば頑張れば mcc18ではない、XC8でビルドすることも不可能ではありません。（要確認）
        - 今のところ、PIC18F2550でしかビルド出来ません。
        - picmon.exeで動作確認を行いましたが、AVR/PICライターの動作確認はまだです。

<!-- dummy comment line for breaking list -->


~

Linux版のhidspxコマンドラインツール（**AVRライターソフト**）。（少しの変更でMacOSXでも動作すると思います）
- [pic18spx-linux.zip](https://github.com/iruka-/ATMEL_AVR/blob/master/web/upload/PIC/pic18spx-linux.zip) 

<!-- dummy comment line for breaking list -->


~
**おまけ**

HIDaspx(AVRライター)しか持っていない場合でもPIC18Fxxxxにファームを焼けます。
- read more : [PICspx](PICspx.md) 

<!-- dummy comment line for breaking list -->

~
Linuxホストからpicmon,picbootを試すことも出来ます。picmon/ ディレクトリで再コンパイルを行ってください。
- 実行には今のところルート権限が必要です。

<!-- dummy comment line for breaking list -->

~
~
**おまけ２**

## 赤外線リモコン信号の解析機能を実装済みです。

上記**ダウンロード**のアーカイブ(pic18spx-XXXX.zip)を取得してください。

- ■ 概要

<!-- dummy comment line for breaking list -->

	  PIC18F2550/4550を使用して、赤外線リモコンの受光波形を観測することが出来ます。

■ 使い方


	  （１）firmware/picmon-18f2550.hex を焼きます。
	  （２）リモコン受信器(38kHz)の出力端子をPICのPortA.bit0に繋ぎます。
	  （３）PICデバイスをUSB経由で接続した後、 picmonit.exe を起動します。
	
	C:picspx> picmon\picmonit.exe

	PIC> プロンプトが出たら、以下のコマンドを入力します。
	PIC> graph porta infra

	  （４）リモコン受信器に向けて、家電協規格の赤外線リモコン信号を送ります。

	するとコンソールに、解析したビットを表示します。

	HDR:01000000_00000100_00000001_00000000_10001000_10001001_ HEX:40 04 01 00 88 89
	HDR:01000000_00000100_00000001_00000000_00001000_00001001_ HEX:40 04 01 00 08 09
	HDR:01000000_00000100_00000001_00000000_00001000_00001001_ HEX:40 04 01 00 08 09
	HDR:01000000_00000100_00000001_00000000_11001000_11001001_ HEX:40 04 01 00 c8 c9
	HDR:01000000_00000100_00000001_00000000_00101000_00101001_ HEX:40 04 01 00 28 29

■ HEXの内容について

	先頭２バイト＝メーカーコード
	次の３バイト＝リモコンボタンに応じた固有コード
	最後の１バイト＝ボタン固有コード３バイトのチェックサム（ＸＯＲ値???）


■ 家電協以外のリモコンは？

	現在のところ対応していません。（主にソニー、ＮＥＣ方式）
	対応方法としては、picmon/infra.c に追加機能的に実装することができると思います。

